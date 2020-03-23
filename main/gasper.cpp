#include "gasper.h"

#include "../sdk/minecraft/minecraft.h"
#include "../sdk/minecraft/player/player.h"
#include "../sdk/minecraft/world/world.h"



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

	get_launchwrapper();
	b_running = true;

	return true;
}

void gasper::c_gasper::run()
{ 
	sdk::instance = std::make_unique<sdk::c_minecraft>();
	
	while (b_running)
	{
		if (wrapper::get_async_keystate(VK_HOME))
			b_running = false;

		static sdk::qword time;
		static std::once_flag flag;

		std::call_once(flag, [&]() { wrapper::show_message(xorstr_("Injected.")); });

		auto minecraft_inst = sdk::instance->get_minecraft();
		auto local = std::make_unique<c_player>(sdk::instance->get_player(minecraft_inst));
		auto world = std::make_unique<c_world>(sdk::instance->get_world(minecraft_inst));

		/// Returns true when the local player object and the world object aren't null
		const auto is_sane = [&]() {
			return (local->get_object() && world->get_object());
		};

		/// Get closest player to local player (distance wise)
		const auto get_closest_player = [&]() {
			double dist = (std::numeric_limits<double>::max)();
			std::shared_ptr<c_player> target = nullptr;
			for (const auto& player : world->get_players())
			{
				if (get_env()->IsSameObject(local->get_object(), player->get_object()))
					continue;

				if (!player->get_health() > 0)
					continue;

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

				if (difference <= 30) {
					auto current_yaw = local->get_yaw();
					current_yaw += (difference / 20);

					local->set_yaw(current_yaw);
				}
			}
		}

		get_env()->DeleteLocalRef(minecraft_inst);
	}
}

void gasper::c_gasper::dispose()
{
	get_env()->DeleteGlobalRef(classloader_obj);

	get_vm()->DetachCurrentThread();

	env = nullptr;
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


std::unique_ptr<gasper::c_gasper> gasper::instance;