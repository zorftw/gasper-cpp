#include "player.h"

#include "../../../main/gasper.h"

c_player::c_player(jobject obj)
{
	player_obj = obj;
}

c_player::~c_player()
{
	gasper::instance->get_env()->DeleteLocalRef(player_obj);
}

float c_player::get_health()
{
	jmethodID get_health_mid = gasper::instance->get_env()->GetMethodID(gasper::instance->get_env()->GetObjectClass(player_obj), xorstr_("func_110143_aJ"), xorstr_("()F"));

	if (!get_health_mid)
		return 1337.f;

	return gasper::instance->get_env()->CallFloatMethod(player_obj, get_health_mid);
}

sdk::vec3d c_player::get_position()
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);
	jfieldID pos_x_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70165_t"), xorstr_("D"));
	jfieldID pos_y_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70163_u"), xorstr_("D"));
	jfieldID pos_z_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70161_v"), xorstr_("D"));

	return sdk::vec3d{
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_x_fid),
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_y_fid),
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_z_fid)
	};
}