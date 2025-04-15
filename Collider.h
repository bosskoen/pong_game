#pragma once
#include "template.h"
#include "Event.hpp"
#include "MACRO.hpp"
#include "surface.h"
#include "Component.h"

using namespace Tmpl8;

namespace Core{
	class GameObject;
	class IAABB : public virtual Component
	{
	public:

		std::string id{ "" };
		bool staticCollider{ true }, isTrigger{ false };
		vec2 ofset{};
		vec2 size{};

		virtual void Update() {};
		virtual void doCollision(IAABB& other, float overlap_x = 0, float overlap_y = 0) = 0;
		void setId(std::string a_id) { id = a_id; }

#ifdef _DEBUG
		void draw_outline(Tmpl8::Surface& screen)  ;
#endif // DEBUG


		virtual ~IAABB() = default;
	}; // TODO collision mask (and disabeled);

	class Trigger : public virtual IAABB{
	private:
		
		enum State { ENTER, EXIT, STAY , FROREMOVAL};
		std::unordered_map<GameObject*,State> colided;

		void EventTriggerEnter(GameObject* other);
		void EventTriggerExit(GameObject* other);
		void EventTriggerStay(GameObject* other);
		
	public:
		Trigger() { isTrigger = true; }
		Trigger(vec2 a_size, vec2 a_ofset);
		Trigger(vec2 a_size, vec2 a_ofset, std::string a_id);


		Event<GameObject&> onTriggerEnter;
		Event<GameObject&> onTriggerExit;
		Event<GameObject&> onTriggerStay;

		/// <summary>
		/// call one's a frame to handle OnExit and OnStay
		/// </summary>
		void Update() override;
		void doCollision(IAABB& other, float overlap_x =0 , float overlap_y= 0) override; 

		void RefireTriggerEnter();
		void RefireTriggerExit();
		void RefireTriggerStay();
		
	};

	class Collider;

	/// a higher int value is a hight priority, so the one that is the furdes down has the highest priority
	enum CollidedState {
		None,
		Air,
		Wall,
		Grounded
	};

	namespace ColFunc {
		/// <summary>
		///		 UP
		///		 ___
		/// LEFT|	| RIGHT
		///		|___|
		///		 DOWN
		/// </summary>
		enum ColDir {
			NONE,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};
		void DefaultCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment);

		/// <summary>
		/// perfect elastic collisions.
		/// </summary>
		void PerfecBounceCollision(IAABB& other,IAABB& static_collider ,ColDir direction, float displasment);
		void PongPadleCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment);
	}

	class Collider : public virtual IAABB {
	private:

		void (*behaviour)(IAABB&, IAABB& ,ColFunc::ColDir , float) { &ColFunc::DefaultCollision };
	public:

		Collider() = default;
		Collider(void(*a_behaviour)(IAABB&, IAABB&, ColFunc::ColDir, float), bool isStatic = true);
		Collider(vec2 a_size, vec2 a_ofset, bool isStatic = true);
		Collider(vec2 a_size, vec2 a_ofset, void(*a_behaviour)(IAABB&, IAABB&, ColFunc::ColDir, float), bool isStatic = true);

		void setColliderBehaviour(void(*a_behaviour)(IAABB&, IAABB&, ColFunc::ColDir , float));
		void setStatic(bool isStatic = true);

		void doCollision(IAABB& other, float overlap_x, float overlap_y) override;
	};

}

