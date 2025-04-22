#include "Collider.h"
#include "GameObject.h"

#ifdef _DEBUG
#include "Color.h"
#endif // _DEBUG



Core::Trigger::Trigger(vec2 a_size, vec2 a_ofset)
{
	size = a_size;
	ofset = a_ofset;
	isTrigger = true;
}

Core::Trigger::Trigger(vec2 a_size, vec2 a_ofset, std::string a_id)
{
	size = a_size;
	ofset = a_ofset;
	isTrigger = true;
	id = a_id;
}

// Updates the colided list for tracking if a object stay or exited.
void Core::Trigger::Update()
{
	for (auto it = colided.begin(); it != colided.end(); ) {  
		State& state = it->second;

		if (state == ENTER || state == STAY) {
			it->second = FROREMOVAL;
			++it;  
		}
		else if (state == FROREMOVAL) {
			it->second = EXIT;

			if (it->first) {
				onTriggerExit.Invoke(*it->first);
				++it;  
			}
			else {
				fmt::println("null pointer in trigger collided list in game object: {}", gameobject->name);
				it = colided.erase(it); 
			}
		}
		else if (state == EXIT) {
			it = colided.erase(it);  
		}
		else {
			++it;  
		}
	}
}
// overlap isn't a use but a left over from the virtual fiction
// adds object the collided list
void Core::Trigger::doCollision(IAABB& other, float overlap_x, float overlap_y)
{
	GameObject* object = other.gameobject;
	if (colided.find(object) == colided.end()) {
		colided[object] = ENTER;
		onTriggerEnter.Invoke(*object);
		return;
	}
	if (colided[object] == FROREMOVAL) {
		colided[object] = STAY;
		onTriggerStay.Invoke(*object);
	}
}

void Core::Trigger::EventTriggerEnter(GameObject* other)
{
	if (!other) return;
	onTriggerEnter.Invoke(*other);
}

void Core::Trigger::EventTriggerExit(GameObject* other)
{
	if (!other) return;
	onTriggerExit.Invoke(*other);
}


void Core::Trigger::EventTriggerStay(GameObject* other)
{
	if (!other) return;
	onTriggerStay.Invoke(*other);
}

void Core::Trigger::RefireTriggerEnter()
{
	for (auto& value : colided) {
		if (value.second == ENTER) {
			if (value.first) {
				onTriggerEnter.Invoke(*value.first);
			}
			else {
				colided.erase(value.first);
				fmt::println("null pointer in trigger collided list in game object: {}. when refirering enter",gameobject->name);
			}
		}
	}
}

void Core::Trigger::RefireTriggerStay()
{
	for (auto& value : colided) {
		if (value.second == STAY) {
			if (value.first) {
				onTriggerStay.Invoke(*value.first);
			}
			else {
				colided.erase(value.first);
				fmt::println("null pointer in trigger collided list in game object: {}. when refirering stay", gameobject->name);
			}
		}
	}
}

void Core::Trigger::RefireTriggerExit()
{
	for (auto& value : colided) {
		if (value.second == EXIT) {
			if (value.first) {
				onTriggerExit.Invoke(*value.first);
			}
			else {
				colided.erase(value.first);
				fmt::println("null pointer in trigger collided list in game object: {}. when refirering exit", gameobject->name);
			}
		}
	}
}

Core::Collider::Collider(void(*a_behaviour)(IAABB&, IAABB& ,ColDir, float), bool isStatic)
{
	behaviour = a_behaviour;
	staticCollider = isStatic;
}

Core::Collider::Collider(vec2 a_size, vec2 a_ofset, bool isStatic)
{
	size = a_size;
	ofset = a_ofset;
	staticCollider = isStatic;
}

Core::Collider::Collider(vec2 a_size, vec2 a_ofset, void(*a_behaviour)(IAABB&, IAABB&, ColDir, float), bool isStatic)
{
	size = a_size;
	ofset = a_ofset;
	staticCollider = isStatic;
	behaviour = a_behaviour;
}

void Core::Collider::setStatic(bool isStatic)
{
	staticCollider = isStatic;
}

void Core::Collider::doCollision(IAABB& other, float overlap_x, float overlap_y)
{
	ColDir dir = ColDir::NONE;
	float displacement{};

	if (overlap_x < overlap_y) {
		// Horizontal collision
		dir = (other.gameobject->velocity.x > 0) ? ColDir::LEFT :
			(other.gameobject->velocity.x < 0) ? ColDir::RIGHT :
			(other.gameobject->pos.x < gameobject->pos.x) ? ColDir::LEFT : ColDir::RIGHT;
		displacement = overlap_x + 1.0f;
	}
	else {
		// Vertical collision
		dir = (other.gameobject->velocity.y > 0) ? ColDir::UP :
			(other.gameobject->velocity.y < 0) ? ColDir::DOWN : 
			(other.gameobject->pos.y < gameobject->pos.y)? ColDir::UP: ColDir::DOWN;
		displacement = overlap_y + 1.0f;
	}
	//call custom function
	behaviour(other, *this ,dir, displacement);
}


void Core::Collider::setColliderBehaviour(void(*a_behaviour)(IAABB&, IAABB&, ColDir , float)) { this->behaviour = a_behaviour; }

#ifdef _DEBUG
void Core::IAABB::draw_outline(Tmpl8::Surface& screen) const 
{
	float x = gameobject->pos.x + ofset.x, y = gameobject->pos.y + ofset.y;
	screen.Box(static_cast<int>(x - size.x / 2), static_cast<int>(y - size.y / 2),
		static_cast<int>(x + size.x / 2), static_cast<int>(y + size.y / 2), Util::Color::RED.ToInt());
}
#endif
