#pragma once

#include "impl.h"

namespace aimbot {
	void invoke(std::shared_ptr<c_context>);

	inline auto m_enabled = false;
	inline auto m_speed = 10.0f;
	inline auto m_speed_body = 2.0f;
	inline auto m_fov = 30.0f;
}