#pragma once
#include "Globals.h"
#include "Physics_interface.h"
#include "Cloning.h"

using Util::Cloneable;

namespace Core {
	class GameObject;

	class Gravity : public virtual Cloneable<Gravity, IPhysics>
	{
	private:

	public:

		void Start() override {};
		void Update() override;
		Priority getPriority() const override { return Priority::Gravity; }
	};

	class PlatformerController : public virtual Cloneable<Gravity, IPhysics> {
	private:

	public:
		void Update() override;
		void Start() override {};
		Priority getPriority() const override { return Priority::Movement; }
	};

	class TopDownController : public virtual Cloneable<TopDownController, IPhysics> {
	private:

	public:
		void Update() override;
		void Start() override {};
		Priority getPriority() const override { return Priority::Movement; }
	};

	class FlappyController : public virtual Cloneable<FlappyController, IPhysics> {
	private:
		Util::Key controle_button{ Util::Key::Space };
	public:
		FlappyController() = default;
		FlappyController(Util::Key button) : controle_button(button) {};
		void Update() override;
		void Start() override {};
		Priority getPriority() const override { return Priority::Movement; }
	};
}
