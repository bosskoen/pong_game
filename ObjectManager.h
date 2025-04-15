#pragma once
#include "GameObject.h"
#include "Option.hpp"

using namespace std;

namespace Core{
	class ObjectManager
	{
		static vector<GameObject*> game_objects;
		static vector<IRenderer*> render_order;

		// keep separate list for security and insurance, correct order of operation and keep all casting code in one class
		static vector<GameObject*> pendingAddGameObjects;
		static vector<GameObject*> pendingRemoveGameObjects;

		static vector<std::pair<GameObject*, IScript*>> scriptsAdd;
		static vector<IScript*>scriptsRemove;

		static vector<std::pair<GameObject*, IPhysics*>> physicsAdd;
		static vector< IPhysics*> physicsRemove;

		static vector<std::pair<GameObject*, IRenderer*>> renderersAdd;
		static vector< IRenderer*> renderersRemove;

		static vector<std::pair<GameObject*, IAABB*>> collidersAdd;
		static vector< IAABB*> collidersRemove;

		static void collisionCheck(IAABB& collider1, IAABB& collider2);
		static void collisionUpdate(std::vector<GameObject*>& gameobjects);

	public:

		static bool rebuild_render_list, reorder_renderlist;

		/// <summary>
		/// only at the end of the program
		/// </summary>
		static void cleanup();
		static void clear_scene();
		static void ProcessLifecycleQueues();

		/// <summary>
		/// for normal use, adds a gameObject to the queue to be added. and will call start on it
		/// </summary>
		static void queueGameObject(GameObject& new_object);
		/// <summary>
		/// for constructor, adds a gameObject to the maniger.and will call start on it
		/// </summary>
		static void injectGameObject(GameObject& new_object);
		/// <summary>
		/// for destructor, removes a gameObject from the mangier. and will call stop on it
		/// </summary>
		static void removeGameObject(GameObject& existing_object);
		/// <summary>
		/// for normal use, adds a gameObject to the queue to be removed. and will call stop on it
		/// </summary>
		static void queueGameObjectForRemoval(GameObject& existing_object);

		

		/// <summary>
		/// calls update on all Game objects managed
		/// </summary>
		/// <param name="screen"></param>
		static void update(Tmpl8::Surface& screen);
		static void render(Surface& screen);

		static Option<GameObject&> get_gameObject(const std::string& name);
		static vector<GameObject&> get_gameObject_byLabel(const Label& label);

		friend class GameObject;
	};
}
