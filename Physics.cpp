#include "Physics.h"
#include "GameObject.h"

namespace Core {

	void Gravity::Update()
	{
		gameobject->velocity += Tmpl8::vec2::DOWN * Globals::gravity * Globals::DeltaTime;
	}
	void FlappyController::Update()
	{
		if (Input::GetKeyDown(controle_button)) {
			gameobject->velocity.y = Globals::jump_inpuls;
			gameobject->ColStat = Air;
		}
	}
	void PlatformerController::Update()
	{
        float move_dir = static_cast<float>(Input::GetKey(D) - Input::GetKey(A));

        if (gameobject->ColStat == Grounded && Input::GetKeyDown(Space)) {
            gameobject->velocity.y = vec2::UP.y * Globals::jump_inpuls;
            gameobject->ColStat = Air;
        }

        if (move_dir == 0) return;

        float vel_inc = move_dir * ((gameobject->ColStat == Grounded) ? 1.0f : Globals::air_control) * Globals::DeltaTime * Globals::move_acceleration;
        float abs_vel_x = fabsf(gameobject->velocity.x);


         if (abs_vel_x < Globals::max_move_speed || gameobject->velocity.x * vel_inc < 0) { 
             // only changes the velocity when the velocity is smaller than max velocity or the movement direction is in the opposite direction
             gameobject->velocity.x += vel_inc;
            if (gameobject->velocity.x > Globals::max_move_speed) {
                gameobject->velocity.x = copysignf(Globals::max_move_speed, gameobject->velocity.x);
            }
        }
	}
    void TopDownController::Update()
    {
        vec2 move_dir(static_cast<float>(Input::GetKey(D) - Input::GetKey(A)), static_cast<float>(Input::GetKey(S) - Input::GetKey(W)));
        if (move_dir.x == 0 && move_dir.y == 0) return;
        move_dir.normalize(); // won't work if a controller is used
        
        float current_speed = gameobject->velocity.length();
        vec2 vel_inc = move_dir * Globals::DeltaTime * Globals::move_acceleration;
        
        if ( current_speed < Globals::max_move_speed || gameobject->velocity.dot(vel_inc) < 0) {
            // only changes the velocity when the velocity is smaller than max velocity or the movement direction is in the opposite direction
            gameobject->velocity += vel_inc;
            if (current_speed > Globals::max_move_speed) {
                gameobject->velocity = gameobject->velocity.normalized() * Globals::max_move_speed;
            }
        }
    }
}
