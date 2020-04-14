#pragma once

#include "../../sdk/includes.h"
class c_player;
class c_world;

class c_context {
public:
	std::shared_ptr<c_player> local;
	std::shared_ptr<c_world> world;
	bool hovering;
	bool ingame;

	c_context(std::shared_ptr<c_player> plr, std::shared_ptr<c_world> wld, bool ingam, bool hoverig)
	{
		local = plr;
		world = wld;
		ingame = ingam;
		hovering = hoverig;
	}
};

namespace cheats {

	class c_cheats {
	private:
		std::vector<std::function<void(std::shared_ptr<c_context>)>> _functions;
	public:

		~c_cheats();

		void register_function(std::function<void(std::shared_ptr<c_context>)> fn);
		void invoke(std::shared_ptr<c_context> ctx);

	};
	inline std::unique_ptr<c_cheats> instance = nullptr;
}