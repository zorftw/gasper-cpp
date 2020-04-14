#include "..\headers\strafe.h"

/// Get directional float
float get_direction(std::shared_ptr<c_context> ctx);

/// Get calculated speed
float get_speed(std::shared_ptr<c_context> ctx);

/// Set speed
void set_speed(std::shared_ptr<c_context> ctx, const float speed);

/// Fun fact: I had originally written this feature in some old client me and Koupah were making (but eventually dropped)
/// later he adapted that feature and made it a public module and since then we've seen it pretty much in ever cheat since (lulw)
/// so just wanted to tell you I made this shit as can be seen in my commit log of the git we kept this feature has been present in the git
/// since June 2018 but I had implemented it in other cheats as well so let's say begin 2018? 
/// epic right?
void strafe::invoke(std::shared_ptr<c_context> ctx)
{
	/// Keep a timer
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	/// Returns true when the local player object and the world object aren't null
	const auto is_sane = [&]() {
		return (ctx->local->get_object() && ctx->world->get_object());
	};

	if (!strafe::m_enabled || !is_sane())
		return;

	/// Here we go, 20 times a second again (doesn't necessarily have to be synced as well)
	if (!timer->has_passed(50))
		return;

	/// If we're not being hit
	/// We're on the ground
	/// And we're moving
	if (ctx->local->get_hurt_time() == 0
		&& (ctx->local->get_moveforward() != .0f
			|| ctx->local->get_movestrafe() != .0f))
		/// Set the speed :)
		set_speed(ctx, get_speed(ctx));

	timer->reset();
}

float get_direction(std::shared_ptr<c_context> ctx)
{
	float yaw = ctx->local->get_yaw();
	float move_forward = ctx->local->get_moveforward();
	float move_strafe = ctx->local->get_movestrafe();

	/// See if we're moving in a different direction (backwards)
	if (move_forward < 0.0f)
		yaw += 180.0f;

	float forward;

	/// Do some directional calculations for forwards and backwards
	if (move_forward < 0.0f)
		forward = -.5f;
	else if (move_forward > 0.0f)
		forward = .5f;
	else
		forward = 1.0f;

	/// Then accomodate for strafing as well
	if (move_strafe > 0.0f)
		yaw -= 90.0f * forward;
	else if (move_strafe < 0.0f)
		yaw += 90.0f * forward;

	yaw *= .017453292f;
	return yaw;
}

float get_speed(std::shared_ptr<c_context> ctx)
{
	auto velocity_vector = ctx->local->get_motion_vector();

	return sqrt(velocity_vector.x * velocity_vector.x + velocity_vector.z * velocity_vector.z);
}

void set_speed(std::shared_ptr<c_context> ctx, const float speed)
{
	auto direction = get_direction(ctx);
	auto velocity_vector = ctx->local->get_motion_vector();

	velocity_vector.x = -(sin(direction) * speed);
	velocity_vector.z = cos(direction) * speed;
}
