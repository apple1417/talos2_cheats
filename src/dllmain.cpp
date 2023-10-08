#include "pch.h"

#include "gui/gui.h"
#include "pointers.h"

namespace {

/**
 * @brief Main startup thread.
 * @note Instance of `LPTHREAD_START_ROUTINE`.
 *
 * @return unused.
 */
DWORD WINAPI startup_thread(LPVOID /*unused*/) {
    auto mh_ret = MH_Initialize();
    if (mh_ret != MH_OK) {
        std::cerr << "[t2c] Minhook initialization failed: " << mh_ret << "\n";
        return 0;
    }

    try {
        t2c::pointers::init();
        t2c::gui::init();
    } catch (std::exception& ex) {
        std::cerr << "[t2c] Exception occured during initalization: " << ex.what() << "\n";
    }

    std::cout << "[t2c] Talos 2 Cheats loaded\n";

    return 1;
}

}  // namespace

/**
 * @brief Main entry point.
 *
 * @param h_module Handle to module for this dll.
 * @param ul_reason_for_call Reason this is being called.
 * @return True if loaded successfully, false otherwise.
 */
// NOLINTNEXTLINE(readability-identifier-naming)  - for `DllMain`
BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID /*unused*/) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(h_module);
            CreateThread(nullptr, 0, &startup_thread, nullptr, 0, nullptr);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
