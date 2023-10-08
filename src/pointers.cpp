#include "pch.h"
#include "pointers.h"
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <cstddef>
#include "memory.h"

namespace t2c::pointers {

namespace {

const constinit memory::Pattern<23> INPUT_COMP_PATTERN{
    "48 8D 15 ????????"  // lea rdx, [Talos2-Win64-Shipping.exe+8750CF8]
    "48 8B CB"           // mov rcx, rbx
    "FF 90 ????????"     // call qword ptr [rax+00000E18]
    "48 8B 8B ????????"  // mov rcx, [rbx+00000430]
    ,
    3};
uintptr_t input_comp_base_ptr{};

const constexpr auto INPUT_COMP_BASE_PTR_OFFSET = 0;
const constexpr auto INPUT_COMP_OUTER_OFFSET = 0x20;

const constexpr auto PAWN_COLLISION_COMP_OFFSET = 0x1C0;
const constexpr auto CYLINDER_COMP_POS_OFFSET = 0x260;

const constinit memory::Pattern<16> TALOS_CHAR_ENABLE_GHOST_PATTERN{
    "48 83 EC 28"        // sub rsp, 28
    "48 89 74 24 ??"     // mov [rsp+40], rsi
    "48 8B B1 ????????"  // mov rsi, [rcx+00000350]
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

    if constexpr (std::is_same_v<R, decltype(ptr)>) {
        return ptr;
    } else {
        return reinterpret_cast<R>(ptr);
    }
}

}  // namespace

void init(void) {
    input_comp_base_ptr = memory::read_offset(INPUT_COMP_PATTERN.sigscan());

    talos_char_enable_ghost_ptr =
        TALOS_CHAR_ENABLE_GHOST_PATTERN.sigscan<talos_char_enable_ghost_func>();
    talos_char_enable_walk_ptr =
        TALOS_CHAR_ENABLE_WALK_PATTERN.sigscan<talos_char_enable_walk_func>();
}

Vec3d* position(void) {
    return safe_dereference<Vec3d*>(input_comp_base_ptr,
                                    {INPUT_COMP_BASE_PTR_OFFSET, INPUT_COMP_OUTER_OFFSET,
                                     PAWN_COLLISION_COMP_OFFSET, CYLINDER_COMP_POS_OFFSET});
}

void enable_ghost(void) {
    if (talos_char_enable_ghost_ptr == 0) {
        return;
    }

    auto pawn = safe_dereference<UTalosCharacter*>(
        input_comp_base_ptr, {INPUT_COMP_BASE_PTR_OFFSET, INPUT_COMP_OUTER_OFFSET, 0});
    if (pawn == 0) {
        return;
    }

    talos_char_enable_ghost_ptr(pawn);
}

void disable_ghost(void) {
    if (talos_char_enable_walk_ptr == 0) {
        return;
    }

    auto pawn = safe_dereference<UTalosCharacter*>(
        input_comp_base_ptr, {INPUT_COMP_BASE_PTR_OFFSET, INPUT_COMP_OUTER_OFFSET, 0});
    if (pawn == 0) {
        return;
    }

    talos_char_enable_walk_ptr(pawn);
}

}  // namespace t2c::pointers
