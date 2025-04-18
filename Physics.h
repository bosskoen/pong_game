#pragma once
#include "Globals.h"
#include "Physics_interface.h"

namespace Core {
	class GameObject;

	class Gravity : public virtual IPhysics
	{
	private:

	public:

		void Start() override {};
		void Update() override;
		Priority getPriority() const override { return Priority::Gravity; }
	};

	class PlatformerController : public virtual IPhysics {
	private:

	public:
		void Update() override;
		void Start() override {};
		Priority getPriority() const override { return Priority::Movement; }
	};

	class TopDownController : public virtual IPhysics {
	private:

	public:
		void Update() override;
		void Start() override {};
		Priority getPriority() const override { return Priority::Movement; }
	};

	class FlappyController : public virtual IPhysics {
	private:
		Util::Key controle_button{ Util::Key::Space };
	public:
		FlappyController() = default;
		FlappyController(Util::Key button) : controle_button(button) {};
		void Update() override;
		Priority getPriority() const override { return Priority::Movement; }
	};
}
