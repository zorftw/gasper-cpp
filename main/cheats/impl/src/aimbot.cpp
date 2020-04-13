#include "../headers/aimbot.h"

void aimbot::invoke(std::shared_ptr<c_context> ctx)
{
	/// Keep a timer
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	/// Returns true when the local player object and the world object aren't null
	const auto is_sane = [&]() {
		return (ctx->local->get_object() && ctx->world->get_object());
	};

	/// Get closest player to local player (distance wise)
	const auto get_closest_player = [&]() {
		/// If you still use some random number you're stupid
		double dist = (std::numeric_limits<double>::max)();
		/// Target :O
		std::shared_ptr<c_player> target = nullptr;

		/// Iterate over all player objects in the world
		/// note: c_world#get_players actually iterates the playerlist so if you want to iterate the entity list instead
		/// you should actually change the field
		for (const auto& player : ctx->world->get_players())
		{
			/// Compare the object to our local player to make sure we aren't going to be aiming at ourselves
			if (gasper::instance->get_env()->IsSameObject(ctx->local->get_object(), player->get_object()))
				continue;

			/// Not ded?
			if (!player->get_health() > 0)
				continue;

			/// Basic stuff
			if (ctx->local->get_distance_to(player) <= dist)
			{
				dist = ctx->local->get_distance_to(player);
				target = player;
			}
		}

		return target;
	};

	if (!is_sane() || !aimbot::m_enabled)
		return;

	auto target = get_closest_player();

	if (target && timer->has_passed(30) )
	{
		auto angles = sdk::util::get_angles(ctx->local->get_position(), target->get_position());

		auto difference = sdk::util::wrap_to_180(-(ctx->local->get_yaw() - angles.first));

		/// difference might be negative so yknow
		if (abs(difference) <= 30) {
			auto current_yaw = ctx->local->get_yaw();
			current_yaw += (difference / 20);

			ctx->local->set_yaw(current_yaw);
		}

		timer->reset();
	}
}