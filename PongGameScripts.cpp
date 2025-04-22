#include "PongGameScripts.hpp"

namespace Scripts {
	float PongController::ball_speed = 300;
	float constexpr BallInfo::max_ball_speed = 875;
	float PlayerPadleControler::default_speed = 400;

	float PongController::min_time_to_next_spawn = 10.0f;
	float PongController::max_time_to_next_spawn = 20.0f;

	int constexpr PongController::powerup_size = 48;
	vec2 constexpr PongController::powerup_spawn_range = { 600, 560 };

	constexpr float PowerTracker::speed_up_factor = 1.7f;
	constexpr float PowerTracker::speed_up_time = 7.5f;

	constexpr float PowerTracker::size_up_factor = 1.5f;
	constexpr float PowerTracker::size_up_time = 9.0f;

}