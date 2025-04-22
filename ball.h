#pragma once
#include "GameObject.h"
#include "Cloning.h"
#include "Collider_interface.h"
namespace Scripts {
	/// <summary>
	/// small data class that tracks what the last hit is so power ups can be added properly
	/// </summary>
	class BallInfo : public virtual Util::Cloneable<BallInfo, Core::IScript> {
	public:
		static const float max_ball_speed;
		Core::GameObject* last_hit{ nullptr };

		void Start() override {}
		void Stop() override {}
		void Update() override {}
	};

	void PongPadleCollision(Core::IAABB& other, Core::IAABB& static_collider, Core::ColDir direction, float displasment);
}