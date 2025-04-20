#pragma once
#include "Event.hpp"

#include "ColBehavior.h"

using namespace Tmpl8;

namespace Core{
	class GameObject;

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

	class Collider : public virtual IAABB {
	private:

		void (*behaviour)(IAABB&, IAABB& ,ColDir , float) { &PreBuild::DefaultCollision };
	public:

		Collider() = default;
		Collider(void(*a_behaviour)(IAABB&, IAABB&, ColDir, float), bool isStatic = true);
		Collider(vec2 a_size, vec2 a_ofset, bool isStatic = true);
		Collider(vec2 a_size, vec2 a_ofset, void(*a_behaviour)(IAABB&, IAABB&, ColDir, float), bool isStatic = true);

		void setColliderBehaviour(void(*a_behaviour)(IAABB&, IAABB&, ColDir , float));
		void setStatic(bool isStatic = true);

		void doCollision(IAABB& other, float overlap_x, float overlap_y) override;
	};

}

