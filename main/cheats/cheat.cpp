#include "cheat.h"

#include "../../sdk/minecraft/player/player.h"
#include "../../sdk/minecraft/world/world.h"

cheats::c_cheats::~c_cheats()
{
	_functions.clear();
}

void cheats::c_cheats::register_function(std::function<void(std::shared_ptr<c_context>)> fn)
{
	_functions.push_back(fn);
}

void cheats::c_cheats::invoke(std::shared_ptr<c_context> ctx)
{
	for (auto fn : _functions)
		fn(ctx);
}

