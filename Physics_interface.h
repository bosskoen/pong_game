#pragma once

#include "Component.h"
namespace Core {
	enum class Priority {
		Low,
		Movement,
		Gravity,
		Friction,
		High,
	};

	class IPhysics : public virtual Component
	{
	protected:

	public:
		virtual void Update() = 0;
		virtual void Start() = 0;

		virtual Priority getPriority() const = 0;
		virtual ~IPhysics() = default;
	};
}