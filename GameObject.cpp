#include "GameObject.h"
#include "Globals.h"
#include "ObjectManager.h"

#include "Cloning.h"

namespace Core {
	// Move components and reassign their gameobject pointers to this new object
	GameObject::GameObject(GameObject&& other) noexcept
	{
		this->pos = other.pos;
		this->renderers = std::move(other.renderers);
		this->scripts = std::move(other.scripts);
		this->name = std::move(other.name);
		this->physics = std::move(other.physics);
		this->colliders = std::move(other.colliders);
		for (IAABB* collider : colliders) {
			collider->gameobject = this;
		}
		for (IRenderer* renderer : renderers) {
			renderer->gameobject = this;
		}
		for (IScript* script : scripts) {
			script->gameobject = this;
		}
		for (IPhysics* physic : physics) {
			physic->gameobject = this;
		}
		other.colliders.clear();
		other.physics.clear();
		other.renderers.clear();
		other.scripts.clear();
		other.name.clear();
	}
	GameObject::GameObject(const GameObject& other)
	{
		this->pos = other.pos;
		this->name = other.name; 
		for (IRenderer* renderer : other.renderers) {
			Util::ICloneable<IRenderer>* clo = dynamic_cast<Util::ICloneable<IRenderer>*>(renderer);
			if (clo) {
				IRenderer* new_render = clo->Clone();
				new_render->gameobject = this;
				this->renderers.push_back(new_render);
			}
		}
		for (IScript* script : other.scripts) {
			Util::ICloneable<IScript>* clo = dynamic_cast<Util::ICloneable<IScript>*>(script);
			if (clo) {
				IScript* new_script = clo->Clone();
				new_script->gameobject = this;
				this->scripts.push_back(new_script);
			}
		}
		for (IPhysics* physic : other.physics) {
			Util::ICloneable<IPhysics>* clo = dynamic_cast<Util::ICloneable<IPhysics>*>(physic);
			if (clo) {
				IPhysics* new_phys = clo->Clone();
				new_phys->gameobject = this;
				this->physics.push_back(new_phys);
			}
		}
		for (IAABB* collider : other.colliders) {
			Util::ICloneable<IAABB>* clo = dynamic_cast<Util::ICloneable<IAABB>*>(collider);
			if (clo) {
				IAABB* new_collider = clo->Clone();
				new_collider->gameobject = this;
				this->colliders.push_back(new_collider);
			}
		}
	}

	void GameObject::_Direct_AddRenderer(IRenderer& renderer) {
		renderer.gameobject = this;
		renderers.push_back(&renderer);
		if(isInManiger)	ObjectManager::rebuild_render_list = true;
		if (!is_active) {
			renderer.deactivate(true);
		}
	}

	void GameObject::_Direct_AddPhysics(IPhysics& physics)
	{
		physics.gameobject = this;
		auto it = std::lower_bound(this->physics.begin(), this->physics.end(), &physics,
			[](IPhysics* a, IPhysics* b) -> bool {
				return a->getPriority() > b->getPriority(); // Higher priority = earlier in list
			});
		this->physics.insert(it ,&physics);
	}

	void GameObject::_Direct_AddScript(IScript& script) {
		script.gameobject = this;
		scripts.push_back(&script);
	}

	void GameObject::updateScripts() {
		for (IScript* script : scripts) {
			script->Update();
		}
	}

	void GameObject::updateTriggers()
	{
		for (IAABB* collider : colliders) {
			collider->Update();
		}
	}

	void GameObject::StartColliders()
	{
	}

	void GameObject::StartPhysics()
	{
		for (IPhysics* physic : physics) {
			physic->Start();
		}
	}

	void GameObject::StartScripts()
	{
		for (IScript* script : scripts) {
			script->Start();
		}
	}

	void GameObject::StopSctipts()
	{
		for (IScript* script : scripts) {
			script->Stop();
		}
	}

	void GameObject::_Direct_AddCollider(IAABB& collider)
	{
		collider.gameobject = this;
		colliders.push_back(&collider);
	}

	void GameObject::_removeRenderer(IRenderer& renderer)
	{
		auto it = std::find(renderers.begin(), renderers.end(), &renderer);
		if (it != renderers.end()) {
			renderers.erase(it);
			if(isInManiger)	ObjectManager::rebuild_render_list = true; 
			delete &renderer;
		}
		
	}
	void GameObject::_removeScript(IScript& script) {
		auto it = std::find(scripts.begin(), scripts.end(), &script);
		if (it != scripts.end()) {
			scripts.erase(it);
			script.Stop();
			delete &script;
		}
	}

	void GameObject::_removePhysics(IPhysics& physics)
	{
		auto it = std::find(this->physics.begin(), this->physics.end(), &physics);
		if (it != this->physics.end()) {
			this->physics.erase(it);
			delete &physics;
		}
	}

	void GameObject::_removeCollider(IAABB& collider) {
		auto it = std::find(colliders.begin(), colliders.end(), &collider);
		if (it != colliders.end()) {
			colliders.erase(it);
			delete &collider;
		}
	
	}

	void GameObject::initializeWithComponent(Component& component)
	{
		if (auto script = dynamic_cast<IScript*>(&component)) {
			_Direct_AddScript(*script);
		}
		else if (auto renderer = dynamic_cast<IRenderer*>(&component)) {
			_Direct_AddRenderer(*renderer);
		}
		else if (auto physics = dynamic_cast<IPhysics*>(&component)) {
			_Direct_AddPhysics(*physics);
		}
		else if (auto collider = dynamic_cast<IAABB*>(&component)) {
			_Direct_AddCollider(*collider);
		}
		else {
			fmt::println("Component type not recognized");
			return;
		}
	}

	void GameObject::injectComponent(Component& component)
	{
		if (auto script = dynamic_cast<IScript*>(&component)) {
			_Direct_AddScript(*script);
			script->Start();
		}
		else if (auto renderer = dynamic_cast<IRenderer*>(&component)) {
			_Direct_AddRenderer(*renderer);
		}
		else if (auto physics = dynamic_cast<IPhysics*>(&component)) {
			_Direct_AddPhysics(*physics);
			physics->Start();
		}
		else if (auto collider = dynamic_cast<IAABB*>(&component)) {
			_Direct_AddCollider(*collider);
		}
		else {
			fmt::println("Component type not recognized");
			return;
		}
	}

	void GameObject::queueComponent(Component& component)
	{
		if (auto script = dynamic_cast<IScript*>(&component)) {
			ObjectManager::scriptsAdd.push_back({ this, script });
		}
		else if (auto renderer = dynamic_cast<IRenderer*>(&component)) {
			ObjectManager::renderersAdd.push_back({ this, renderer });
		}
		else if (auto physics = dynamic_cast<IPhysics*>(&component)) {
			ObjectManager::physicsAdd.push_back({ this, physics });
		}
		else if (auto collider = dynamic_cast<IAABB*>(&component)) {
			ObjectManager::collidersAdd.push_back({ this, collider });
		}
		else {
			fmt::println("Component type not recognized");
			return;
		}
	}

	void GameObject::removeComponent(Component& component)
	{
		if (auto script = dynamic_cast<IScript*>(&component)) {
			_removeScript(*script);
		}
		else if (auto renderer = dynamic_cast<IRenderer*>(&component)) {
			_removeRenderer(*renderer);
		}
		else if (auto physics = dynamic_cast<IPhysics*>(&component)) {
			_removePhysics(*physics);
		}
		else if (auto collider = dynamic_cast<IAABB*>(&component)) {
			_removeCollider(*collider);
		}
		else {
			fmt::println("Component type not recognized");
			return;
		}
	}

	void GameObject::queueComponentForRemoval(Component& component)
	{
		if (component.gameobject == nullptr) return;
		if (component.gameobject->markedForDelete == true || component.markedForDelete == true) return; 
		component.markedForDelete = true;

		if (auto script = dynamic_cast<IScript*>(&component)) {
			ObjectManager::scriptsRemove.push_back(script);
		}
		else if (auto renderer = dynamic_cast<IRenderer*>(&component)) {
			ObjectManager::renderersRemove.push_back( renderer );
		}
		else if (auto physics = dynamic_cast<IPhysics*>(&component)) {
			ObjectManager::physicsRemove.push_back( physics );
		}
		else if (auto collider = dynamic_cast<IAABB*>(&component)) {
			ObjectManager::collidersRemove.push_back( collider);
		}
		else {
			fmt::println("Component type not recognized");
			return;
		}
	}

	void GameObject::updatePhysics()
	{
		for (IPhysics* physic : physics) {
			physic->Update();
		}
		pos += velocity * Globals::DeltaTime;
	}

	// Cleanup all dynamically allocated components and stop scripts
	GameObject::~GameObject() {
		StopSctipts();
		for (IRenderer* renderer : renderers) {
			delete renderer;
		}
		for (IScript* script : scripts) {
			delete script;
		}
		for (IPhysics* physics : physics) {
			delete physics;
		}
		for(IAABB* collider : colliders) {
			delete collider;
		}
	}

	void GameObject::activate()
	{
		if(is_active) return;

		is_active = true;
		for (IRenderer* renderer : renderers) {
			if (renderer->wasActive()) {
				renderer->activate();
			}
		}

	}
	void GameObject::deactivate()
	{
		if(!is_active) return;

		is_active = false;
		for (IRenderer* renderer : renderers) {
			renderer->deactivate(true);
		}
	}
}