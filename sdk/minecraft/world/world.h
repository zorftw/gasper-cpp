#pragma once

#include "../minecraft.h"
#include "../player/player.h"

class c_world {
private:
	jobject world_obj;
public:
	c_world(jobject obj);
	~c_world();

	const jobject get_object()
	{
		return world_obj;
	}

	std::vector<std::shared_ptr<c_player>> get_players();
};