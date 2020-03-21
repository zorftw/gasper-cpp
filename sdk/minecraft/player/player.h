#pragma once

#include "../minecraft.h"

class c_player {
private:
	jobject player_obj;
public:
	c_player(jobject obj);
	~c_player();

	float get_health();
	sdk::vec3d get_position();

	const jobject get_object()
	{
		return player_obj;
	}
};