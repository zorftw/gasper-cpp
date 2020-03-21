#include "minecraft.h"
#include "../../main/gasper.h"

std::unique_ptr<sdk::c_minecraft> sdk::instance;

jobject sdk::c_minecraft::get_minecraft()
{
	jobject res;
	auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
	auto mc_fid = gasper::instance->get_env()->GetStaticFieldID(cls, xorstr_("field_71432_P"), xorstr_("Lnet/minecraft/client/Minecraft;"));

	res = gasper::instance->get_env()->GetStaticObjectField(cls, mc_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_player(jobject mc) {
	jobject res;

	auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
	auto player_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71439_g"), xorstr_("Lnet/minecraft/client/entity/EntityClientPlayerMP;"));

	res = gasper::instance->get_env()->GetObjectField(mc, player_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_world(jobject mc) {
	jobject res;

	auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
	auto world_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71441_e"), xorstr_("Lnet/minecraft/client/multiplayer/WorldClient;"));

	res = gasper::instance->get_env()->GetObjectField(mc, world_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_current_screen(jobject mc) {
	jobject res;

	auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
	auto current_screen_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71462_r"), xorstr_("Lnet/minecraft/client/gui/GuiScreen;"));

	res = gasper::instance->get_env()->GetObjectField(mc, current_screen_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}