#pragma once

#include "../includes.h"

namespace sdk {
	class c_minecraft {
	private:
	public:
	
		jobject get_minecraft();

		jobject get_player(jobject);

		jobject get_world(jobject mc);

		jobject get_current_screen(jobject mc);

		jobject get_entity_over(jobject mc);

	};

	extern std::unique_ptr<c_minecraft> instance;
}