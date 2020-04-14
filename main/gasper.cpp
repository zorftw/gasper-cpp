#include "gasper.h"

#include "../sdk/minecraft/minecraft.h"
#include "../sdk/minecraft/player/player.h"
#include "../sdk/minecraft/world/world.h"

#include "cheats/cheat.h"

#include "cheats/impl/headers/aimbot.h"
#include "cheats/impl/headers/reach.h"
#include "cheats/impl/headers/strafe.h"


jvmtiEnv* jvmti_env;

/// Attach to the JVM
bool gasper::c_gasper::attach()
{
	auto jvm_dll = wrapper::get_module_handle(xorstr_("jvm.dll"));

	auto created_java_vms = reinterpret_cast<sdk::t_createdvms>(wrapper::get_proc_address(xorstr_("JNI_GetCreatedJavaVMs"), jvm_dll));

	auto ret = created_java_vms(&vm, 1, nullptr);

	if (ret != JNI_OK)
		return false;

	ret = get_vm()->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);

	if (ret != JNI_OK)
		return false;

	get_vm()->GetEnv(reinterpret_cast<void**>(&jvmti_env), JVMTI_VERSION_1_1);

	if (!jvmti_env)
		return false;

	/// Get the launchwrapper instance so that we can use findClass
	get_launchwrapper();

	/// Hook our shizzle
	hook();

	/// Now we can begin business
	b_running = true;

	return true;
}

void gasper::c_gasper::run()
{ 
	/// Spawn an instance of our "SDK"
	sdk::instance = std::make_unique<sdk::c_minecraft>();

	/// Spawn an instance of our "cheats"
	cheats::instance = std::make_unique<cheats::c_cheats>();

	/// Register all our cheats
	cheats::instance->register_function(aimbot::invoke);
	cheats::instance->register_function(reach::invoke);
	cheats::instance->register_function(strafe::invoke);

	//Give it a console for debugging purposes
	AllocConsole();
	//Allow you to close the console without the host process closing too
	SetConsoleCtrlHandler(NULL, true);
	//Assign console in and out to pass to the create console rather than minecraft's
	FILE* fIn;
	FILE* fOut;
	freopen_s(&fIn, "conin$", "r", stdin);
	freopen_s(&fOut, "conout$", "w", stdout);
	freopen_s(&fOut, "conout$", "w", stderr);
	
	/// We better be running
	while (b_running)
	{
		/// "Self-destruct" button
		if (wrapper::get_async_keystate(VK_HOME))
			b_running = false;

		/// Le epic flag for le epic modern C++
		static std::once_flag flag;

		/// Keep references simple so that we can easily dispose of them later (they're all localref btw)
		/// but they last for one cycle of this thread so we shouldn't worry about them being accidentally destroyed suddenly
		auto minecraft_inst = sdk::instance->get_minecraft();

		/// Notify of injection (this is for testing)
		/// Note to self: release mode currently contains pretty much the same debug info as debug config
		/// so if you want to sell this piece of shit you better change that or else you'll get crackalacked in a minute
		std::call_once(flag, [&]() { wrapper::show_message(xorstr_("Injected.")); });

		/// NOTE: the player and world class already dispose of the references themselves
		auto local = std::make_shared<c_player>(sdk::instance->get_player(minecraft_inst));
		auto world = std::make_shared<c_world>(sdk::instance->get_world(minecraft_inst));

		cheats::instance->invoke(std::make_shared<c_context>(local, world, !sdk::instance->get_current_screen(minecraft_inst), false));

		/// Delete the reference
		get_env()->DeleteLocalRef(minecraft_inst);

		/// Let's save the CPU some processing powerz
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

/// This is called when we're supposed to dispose of our mineman hackery
void gasper::c_gasper::dispose()
{
	get_env()->DeleteGlobalRef(classloader_obj);
	get_vm()->DetachCurrentThread();

	ImGui_ImplOpenGL2_Shutdown();
	wglDeleteContext(hooks::gl_context->m_glrenderctx);
	ImGui::DestroyContext();
	ImGui_ImplWin32_Shutdown(); 

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	SetWindowLongPtrA(wrapper::find_window(xorstr_("Minecraft 1.7.10")), GWLP_WNDPROC, (LONG_PTR)hooks::original_wndproc);

	env = nullptr;
	hooks::gl_context = nullptr;
	vm = nullptr;
}

void gasper::c_gasper::get_launchwrapper()
{
		jclass launchwrapper_cls = get_env()->FindClass(xorstr_("net/minecraft/launchwrapper/LaunchClassLoader"));
		jclass launch_cls = get_env()->FindClass(xorstr_("net/minecraft/launchwrapper/Launch"));

		if (wrapper::handle_issue(xorstr_("launchwrapper"), launchwrapper_cls) ||
			wrapper::handle_issue(xorstr_("launch_cls"), launch_cls))
			std::exit(0);

		auto classloader_fid = get_env()->GetStaticFieldID(launch_cls, xorstr_("classLoader"), xorstr_("Lnet/minecraft/launchwrapper/LaunchClassLoader;"));

		findclass_md = get_env()->GetMethodID(launchwrapper_cls, xorstr_("findClass"), xorstr_("(Ljava/lang/String;)Ljava/lang/Class;"));
		classloader_obj = get_env()->NewGlobalRef(get_env()->GetStaticObjectField(launch_cls, classloader_fid));

		get_env()->DeleteLocalRef(launchwrapper_cls);
		get_env()->DeleteLocalRef(launch_cls);
}

void gasper::c_gasper::hook()
{
	MH_Initialize();

	auto swap_buffers_ptr = wrapper::get_proc_address(xorstr_("SwapBuffers"), wrapper::get_module_handle(xorstr_("Gdi32.dll")));

	MH_CreateHook(swap_buffers_ptr, hooks::swap_buffers_hk, reinterpret_cast<void**>(&hooks::oswap_buffers));
	MH_EnableHook(MH_ALL_HOOKS);

	/// Set the WndProc
	hooks::original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(wrapper::find_window	(xorstr_("Minecraft 1.7.10")), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc_hk)));
}


std::unique_ptr<gasper::c_gasper> gasper::instance;