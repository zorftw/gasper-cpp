#include "minecraft.h"
#include "../../main/gasper.h"

std::unique_ptr<sdk::c_minecraft> sdk::instance;

#pragma warning(disable:4996)

jobject sdk::c_minecraft::get_minecraft()
{
	jobject res;
	if (!BADLION_CLIENT) {
		auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
		auto mc_fid = gasper::instance->get_env()->GetStaticFieldID(cls, xorstr_("field_71432_P"), xorstr_("Lnet/minecraft/client/Minecraft;"));

		res = gasper::instance->get_env()->GetStaticObjectField(cls, mc_fid);

		gasper::instance->get_env()->DeleteLocalRef(cls);

		return res;
	}

	auto cls = gasper::instance->find_class(xorstr_("ave"));
	auto ave = gasper::instance->get_env()->GetStaticFieldID(cls, xorstr_("S"), xorstr_("Lave;"));

	res = gasper::instance->get_env()->GetStaticObjectField(cls, ave);

	gasper::instance->get_env()->DeleteLocalRef(cls);
	

	return res;
}

jobject sdk::c_minecraft::get_player(jobject mc) {
	jobject res;

	if (!BADLION_CLIENT) {
		auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
		auto player_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71439_g"), xorstr_("Lnet/minecraft/client/entity/EntityClientPlayerMP;"));

		res = gasper::instance->get_env()->GetObjectField(mc, player_fid);

		gasper::instance->get_env()->DeleteLocalRef(cls);
		return res;
	}

	auto cls = gasper::instance->find_class(xorstr_("ave"));
	auto player_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("h"), xorstr_("Lbew;"));

	res = gasper::instance->get_env()->GetObjectField(mc, player_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_world(jobject mc) {
	jobject res;

	if (!BADLION_CLIENT) {
		auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
		auto world_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71441_e"), xorstr_("Lnet/minecraft/client/multiplayer/WorldClient;"));

		res = gasper::instance->get_env()->GetObjectField(mc, world_fid);

		gasper::instance->get_env()->DeleteLocalRef(cls);
		return res;
	}

	auto cls = gasper::instance->find_class(xorstr_("ave"));
	auto world_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("f"), xorstr_("Lbdb;"));

	res = gasper::instance->get_env()->GetObjectField(mc, world_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_current_screen(jobject mc) {
	jobject res;

	if (!BADLION_CLIENT) {
		auto cls = gasper::instance->find_class(xorstr_("net.minecraft.client.Minecraft"));
		auto current_screen_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("field_71462_r"), xorstr_("Lnet/minecraft/client/gui/GuiScreen;"));

		res = gasper::instance->get_env()->GetObjectField(mc, current_screen_fid);

		gasper::instance->get_env()->DeleteLocalRef(cls);
		return res;
	}

	auto cls = gasper::instance->find_class(xorstr_("ave"));
	auto current_screen_fid = gasper::instance->get_env()->GetFieldID(cls, xorstr_("m"), xorstr_("Laxu;"));

	res = gasper::instance->get_env()->GetObjectField(mc, current_screen_fid);

	gasper::instance->get_env()->DeleteLocalRef(cls);

	return res;
}

jobject sdk::c_minecraft::get_entity_over(jobject mc) {
	jobject res;

	auto cls = gasper::instance->find_class(!BADLION_CLIENT ? xorstr_("net.minecraft.client.Minecraft") : xorstr_("ave"));

	auto object_mouse_over_fid = gasper::instance->get_env()->GetFieldID(cls, !BADLION_CLIENT ? xorstr_("field_71476_x") : xorstr_("s"), !BADLION_CLIENT ? xorstr_("Lnet/minecraft/util/MovingObjectPosition;") : xorstr_("Lauh;"));

	auto mop_entityhit_cls = gasper::instance->find_class(!BADLION_CLIENT ? xorstr_("net.minecraft.util.MovingObjectPosition") : xorstr_("auh"));

	auto entity_hit_fid = gasper::instance->get_env()->GetFieldID(mop_entityhit_cls, !BADLION_CLIENT ? xorstr_("field_72308_g") : xorstr_("d"), !BADLION_CLIENT ? xorstr_("Lnet/minecraft/entity/Entity;") : xorstr_("Lpk;"));

	gasper::instance->get_env()->DeleteLocalRef(cls);
	gasper::instance->get_env()->DeleteLocalRef(mop_entityhit_cls);

	auto object_mouse_over = gasper::instance->get_env()->GetObjectField(mc, object_mouse_over_fid);
	res = gasper::instance->get_env()->GetObjectField(object_mouse_over, entity_hit_fid);

	gasper::instance->get_env()->DeleteLocalRef(object_mouse_over);

	return res;
}