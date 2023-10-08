#include "pch.h"

#include "cheats.h"
#include "gui/gui.h"
#include "gui/hook.h"
#include "pointers.h"

namespace t2c::gui {

namespace {

bool showing_window = true;

/**
 * @brief Draws the widgets holding player position.
 */
void draw_position_widgets(void) {
    auto pos = pointers::position();
    bool pos_disabled = pos == nullptr;

    pointers::Vec3d dummy_pos{};
    if (pos_disabled) {
        ImGui::BeginDisabled();
        pos = &dummy_pos;
    }

    ImGui::InputDouble("X", &pos->x);
    ImGui::InputDouble("Y", &pos->y);
    ImGui::InputDouble("Z", &pos->z);

    if (pos_disabled) {
        ImGui::EndDisabled();
    }
}

/**
 * @brief Draws the save/load position widgets.
 */
void draw_pos_save_load_pos_widgets(void) {
    if (ImGui::Button("Save")) {
        cheats::save_pos();
    }

    auto load_disabled = !cheats::has_saved_pos();
    if (load_disabled) {
        ImGui::BeginDisabled();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        cheats::load_pos();
    }
    if (load_disabled) {
        ImGui::EndDisabled();
    }
}

/**
 * @brief Draws a checkbox which may be disabled.
 *
 * @param name The checkbox's name.
 * @param ptr The checkbox's value, or nullptr if disabled.
 */
void draw_nullable_checkbox(const char* name, bool* ptr) {
    bool dummy = false;

    auto disabled = ptr == nullptr;

    if (disabled) {
        ImGui::BeginDisabled();
        ptr = &dummy;
    }
    ImGui::Checkbox(name, ptr);

    if (disabled) {
        ImGui::EndDisabled();
    }
}

/**
 * @brief Draws the widgets to enable individual cheats.
 */
void draw_cheats_widgets(void) {
    if (ImGui::Button("Ghost")) {
        cheats::toggle_ghost();
    }

    draw_nullable_checkbox("Turbo", pointers::turbo());
    draw_nullable_checkbox("God", pointers::god());
}

}  // namespace

void render(void) {
    if (!showing_window) {
        return;
    }

    ImGui::Begin("Talos 2 Cheats (Ctrl+Shift+Ins)", &showing_window, ImGuiWindowFlags_NoCollapse);

    ImGui::SeparatorText("Cheats");
    draw_cheats_widgets();
    ImGui::SeparatorText("Position");
    draw_position_widgets();
    draw_pos_save_load_pos_widgets();

    ImGui::End();
}

void init(void) {
    hook();
}

bool is_showing(void) {
    return showing_window;
}

void toggle_showing(void) {
    showing_window = !showing_window;
}

}  // namespace t2c::gui
