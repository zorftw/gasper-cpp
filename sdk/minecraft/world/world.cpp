#include "world.h"

#include "../../../main/gasper.h"

c_world::c_world(jobject obj)
{
	world_obj = obj;
}

c_world::~c_world()
{
	gasper::instance->get_env()->DeleteLocalRef(world_obj);
}

std::vector<std::shared_ptr<c_player>> c_world::get_players()
{
	jfieldID player_entities = gasper::instance->get_env()->GetFieldID(gasper::instance->get_env()->GetObjectClass(world_obj), xorstr_("field_73010_i"), xorstr_("Ljava/util/List;"));
	jclass list_cls = gasper::instance->get_env()->FindClass(xorstr_("java/util/List"));
	jmethodID to_array_md = gasper::instance->get_env()->GetMethodID(list_cls, xorstr_("toArray"), xorstr_("()[Ljava/lang/Object;"));

	gasper::instance->get_env()->DeleteLocalRef(list_cls);

	std::vector<std::shared_ptr<c_player>> res;

	jobject obj_player_entities = gasper::instance->get_env()->GetObjectField(world_obj, player_entities);

	if (!obj_player_entities)
		return res;

	jobjectArray array_player_list = reinterpret_cast<jobjectArray>(gasper::instance->get_env()->CallObjectMethod(obj_player_entities, to_array_md));

	if (!array_player_list)
		return res;

	size_t len = gasper::instance->get_env()->GetArrayLength(array_player_list);

	for (int i = 0; i < len; ++i)
	{
		jobject player = gasper::instance->get_env()->GetObjectArrayElement(array_player_list, i);
		res.push_back(std::make_shared<c_player>(player));
	}

	gasper::instance->get_env()->DeleteLocalRef(obj_player_entities);
	gasper::instance->get_env()->DeleteLocalRef(array_player_list);

	return res;
}


