#include "../headers/reach.h"

void reach::invoke(std::shared_ptr<c_context> ctx)
{
	/// Keep a timer
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	/// Returns true when the local player object and the world object aren't null
	const auto is_sane = [&]() {
		return (ctx->local->get_object() && ctx->world->get_object());
	};
	/// Santity check and only run 20 times a second because that's what MC does as well, no need to overkill atm
	if (is_sane() && timer->has_passed(50) && reach::m_enabled)
	{
		auto distance = reach::m_reach;

		auto position = ctx->local->get_position();

		for (const auto& entity : ctx->world->get_players())
		{
			if (gasper::instance->get_env()->IsSameObject(ctx->local->get_object(), entity->get_object()))
				continue;

			if (!entity->get_health() > 0.0f)
				continue;

			/// Position of entity
			auto entity_position = entity->get_position();

			/// Get the hyp
			auto hypothenuse_distance = hypot(position.x - entity_position.x, position.z - entity_position.z);

			/// Check if the distance is greater than the vertical dist
			if (distance > hypothenuse_distance)
				distance -= hypothenuse_distance;

			/// Get angles to player
			auto angles = sdk::util::get_angles(position, entity_position);

			/// Distance between angles
			auto difference = sdk::util::wrap_to_180(-(ctx->local->get_yaw() - angles.first));

			/// Check if player isn't on screen
			if (std::abs(difference) > 180.0f)
				continue;

			/// ye ye
			auto cos = std::cos(sdk::util::deg_to_radiants(angles.first + 90.0f));
			auto sin = std::sin(sdk::util::deg_to_radiants(angles.first + 90.0f));

			/// Move accordingly
			auto x = entity_position.x, z = entity_position.z;
			x -= (cos * distance);
			z -= (sin * distance);

			/// At this point what we've done, we've calculated the position of the entity where he would be if we moved him, considering we can't set his position without
			/// Glitching out, we'll have to move the bounding box instead luckily for us, minecraft does this for us :)
			/// The values we get is entity_width = 0.6f and entity_height = 1.8f
			auto entity_width = 0.6f;
			auto entity_height = 1.8f;

			/// Width from left to middle
			auto bb_width = entity_width / 2.0f;

			/// To see what original is :)
			auto current_boundingbox = entity->get_bounding_box()->get_native_boundingbox();

			/// Set the values for min
			s_axisalignedbb bb{};
			bb.minX = x - bb_width;
			bb.minY = current_boundingbox.minY;
			bb.minZ = z - bb_width;

			/// And max ofcourse
			bb.maxX = x + bb_width;
			bb.maxY = current_boundingbox.maxY;
			bb.maxZ = z + bb_width;

			/// Now set the entity's BB
			entity->get_bounding_box()->set_native_boundingbox(bb);
		}
	}
}