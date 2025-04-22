#include "ColBehavior.h"
#include "GameObject.h"

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
}