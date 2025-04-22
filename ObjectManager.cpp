#include "ObjectManager.h"
#include "renderer.h"

namespace Core {

	vector<GameObject*> ObjectManager::game_objects;
	vector<IRenderer*> ObjectManager::render_order;

	vector<GameObject*> ObjectManager::pendingAddGameObjects;
	vector<GameObject*> ObjectManager::pendingRemoveGameObjects;
	
	// keep separate list for security and insurance, correct order of operation and keep all casting code in one class
	vector<std::pair<GameObject*, IScript*>> ObjectManager::scriptsAdd;
	vector<IScript*> ObjectManager::scriptsRemove;

	vector<std::pair<GameObject*, IPhysics*>> ObjectManager::physicsAdd;
	vector<IPhysics*> ObjectManager::physicsRemove;

	vector<std::pair<GameObject*, IRenderer*>> ObjectManager::renderersAdd;
	vector<IRenderer*> ObjectManager::renderersRemove;

	vector<std::pair<GameObject*, IAABB*>> ObjectManager::collidersAdd;
	vector<IAABB*> ObjectManager::collidersRemove;


	bool ObjectManager::rebuild_render_list = false, ObjectManager::reorder_renderlist = false;

	void ObjectManager::cleanup()
	{
		for (GameObject* object : game_objects) {
			if(object->renderers.size() > 0) {
				rebuild_render_list = true;
			}
			object->isInManiger = false;
			delete object;
		}
		// All dynamically allocated components and objects in add queues have been deleted above
		for (GameObject* object : pendingAddGameObjects) {
			delete object;
		}
		pendingAddGameObjects.clear();
		for (auto& pair : scriptsAdd) {
			delete pair.second;
		}
		scriptsAdd.clear();
		for (auto& pair : physicsAdd) {
			delete pair.second;
		}
		physicsAdd.clear();
		for (auto& pair : renderersAdd) {
			delete pair.second;
		}
		renderersAdd.clear();
		for (auto& pair : collidersAdd) {
			delete pair.second;
		}
		collidersAdd.clear();
		game_objects.clear();
	}

	void ObjectManager::ProcessLifecycleQueues()
	{
		// --- remove elements --- 
		for (IScript* script : scriptsRemove) {
			script->gameobject->_removeScript(*script);
		}
		scriptsRemove.clear();
		for (IPhysics* script : physicsRemove) {
			script->gameobject->_removePhysics(*script);
		}
		physicsRemove.clear();
		for (IRenderer* script : renderersRemove) {
			script->gameobject->_removeRenderer(*script);
		}
		renderersRemove.clear();
		for (IAABB* script : collidersRemove) {
			script->gameobject->_removeCollider(*script);
		}
		collidersRemove.clear();

		// --- remove gameobjects --- 
		for (GameObject* object : pendingRemoveGameObjects) {
			auto it = std::find(game_objects.begin(), game_objects.end(), object);
			if (it != game_objects.end()) {
				if (object->renderers.size() > 0) {
					rebuild_render_list = true;
				}
				object->isInManiger = false;
				game_objects.erase(it); 
				delete object;           
			}
		}
		pendingRemoveGameObjects.clear();


		// --- add gameobjects --- 
		for (GameObject* object : pendingAddGameObjects) {
			game_objects.push_back(object);
			object->isInManiger = true;
		}
		// --- add elements --- 
		for (auto& pair : scriptsAdd) {
			pair.first->_Direct_AddScript(*pair.second);
		}
		
		for (auto& pair : physicsAdd) {
			pair.first->_Direct_AddPhysics(*pair.second);
		}
		
		for (auto& pair : renderersAdd) {
			pair.first->_Direct_AddRenderer(*pair.second);
		}
		
		for (auto& pair : collidersAdd) {
			pair.first->_Direct_AddCollider(*pair.second);
		}
		
		// --- start scripts --- 
		for (GameObject* object : pendingAddGameObjects) {
			object->StartColliders();
			object->StartPhysics();
			object->StartScripts();
			if (object->renderers.size() > 0) {
				rebuild_render_list = true;
			}
		}
		for (auto& pair : scriptsAdd) {
			pair.second->Start();
		}
		for (auto& pair : physicsAdd) {
			pair.second->Start();
		}

		pendingAddGameObjects.clear();
		scriptsAdd.clear();
		physicsAdd.clear();
		renderersAdd.clear();
		collidersAdd.clear();
	}

	void ObjectManager::render(Surface& screen)
	{
		if (rebuild_render_list) {
			render_order.clear();
			render_order.reserve(game_objects.size() * 2); // could be betters ied no constens 
			for (GameObject* object : game_objects) {
				for (IRenderer* render : object->renderers) {
					render_order.push_back(render);
				}
			}
			std::stable_sort(render_order.begin(), render_order.end(),
				[](const IRenderer* a, const IRenderer* b) -> bool 
				{ return a->get_layer() < b->get_layer(); });
			rebuild_render_list = false;
			reorder_renderlist = false;
		}
		else if (reorder_renderlist) {
			std::stable_sort(render_order.begin(), render_order.end(),
				[](const IRenderer* a, const IRenderer* b) -> bool
				{ return a->get_layer() < b->get_layer(); });
			reorder_renderlist = false;
		}
		for (IRenderer* renderer : render_order) {
			if (renderer->isActive()) renderer->draw(screen);
		}
	}

	void ObjectManager::injectGameObject(GameObject& new_object)
	{
		game_objects.push_back(&new_object);
		new_object.isInManiger = false;
		if (new_object.renderers.size() > 0) {
			rebuild_render_list = true;
		}
		new_object.StartColliders();
		new_object.StartPhysics();
		new_object.StartScripts();
	}

	void ObjectManager::queueGameObjectForRemoval(GameObject& existing_object)
	{
		if (existing_object.markedForDelete) return;
		pendingRemoveGameObjects.push_back(&existing_object);
		existing_object.markedForDelete = true;
	}

	void ObjectManager::clear_scene()
	{
		for (GameObject* object : game_objects) {
			queueGameObjectForRemoval(*object);
		}
	}

	void ObjectManager::removeGameObject(GameObject& existing_object) 
	{
		auto it = std::find(game_objects.begin(), game_objects.end(), &existing_object);
		if (it != game_objects.end()) {
			game_objects.erase(it);
			existing_object.isInManiger = false;
			if (existing_object.renderers.size() > 0){
				rebuild_render_list = true;
			}
			delete &existing_object;
		}
	}

	void ObjectManager::queueGameObject(GameObject& new_object) {
		pendingAddGameObjects.push_back(&new_object);
	}

	void ObjectManager::update(Tmpl8::Surface& screen) {
		ProcessLifecycleQueues();
		for (GameObject* object : game_objects) {
			if(object->isActive()) object->updateScripts();
		}
		for (GameObject* object : game_objects) {
			if (object->isActive()) object->updatePhysics();
		}
		for (GameObject* object : game_objects) {
			if (object->isActive()) object->updateTriggers();
		}
		collisionUpdate(game_objects);
		render(screen);
#ifdef _DEBUG
		for (GameObject* object : game_objects){
			if (!object->isActive()) continue;
			for (IAABB* collider : object->colliders) {
				 collider->draw_outline(screen);
			}
		}
#endif
	}

	Util::Option<GameObject&> ObjectManager::get_gameObject(const string& name)
	{
		for (GameObject* object : game_objects) {
			if (object->name == name) {
				return Util::Option<GameObject&>(*object);
			}
		}
		return Util::Option<GameObject&>::None();
		
	}

	vector<RefCell<GameObject>> ObjectManager::get_object_by_label(const Label lable)
	{
		vector<RefCell<GameObject>> vec{};
		for (GameObject* object : game_objects) {
			if (object->label == lable) {
				vec.push_back(object);
			}
		}
		return vec;
	}

	void ObjectManager::collisionUpdate(std::vector<GameObject*>& gameobjects) {
		for (GameObject* object : gameobjects) {
			if (!object->isActive()) continue;
			for (IAABB* collider : object->colliders) {
				if (!collider) {
					fmt::println("null pointer in AABB list of game object: {}", object->name);
					continue;
				}
				if (collider->staticCollider) continue; // only dynamic colliders initiate check
				for (GameObject* other_object : gameobjects) {
					if (object == other_object || !other_object->isActive()) continue;
					for (IAABB* other_collider : other_object->colliders) {
						if (!other_collider) {
							fmt::println("null pointer in AABB list of game object: {}", other_object->name);
							continue;
						}
						collisionCheck(*collider, *other_collider);

					}
				}
			}
		}
	}
	//TODO not O(n^2) collision detection

	//collider1 is dynamic, 2 is stationary/other
	void ObjectManager::collisionCheck(IAABB& collider1, IAABB& collider2) {
		Tmpl8::vec2 pos1 = collider1.ofset + collider1.gameobject->pos;
		Tmpl8::vec2 pos2 = collider2.ofset + collider2.gameobject->pos;

		float delta_x = fabsf(pos1.x - pos2.x);
		float delta_y = fabsf(pos1.y - pos2.y);
		float min_delta_x = collider1.size.x / 2 + collider2.size.x / 2;
		float min_delta_y = collider1.size.y / 2 + collider2.size.y / 2;

		collider1.gameobject->ColStat = Air;

		if (!((delta_y-1 <= min_delta_y) && (delta_x -1 <= min_delta_x))) return;
		if (collider1.isTrigger) {
			if(collider2.isTrigger) {
				collider1.doCollision(collider2);
				collider2.doCollision(collider1);
			} else {
				collider1.doCollision(collider2);
			}
		}
		else {
			if(collider2.isTrigger) {
				collider2.doCollision(collider1); 
			} else {
				if (collider2.staticCollider) {
					collider2.doCollision(collider1, min_delta_x - delta_x, min_delta_y - delta_y);
				}
				else {
					//both moving coliders
					//TODO what to do here
					TODO("moving collides collision");
				}
			}
		}
	}

}
