#include "gasper.h"

#include "../sdk/minecraft/minecraft.h"
#include "../sdk/minecraft/player/player.h"
#include "../sdk/minecraft/world/world.h"

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
	
	/// We better be running
	while (b_running)
	{
		/// "Self-destruct" button
		if (wrapper::get_async_keystate(VK_HOME))
			b_running = false;

		/// Keep the time
		static sdk::qword time;

		/// Le epic flag for le epic modern C++
		static std::once_flag flag;

		/// Notify of injection (this is for testing)
		/// Note to self: release mode currently contains pretty much the same debug info as debug config
		/// so if you want to sell this piece of shit you better change that or else you'll get crackalacked in a minute
		std::call_once(flag, [&]() { wrapper::show_message(xorstr_("Injected.")); });

		/// Keep references simple so that we can easily dispose of them later (they're all localref btw)
		/// but they last for one cycle of this thread so we shouldn't worry about them being accidentally destroyed suddenly
		auto minecraft_inst = sdk::instance->get_minecraft();

		/// NOTE: the player and world class already dispose of the references themselves
		auto local = std::make_unique<c_player>(sdk::instance->get_player(minecraft_inst));
		auto world = std::make_unique<c_world>(sdk::instance->get_world(minecraft_inst));

		/// Returns true when the local player object and the world object aren't null
		const auto is_sane = [&]() {
			return (local->get_object() && world->get_object());
		};

		/// Get closest player to local player (distance wise)
		const auto get_closest_player = [&]() {
			/// If you still use some random number you're stupid
			double dist = (std::numeric_limits<double>::max)();
			/// Target :O
			std::shared_ptr<c_player> target = nullptr;

			/// Iterate over all player objects in the world
			/// note: c_world#get_players actually iterates the playerlist so if you want to iterate the entity list instead
			/// you should actually change the field
			for (const auto& player : world->get_players())
			{
				/// Compare the object to our local player to make sure we aren't going to be aiming at ourselves
				if (get_env()->IsSameObject(local->get_object(), player->get_object()))
					continue;

				/// Not ded?
				if (!player->get_health() > 0)
					continue;

				/// Basic stuff
				if (local->get_distance_to(player) <= dist)
				{
					dist = local->get_distance_to(player);
					target = player;
				}
			}

			return target;
		};

		/// Simple lambda function that returns true if time has been passed
		const auto has_passed = [&](float ms) {
			if (GetTickCount64() - time > ms)
			{
				time = GetTickCount64();
				return true;
			}

			return false;
		};

		/// We're ingame	
		if (is_sane())
		{
			auto entity = get_closest_player();

			/// use 30ms, because the game runs on a 20 tick system, but we want to smooth it out, so run it a tad more
			if (entity && has_passed(30) )
			{
				auto angles = sdk::util::get_angles(local->get_position(), entity->get_position());

				auto difference = sdk::util::wrap_to_180(-(local->get_yaw() - angles.first));

				/// difference might be negative so yknow
				if (abs(difference) <= 30) {
					auto current_yaw = local->get_yaw();
					current_yaw += (difference / 20);

					local->set_yaw(current_yaw);
				}
			}
		}

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
	hooks::original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(wrapper::find_window(xorstr_("Minecraft 1.7.10")), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wndproc_hk)));
}


std::unique_ptr<gasper::c_gasper> gasper::instance;