#include "ball.h"

void Scripts::PongPadleCollision(Core::IAABB& other, Core::IAABB& static_collider, Core::ColDir direction, float displasment)
	{
		bool canbounceback = true;
		auto info = other.gameobject->getScript<Scripts::BallInfo>();
		if (info) {
			auto& x = info.unwrap();

			canbounceback = x.last_hit != static_collider.gameobject; // if the ball hits the same object twice it won't do collision again
			x.last_hit = static_collider.gameobject;
		}
		if (canbounceback) {
			vec2 temp;
			float delta = other.gameobject->pos.y + other.ofset.y - (static_collider.gameobject->pos.y + static_collider.ofset.y);;
			switch (direction)
			{
			case Core::ColDir::UP:
				other.gameobject->pos.y -= displasment;
				{
					temp = { copysignf(1, other.gameobject->velocity.x) * -1, copysignf(0.99f, delta) };
				}
				break;
			case Core::ColDir::DOWN:
				other.gameobject->pos.y += displasment;
				{
					temp = { copysignf(1, other.gameobject->velocity.x) * -1, copysignf(0.99f, delta) };
				}
				break;
			case Core::ColDir::LEFT:
				other.gameobject->pos.x -= displasment;
				{
					float half_pedle_hight = ((static_collider.size.y * 0.95f) / 2.0f);  // the 0.95 is for steeper bounces 
					temp = { -1,delta / half_pedle_hight };
				}
				break;
			case Core::ColDir::RIGHT:
				other.gameobject->pos.x += displasment;
				{
					float half_pedle_hight = ((static_collider.size.y * 0.95f) / 2.0f);  // the 0.95 is for steeper bounces 
					temp = { 1,delta / half_pedle_hight };
				}
				break;

			}
			temp.normalize();
			temp *= std::min(other.gameobject->velocity.length() * 1.05f, Scripts::BallInfo::max_ball_speed); // ball speed increases with 5% every touch
			other.gameobject->velocity = temp;
		}
	}
