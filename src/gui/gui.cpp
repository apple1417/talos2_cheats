#include "pch.h"

#include "gui/gui.h"
#include "gui/hook.h"

namespace t2c::gui {

namespace {

bool showing_window = true;

}

void render(void) {
    ImGui::ShowDemoWindow();
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
