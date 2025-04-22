#pragma once
#include "renderer_interface.h"
#include <vector>
#include "Script.h"
#include "Physics_interface.h"
#include "Collider_interface.h"
#include "Option.hpp"

namespace Core{

	enum Label {
		nothing,
		Score_Trigger,
	};

	class GameObject
	{
	private:

		// GameObject owns all attached components
		std::vector<IRenderer*> renderers;
		std::vector<IScript*> scripts;
		std::vector<IPhysics*> physics;
		std::vector<IAABB*> colliders;

		void StopSctipts();

		/*
		Update order (per frame):
		1. scripts
		2. physics
		3. trigger collision state list
		4. collision resolution (handled by object manager)
		5. rendering (handled by object manager)
		*/

		void updateScripts();
		void updatePhysics();
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

		CollidedState ColStat {}; // set in collider, check for if on ground

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

		/// <summary>
		/// makes this object enter the update loop
		/// </summary>
		void activate();
		/// <summary>
		/// removes object for the update loop
		/// </summary>
		void deactivate();
		bool isActive() const { return is_active; }		

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
		Util::Option<T&> getCollider(const std::string& cond); 
		template<typename T>
		Util::Option<T&> getScript();
		template<typename T>
		Util::Option<T&> getRenderer();
		//TODO one get component function


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
