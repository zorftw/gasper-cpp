#pragma once

#include "impl.h"

namespace strafe {
	void invoke(std::shared_ptr<c_context> ctx);

	inline bool m_enabled = false;
}