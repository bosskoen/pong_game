#include "ColBehavior.h"
#include "GameObject.h"

#include "PongGameScripts.hpp"

using Core::CollidedState;

namespace PreBuild {
	void DefaultCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment)
	{
		CollidedState new_state{};
		switch (direction)
		{
		case ColDir::UP:
			other.gameobject->pos.y -= displasment;
			other.gameobject->velocity.y = 0;
			new_state = CollidedState::Grounded;
			break;
		case ColDir::DOWN:
			other.gameobject->pos.y += displasment;
			other.gameobject->velocity.y = 0;
			new_state = CollidedState::None; // lowest prioroity so it wont change the state
			break;
		case ColDir::LEFT:
			other.gameobject->pos.x -= displasment;
			other.gameobject->velocity.x = 0;
			new_state = CollidedState::Wall;
			break;
		case ColDir::RIGHT:
			other.gameobject->pos.x += displasment;
			other.gameobject->velocity.x = 0;
			new_state = CollidedState::Wall;
			break;
		}
		if (new_state > other.gameobject->ColStat) {
			other.gameobject->ColStat = new_state;
		}
	}

	void PerfecBounceCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment)
	{
		switch (direction)
		{
		case ColDir::UP:
			other.gameobject->pos.y -= displasment;
			other.gameobject->velocity.y *= -1;

			break;
		case ColDir::DOWN:
			other.gameobject->pos.y += displasment;
			other.gameobject->velocity.y *= -1;

			break;
		case ColDir::LEFT:
			other.gameobject->pos.x -= displasment;
			other.gameobject->velocity.x *= -1;

			break;
		case ColDir::RIGHT:
			other.gameobject->pos.x += displasment;
			other.gameobject->velocity.x *= -1;
			break;
		}
	}

	void PongPadleCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment)
	{
		bool canbounceback = true;
		auto info = other.gameobject->getScript<Scripts::BallInfo>();
		if (info) {
			auto& x = info.unwrap();

			canbounceback = x.last_hit != static_collider.gameobject; // if the ball hits the same object twice it won't do collision again
			x.last_hit = static_collider.gameobject;
		}
		if (canbounceback) {
			switch (direction)
			{
			case ColDir::UP:
				other.gameobject->pos.y -= displasment;
				other.gameobject->velocity.y = 0;
				break;
			case ColDir::DOWN:
				other.gameobject->pos.y += displasment;
				other.gameobject->velocity.y = 0;
				break;
			case ColDir::LEFT:
				other.gameobject->pos.x -= displasment;
				{
					float delta = other.gameobject->pos.y + other.ofset.y - (static_collider.gameobject->pos.y + static_collider.ofset.y);
					float half_pedle_hight = ((static_collider.size.y * 0.95f) / 2.0f);  // the 0.95 is for steeper bounces 
					vec2 temp = { -1,delta / half_pedle_hight };
					temp.normalize();
					temp *= std::min(other.gameobject->velocity.length() * 1.05f, Scripts::PongController::max_ball_speed); // ball speed increases with 5% every touch
					other.gameobject->velocity = temp;
				}
				break;
			case ColDir::RIGHT:
				other.gameobject->pos.x += displasment;
				{
					float delta = other.gameobject->pos.y + other.ofset.y - (static_collider.gameobject->pos.y + static_collider.ofset.y);
					float half_pedle_hight = ((static_collider.size.y * 0.95f) / 2.0f);  // the 0.95 is for steeper bounces 
					vec2 temp = { 1,delta / half_pedle_hight };
					temp.normalize();
					temp *= std::min(other.gameobject->velocity.length() * 1.05f, Scripts::PongController::max_ball_speed); // ball speed increases with 5% every touch
					other.gameobject->velocity = temp;
				}
				break;

			}
		}
	}
}