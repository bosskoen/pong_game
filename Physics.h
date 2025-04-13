#pragma once
#include "Globals.h"

#include "Component.h"

namespace Core {
	class GameObject;

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
	};	//TODO set order of operation

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
		Util::Key controle_button{ Util::Space };
	public:
		FlappyController() = default;
		FlappyController(Util::Key button) : controle_button(button) {};
		void Update() override;
		Priority getPriority() const override { return Priority::Movement; }
	};
}
