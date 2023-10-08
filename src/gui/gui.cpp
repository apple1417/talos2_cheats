#include "pch.h"

#include "gui/gui.h"
#include "gui/hook.h"
#include "imgui.h"
#include "pointers.h"

namespace t2c::gui {

namespace {

bool showing_window = true;

/**
 * @brief Shows the widgets which interact with player position.
 */
void show_position_widgets(void) {
    // Dummy we use when we don't have a valid pointer
    static pointers::Vec3d dummy_pos{};

    ImGui::SeparatorText("Position");

    auto pos = pointers::position();
    bool pos_disabled = pos == nullptr;

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

    static pointers::Vec3d saved_pos{0, 0, 0};
    static bool any_saved = false;

    if (ImGui::Button("Save")) {
        any_saved = true;
        saved_pos = *pos;
    }
    ImGui::SameLine();
    if (ImGui::Button("Load") && any_saved) {
        *pos = saved_pos;
    }
}

}  // namespace

void render(void) {
    ImGui::ShowDemoWindow();

    ImGui::Begin("apple's Talos 2 Cheats (Ctrl+Shift+Ins)", &showing_window,
                 ImGuiWindowFlags_NoCollapse);

    show_position_widgets();

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
