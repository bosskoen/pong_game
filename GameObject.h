#pragma once
#include <string>
#include "renderer.h"
#include <vector>
#include "Script.ixx"
#include "Physics.h"
#include "Collider.h"
#include "Option.hpp"

namespace Core{

	enum Label {
		nothing,

	};

	class GameObject
	{
	private:
		std::vector<IRenderer*> renderers;
		std::vector<IScript*> scripts;
		std::vector<IPhysics*> physics;
		std::vector<IAABB*> colliders;

		void StopSctipts();

		void updatePhysics();
		void updateScripts();
		void updateTriggers();

		void _Direct_AddScript(IScript& script);
		void _Direct_AddRenderer(IRenderer& renderer);
		void _Direct_AddPhysics(IPhysics& physics);
		void _Direct_AddCollider(IAABB& collider);

		void _removeRenderer(IRenderer& renderer);
		void _removeScript(IScript& script);
		void _removePhysics(IPhysics& physics);
		void _removeCollider(IAABB& collider);

		void StartColliders();
		void StartPhysics();
		void StartScripts();

		bool is_active{ true }; // skip update if false
	public:
		bool markedForDelete{ false };
		bool isInManiger{ false };

		CollidedState ColStat {};
		std::string name{ "Name" };
		Label label{ Label::nothing };
		Tmpl8::vec2 velocity{};
		Tmpl8::vec2 pos{};

		GameObject() = default;
		GameObject(const std::string& a_name, const Tmpl8::vec2 a_pos, bool a_is_active = true): name(a_name), pos(a_pos), is_active(a_is_active) {}
		GameObject(const Tmpl8::vec2 a_pos): pos(a_pos) {}
		GameObject(GameObject&& other) noexcept;
		GameObject(const GameObject& other);
		~GameObject();

		void activate();
		void deactivate();
		bool isActive() const { return is_active; }

		//const std::vector<IScript*>& getScripts() { return scripts; }

		

		/// <summary>
		/// for initialization, add a component to the game-object but does not start it
		/// </summary>
		void initializeWithComponent(Component& component);
		/// <summary>
		/// for constructor, add a component to the game-object and start it
		/// </summary>
		void injectComponent(Component& component);
		/// <summary>
		/// for normal use, add the component to the queue to be started next frame
		/// </summary>
		void queueComponent(Component& component);

		/// <summary>
		/// for destructor, removes a component and stop it emediatly
		/// </summary>
		void removeComponent(Component& component);
		/// <summary>
		/// for normal use, add the component to the queue to be removed next frame
		/// </summary>
		void queueComponentForRemoval(Component& component);
	




		template<typename T>
		Util::Option<T&> getCollider(const std::string& cond); // get componetn?
		template<typename T>
		Util::Option<T&> getScript();
		template<typename T>
		Util::Option<T&> getRenderer();


		//bool operator == (const std::string& compare_name);
		//bool operator == (const Label& compare_label);
		bool operator == (const GameObject& compare) {return this == &compare;};
		bool operator != (const GameObject& compare) { return this != &compare; };

		friend class ObjectManager;
	};
	

	template<typename T>
	inline Util::Option<T&> GameObject::getCollider(const std::string& cond)
	{
		for (IAABB* collider : colliders) {
			if (collider->id == cond) {
				T* casted = dynamic_cast<T*>(collider);
				if(casted) {
					return Util::Option<T&>(*casted);
				}
			}
		}
		return Util::Option<T&>::None();
	}

	template<typename T>
	inline Util::Option<T&> GameObject::getScript()
	{
		for (IScript* script : scripts) {
			T* casted = dynamic_cast<T*>(script);
			if (casted) {
				return Util::Option<T&>(*casted);
			}
		}
		return Util::Option<T&>::None();
	}

	template<typename T>
	inline Util::Option<T&> GameObject::getRenderer()
	{
		for (IRenderer* script : renderers) {
			T* casted = dynamic_cast<T*>(script);
			if (casted) {
				return Util::Option<T&>(*casted);
			}
		}
		return Util::Option<T&>::None();
	}

}

//TODO order of operations in vectors

/*
scripts:
	animation
	map handling
	object context

	movment??
physics:
	gravatie
	friction

	bonce physics

colliders:
	coisions
	triggers:: do the triggers events run on the same (teleport is my big question)
renderers:
	just draw it

how to hadel diferens between pos udate movment and velocity update movment

game object:
	pos
	name
	label
	velocitie
	[on ground/wall bool]

	sctipts {custom, context, movment, animation}
	physics { gravatie, friction}
	coliders { collid, trigger } [ do i need to add difrent behaviors for vel/pos based movment ]
								{ difrent collistion behaviors coud be set via a fuction ( a bit as a rust closier) that you give with the constructor } or stic with oop and make difernet classes for diferent behaviors
	renders

update order:
	scripts:
		just call update on them { is custom behaviors }
	physics:
		update velocitie:
	ongrount false
	integrade velocitie
	colision:
		{ colission update:
		iterate over all coliders
		if it is stationary skip
		iterate over all other colider
		skip self colision
		can colide thruw bit mask
		does current and other colide if valse skip
		if current is triger:
			if other is triger:
				call event on both trigger
			if other is colider:
				call event on trigger
		if current is colider:
			if other is trigger:
				call trigger event
			if other is colider:
				if other is moving:
					moving colision(current other)
				if other is stationary:
					stationary colision(current other)
					}
		stationary colision(current { moving }, other { stationary} ):
			other.custom_collide_behavior(current) { could be rigedbody or a trampoline }
			set on groud bool

		moving collision(current, other)
			if(current is dominat){
			current.custom_collide_behavior(other)
			} else{
			other.custom_collide_behavior(current)
			}
	render new frame
*/