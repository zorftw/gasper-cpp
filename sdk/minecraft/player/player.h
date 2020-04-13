#pragma once

#include "../minecraft.h"

class c_axisalignedbb;

class c_player {
private:
	jobject player_obj;
public:
	c_player(jobject obj);
	~c_player();

	float get_health();
	sdk::vec3d get_position();
	void set_yaw(float yaw);
	float get_yaw();
	void set_pitch(float pitch);
	float get_pitch();
	std::shared_ptr<c_axisalignedbb> get_bounding_box();
	double get_distance_to(std::shared_ptr<c_player>);

	const jobject get_object()
	{
		return player_obj;
	}
};