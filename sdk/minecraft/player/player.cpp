#include "player.h"

#include "../../../main/gasper.h"
#include "../axisaligned/axisalignedbb.h"

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
	jmethodID get_health_mid;
	if (!BADLION_CLIENT)
		get_health_mid = gasper::instance->get_env()->GetMethodID(gasper::instance->get_env()->GetObjectClass(player_obj), xorstr_("func_110143_aJ"), xorstr_("()F"));
	else
		get_health_mid = gasper::instance->get_env()->GetMethodID(gasper::instance->get_env()->GetObjectClass(player_obj), xorstr_("bn"), xorstr_("()F"));

	if (!get_health_mid)
		return 1337.f;

	return gasper::instance->get_env()->CallFloatMethod(player_obj, get_health_mid);
}

sdk::vec3d c_player::get_position()
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);

	jfieldID pos_x_fid, pos_y_fid, pos_z_fid;

	if (!BADLION_CLIENT) {
		pos_x_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70165_t"), xorstr_("D"));
		pos_y_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70163_u"), xorstr_("D"));
		pos_z_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70161_v"), xorstr_("D"));
	}
	else {
		pos_x_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("s"), xorstr_("D"));
		pos_y_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("t"), xorstr_("D"));
		pos_z_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("u"), xorstr_("D"));
	}

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	return sdk::vec3d{
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_x_fid),
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_y_fid),
		gasper::instance->get_env()->GetDoubleField(player_obj, pos_z_fid)
	};
}

void c_player::set_yaw(float yaw)
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);

	jfieldID yaw_fid;
	if (!BADLION_CLIENT) {
		yaw_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70177_z"), xorstr_("F"));
	}
	else {
		yaw_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("y"), xorstr_("F"));
	}

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	gasper::instance->get_env()->SetFloatField(player_obj, yaw_fid, yaw);
}

float c_player::get_yaw()
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);

	jfieldID yaw_fid;
	if (!BADLION_CLIENT) {
		yaw_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70177_z"), xorstr_("F"));
	}
	else {
		yaw_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("y"), xorstr_("F"));
	}

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	return gasper::instance->get_env()->GetFloatField(player_obj, yaw_fid);
}

void c_player::set_pitch(float pitch)
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);

	jfieldID pitch_fid;
	if (!BADLION_CLIENT) {
		pitch_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70125_A"), xorstr_("F"));
	}
	else {
		pitch_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("z"), xorstr_("F"));
	}

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	gasper::instance->get_env()->SetFloatField(player_obj, pitch_fid, pitch);
}

float c_player::get_pitch()
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);
	
	jfieldID pitch_fid;
	if (!BADLION_CLIENT) {
		pitch_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70125_A"), xorstr_("F"));
	}
	else {
		pitch_fid = gasper::instance->get_env()->GetFieldID(player_class, xorstr_("z"), xorstr_("F"));
	}

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	return gasper::instance->get_env()->GetFloatField(player_obj, pitch_fid);
}

std::shared_ptr<c_axisalignedbb> c_player::get_bounding_box()
{
	auto player_class = gasper::instance->get_env()->GetObjectClass(player_obj);
	jfieldID boundingbox_fid;

	boundingbox_fid = BADLION_CLIENT ? gasper::instance->get_env()->GetFieldID(player_class, xorstr_("f"), xorstr_("Laug;")) : gasper::instance->get_env()->GetFieldID(player_class, xorstr_("field_70121_D"), xorstr_("Lnet/minecraft/util/AxisAlignedBB;"));

	gasper::instance->get_env()->DeleteLocalRef(player_class);

	return std::make_shared<c_axisalignedbb>(gasper::instance->get_env()->GetObjectField(player_obj, boundingbox_fid));
}

double c_player::get_distance_to(std::shared_ptr<c_player> other)
{
	auto pos = get_position();
	auto entity_pos = other->get_position();
	return sdk::util::distance(pos.x, pos.y, pos.z, entity_pos.x, entity_pos.y, entity_pos.z);
}
