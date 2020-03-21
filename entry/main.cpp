#include "../sdk/includes.h"
#include "../main/gasper.h"

void start_gasper_thread();

/// Note: this DLL is manually mapped, there's literally NO reason to handle any other call than DLL_PROCESS_ATTACH
/// thread calls are also non-existant, so no reason to use DisableThreadLibraryCalls
int __stdcall DllMain(
    HINSTANCE hinstDLL,
    unsigned long     fdwReason,
    void* lpvReserved
)   {

    if (fdwReason != DLL_PROCESS_ATTACH)
        return false;

    /// Not necessary to handle the handle to the thread because, no second call I guess.
    wrapper::spawn_thread(reinterpret_cast<LPTHREAD_START_ROUTINE>(start_gasper_thread));

    return true;
}

void start_gasper_thread() {

    /// "Bro you know when the object is destroyed" true.
    gasper::instance = std::make_unique<gasper::c_gasper>();

    /// Try to attach, if we fail the attach function will be sure to drop an error message
    if (!gasper::instance->attach())
        std::exit(0);

    /// Let's get this started 
    gasper::instance->run();

    /// Wait a second actually
    std::this_thread::sleep_for(std::chrono::seconds(1));

    /// Ok now dispose of our shit
    gasper::instance->dispose();
}