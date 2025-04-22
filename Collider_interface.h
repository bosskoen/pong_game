#pragma once
#include "Component.h"
#include "template.h"
#include <string>

#ifdef _DEBUG
#include "surface.h"
#endif

using Tmpl8::vec2;

namespace Core {

	/// <summary>
	///		 UP
	///		 ___
	/// LEFT|	| RIGHT
	///		|___|
	///		 DOWN
	/// </summary>
	enum class ColDir {
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	/// a higher int value is a hight priority, so the one that is the furdes down has the highest priority
	enum CollidedState {
		None,
		Air,
		Wall,
		Grounded
	};

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
		void draw_outline(Tmpl8::Surface& screen) const;
#endif // DEBUG


		virtual ~IAABB() = default;
	}; // TODO collision mask (and disabeled);
}