#include "pch.h"
#include "pointers.h"
#include "memory.h"

namespace t2c::pointers {

namespace {

const constinit memory::Pattern<21> GWORLD_PATTERN{
    "48 39 3D ????????"  // cmp [Talos2-Win64-Shipping.exe+88C5E00], rdi
    "75 07"              // jne Talos2-Win64-Shipping.exe+518C65D
    "48 89 1D ????????"  // mov [Talos2-Win64-Shipping.exe+88C5E00], rbx
    "E8 ????????"        // call Talos2-Win64-Shipping.exe+51052C0
    ,
    3};
uintptr_t gworld_base_ptr{};

const constexpr auto GWORLD_LEVEL_OFFSET = 0x30;
const constexpr auto LEVEL_GAMEMODE_OFFSET = 0xA8;
const constexpr auto GAMEMODE_PAWN_OFFSET = 0x68;

const constexpr auto PAWN_COLLISION_COMP_OFFSET = 0x1C0;
const constexpr auto PAWN_MOVE_COMP_OFFSET = 0x348;
const constexpr auto PAWN_GOD_OFFSET = 0x660;

const constexpr auto CYLINDER_COMP_POS_OFFSET = 0x260;

const constexpr auto MOVE_COMP_TURBO_OFFSET = 0xEF8;

const constinit memory::Pattern<23> TALOS_CHAR_ENABLE_GHOST_PATTERN{
    "40 53"              // push rbx
    "48 83 EC 20"        // sub rsp, 20
    "B2 01"              // mov dl, 01
    "48 8B D9"           // mov rbx, rcx
    "E8 ????????"        // call Talos2-Win64-Shipping.exe+2561C50
    "48 8B 83 ????????"  // mov rax, [rbx+00000348]
};

const constinit memory::Pattern<26> TALOS_CHAR_ENABLE_WALK_PATTERN{
    "40 53"              // push rbx
    "48 83 EC 20"        // sub rsp, 20
    "33 D2"              // xor edx, edx
    "48 8B D9"           // mov rbx, rcx
    "E8 ????????"        // call Talos2-Win64-Shipping.exe+249B260
    "48 8B 8B ????????"  // mov rcx, [rbx+00000348]
    "48 85 C9"           // test rcx, rcx
};

using UTalosCharacter = void;

using talos_char_enable_ghost_func = void (*)(UTalosCharacter* self);
talos_char_enable_ghost_func talos_char_enable_ghost_ptr{};

using talos_char_enable_walk_func = void (*)(UTalosCharacter* self);
talos_char_enable_walk_func talos_char_enable_walk_ptr{};

const constinit memory::Pattern<18> SHOW_CHEATS_PATTERN{
    "0F84 ????????"      // je Talos2-Win64-Shipping.exe+25733AB
    "49 8B 8E ????????"  // mov rcx, [r14+00000288]
    "33 D2"              // xor edx, edx
    "48 8B 01"           // mov rax, [rcx]
};
const constexpr uint8_t SHOW_CHEATS_INJECTED_CODE[] = {
    0x90,  // nop
    0x90,  // nop
    0x90,  // nop
    0x90,  // nop
    0x90,  // nop
    0x90,  // nop
};

/**
 * @brief Safely dereferences a chain of offsets, short circuiting if any become invalid.
 *
 * @tparam R The return type to cast to.
 * @param base The base address.
 * @param offsets The offsets to add.
 * @return The final address behind all the offsets, or 0/nullptr if any pointer was invalid.
 */
template <typename R = uintptr_t>
R safe_dereference(uintptr_t base, std::initializer_list<ptrdiff_t> offsets) {
    if (base == 0) {
        return 0;
    }

    HANDLE this_proc = GetCurrentProcess();

    uintptr_t ptr = base;
    for (const auto& offset : offsets) {
        // Read using ReadProcessMemory, since it will handle invalid pointers
        size_t num_read{};
        if (!ReadProcessMemory(this_proc, reinterpret_cast<LPCVOID>(ptr), &ptr, sizeof(ptr),
                               &num_read)
            || num_read != sizeof(ptr)) {
            return 0;
        }

        if (ptr == 0) {
            return 0;
        }
        ptr += offset;
    }

    // Extra read to make sure we can dereference the final offset
    uintptr_t dummy{};
    size_t num_read{};
    if (!ReadProcessMemory(this_proc, reinterpret_cast<LPCVOID>(ptr), &dummy, sizeof(dummy),
                           &num_read)
        || num_read != sizeof(ptr)) {
        return 0;
    }

    if constexpr (std::is_same_v<R, decltype(ptr)>) {
        return ptr;
    } else {
        return reinterpret_cast<R>(ptr);
    }
}

}  // namespace

void init(void) {
    gworld_base_ptr = memory::read_offset(GWORLD_PATTERN.sigscan());

    talos_char_enable_ghost_ptr =
        TALOS_CHAR_ENABLE_GHOST_PATTERN.sigscan<talos_char_enable_ghost_func>();
    talos_char_enable_walk_ptr =
        TALOS_CHAR_ENABLE_WALK_PATTERN.sigscan<talos_char_enable_walk_func>();

    auto show_cheats = SHOW_CHEATS_PATTERN.sigscan();
    if (show_cheats != 0) {
        memory::unlock_range(show_cheats, sizeof(SHOW_CHEATS_INJECTED_CODE));
        memcpy(reinterpret_cast<void*>(show_cheats), &SHOW_CHEATS_INJECTED_CODE[0],
               sizeof(SHOW_CHEATS_INJECTED_CODE));
    }
}

Vec3d* position(void) {
    return safe_dereference<Vec3d*>(
        gworld_base_ptr, {GWORLD_LEVEL_OFFSET, LEVEL_GAMEMODE_OFFSET, GAMEMODE_PAWN_OFFSET,
                          PAWN_COLLISION_COMP_OFFSET, CYLINDER_COMP_POS_OFFSET});
}

// These two are single byte values which appear to be a standard 0 = off, non-zero = on
// We'll just pretend they're bools to simplify the code interacting with these
static_assert(sizeof(bool) == 1);

bool* god(void) {
    return safe_dereference<bool*>(gworld_base_ptr, {GWORLD_LEVEL_OFFSET, LEVEL_GAMEMODE_OFFSET,
                                                     GAMEMODE_PAWN_OFFSET, PAWN_GOD_OFFSET});
}

bool* turbo(void) {
    return safe_dereference<bool*>(
        gworld_base_ptr, {GWORLD_LEVEL_OFFSET, LEVEL_GAMEMODE_OFFSET, GAMEMODE_PAWN_OFFSET,
                          PAWN_MOVE_COMP_OFFSET, MOVE_COMP_TURBO_OFFSET});
}

void enable_ghost(void) {
    if (talos_char_enable_ghost_ptr == nullptr) {
        return;
    }

    auto pawn = safe_dereference<UTalosCharacter*>(
        gworld_base_ptr, {GWORLD_LEVEL_OFFSET, LEVEL_GAMEMODE_OFFSET, GAMEMODE_PAWN_OFFSET, 0});
    if (pawn == nullptr) {
        return;
    }

    talos_char_enable_ghost_ptr(pawn);
}

void disable_ghost(void) {
    if (talos_char_enable_walk_ptr == nullptr) {
        return;
    }

    auto pawn = safe_dereference<UTalosCharacter*>(
        gworld_base_ptr, {GWORLD_LEVEL_OFFSET, LEVEL_GAMEMODE_OFFSET, GAMEMODE_PAWN_OFFSET, 0});
    if (pawn == nullptr) {
        return;
    }

    talos_char_enable_walk_ptr(pawn);
}

}  // namespace t2c::pointers
