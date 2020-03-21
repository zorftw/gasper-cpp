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

		/// We're ingame	
		if (sdk::instance->get_current_screen(minecraft_inst) == nullptr)
		{
			if (GetTickCount64() - time > 5000)
			{
				auto players = world->get_players();

				for (const auto& player : players)
				{
					if (!get_env()->IsSameObject(player->get_object(), local->get_object()))
					{
						wrapper::show_message(std::to_string(player->get_health()));
					}
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