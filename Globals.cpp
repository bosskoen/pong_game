#include "Globals.h"
#include "SDL.h"

float Globals::DeltaTime = 0;

// set in Tmpl8, a different cases point
const int& Globals::screen_width = ScreenWidth;
const int& Globals::screen_height = ScreenHeight;

float Globals::gravity = 400;
float Globals::jump_inpuls = 400;
float Globals::move_acceleration = 2000;
float Globals::air_control = 0.2;
float Globals::max_move_speed = 200;
float Globals::Time = 0;

std::unordered_map<Util::Key, Util::Input::Status> Util::Input::keyStatus;
std::unordered_map<Util::MouseButton, Util::Input::Status> Util::Input::mouseStatus;


Tmpl8::vec2int Util::Input::mousePos()
{
    Tmpl8::vec2int mousePos = {};
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return mousePos;
}

bool Util::Input::GetMouseHeld(MouseButton key)
{
    auto it = mouseStatus.find(key);
    return it != mouseStatus.end() && it->second == KeyHeld;
}

bool Util::Input::GetMouseDown(MouseButton key)
{
    auto it = mouseStatus.find(key);
    return it != mouseStatus.end() && it->second == KeyDown;
}

bool Util::Input::GetMouseUp(MouseButton key)
{
    auto it = mouseStatus.find(key);
    return it != mouseStatus.end() && it->second == KeyUp;
}

/// <summary>
/// returns true if the key is in either KeyDown or KeyHeld state.
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool Util::Input::GetMouse(MouseButton key)
{
    auto it = mouseStatus.find(key);
    return it != mouseStatus.end() && (it->second == KeyDown || it->second == KeyHeld);
}

void Util::Input::TickList() {
    for (auto it = keyStatus.begin(); it != keyStatus.end(); ) {
        if (it->second == KeyDown) {
            it->second = KeyHeld;
            ++it;
        }
        else if (it->second == KeyUp) {
            it = keyStatus.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = mouseStatus.begin(); it != mouseStatus.end(); ) {
		if (it->second == KeyDown) {
			it->second = KeyHeld;
			++it;
		}
		else if (it->second == KeyUp) {
			it = mouseStatus.erase(it);
		}
		else {
			++it;
		}
	}
}

void Util::Input::KeyDownEvent(Key key) {
    if (keyStatus[key] != KeyHeld) {
        keyStatus[key] = KeyDown;
    }
}

void Util::Input::KeyUpEvent(Key key) {
    if (keyStatus.find(key) != keyStatus.end()) {
        keyStatus[key] = KeyUp;
    }
}

void Util::Input::MouseDownEvent(MouseButton key)
{
    if (mouseStatus[key] != KeyHeld) {
        mouseStatus[key] = KeyDown;
    }
}

void Util::Input::MouseUpEvent(MouseButton key)
{
    if (mouseStatus.find(key) != mouseStatus.end()) {
        mouseStatus[key] = KeyUp;
    }
}

bool Util::Input::GetKeyHeld(Key key) {
    // Check if the key exists and is in KeyHeld state
    auto it = keyStatus.find(key);
    return it != keyStatus.end() && it->second == KeyHeld;
}

bool Util::Input::GetKeyDown(Key key) {
    // Check if the key exists and is in KeyDown state
    auto it = keyStatus.find(key);
    return it != keyStatus.end() && it->second == KeyDown;
}

bool Util::Input::GetKeyUp(Key key) {
    // Check if the key exists and is in KeyUp state
    auto it = keyStatus.find(key);
    return it != keyStatus.end() && it->second == KeyUp;
}

/// <summary>
/// returns true if the key is in either KeyDown or KeyHeld state.
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
bool Util::Input::GetKey(Key key) {
    // Check if the key exists and is in either KeyDown or KeyHeld state
    auto it = keyStatus.find(key);
    return it != keyStatus.end() && (it->second == KeyDown || it->second == KeyHeld);
}

void Globals::ExitApp()
{
    exitapp = true;
}
