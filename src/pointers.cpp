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
}

Vec3d* position(void) {
    return safe_dereference<Vec3d*>(input_comp_base_ptr,
                                    {INPUT_COMP_BASE_PTR_OFFSET, INPUT_COMP_OUTER_OFFSET,
                                     PAWN_COLLISION_COMP_OFFSET, CYLINDER_COMP_POS_OFFSET});
}

}  // namespace t2c::pointers
