#pragma once

#include "impl.h"

namespace reach {
	void invoke(std::shared_ptr<c_context>);

	inline auto m_enabled = false;
	inline auto m_reach = 0.5f;
}