#include "pch.h"
#include "cheats.h"

#include "pointers.h"

namespace t2c::cheats {

namespace {

// We can't easily query the actual ghost state, so track it ourselves
bool think_ghost_enabled = false;

}  // namespace

struct CheatBinds enabled_binds = {true, true, true, false, false};

void set_ghost(void) {
    think_ghost_enabled = true;
    pointers::enable_ghost();
}

void set_walk(void) {
    think_ghost_enabled = false;
    pointers::disable_ghost();
}

void toggle_ghost(void) {
    think_ghost_enabled = !think_ghost_enabled;

    if (think_ghost_enabled) {
        pointers::enable_ghost();
    } else {
        pointers::disable_ghost();
    }
}

void toggle_turbo(void) {
    auto turbo = pointers::turbo();
    if (turbo == nullptr) {
        return;
    }
    *turbo = !*turbo;
}

void toggle_god(void) {
    auto god = pointers::god();
    if (god == nullptr) {
        return;
    }
    *god = !*god;
}

namespace {

std::optional<pointers::Vec3d> saved_pos = std::nullopt;

}

void save_pos(void) {
    auto pos = pointers::position();
    if (pos == nullptr) {
        return;
    }
    saved_pos = *pos;
}

void load_pos(void) {
    if (saved_pos == std::nullopt) {
        return;
    }

    auto pos = pointers::position();
    if (pos == nullptr) {
        return;
    }
    *pos = *saved_pos;
}

bool has_saved_pos(void) {
    return (bool)saved_pos;
}

}  // namespace t2c::cheats
