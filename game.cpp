#include "game.h"
#include "surface.h"

#include "ObjectManager.h"

#include "coded_scene.h"

#include "Globals.h"
#include "ResourceManager.h"


using namespace Core;
using Util::ResourceManager;
using Util::Input;

namespace Tmpl8
{
#ifdef FPSCOUNT
	constexpr int UPDATETIME = 1000;
	int framecount = 0;
	float timepased = UPDATETIME*0.8;
	std::string fps_string = "FPS: 0";
	bool showfps = true;
#endif // FPSCOUNT

	// -----------------------------------------------------------
	// Initialize the application
	// ----------------------------------------------------------

	void Game::Init()
	{
#ifdef FPSCOUNT
		showfps = Globals::screen_height > 10 && Globals::screen_width > 48;
#endif // FPSCOUNT
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		Util::load_pong_menu();

		if (screen) {
			ObjectManager::render(*screen);
		}
		else {
			Panic("Screen isn't a valid pointer anymore.");
		}
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		ObjectManager::cleanup();
		ResourceManager::cleanup();
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		Globals::DeltaTime = deltaTime > 1/24.0f*1000 ? 1/24.0f : deltaTime/1000;
		Globals::Time = deltaTime/1000;
		
		if (screen) {
			screen->Clear(0);
			ObjectManager::update(*screen);
		}
		else {
			Panic("Screen isn't a valid pointer anymore.");
		}

		
#ifdef FPSCOUNT
		if (showfps) {
			if (timepased > UPDATETIME && framecount != 0)
			{
				int fps = static_cast<int>(((framecount * 1000.0f) / timepased) + 0.5f);
				fps_string = "FPS: " + std::to_string(fps);
				timepased = 0;
				framecount = 0;
			}
			framecount++;
			screen->Print(fps_string.c_str(), 4, 4, 0xffffff);
			timepased += deltaTime;
		}
#endif // FPSCOUNT

		Util::Input::TickList();


	}

	void Game::MouseUp(int button)
	{
		Input::MouseUpEvent(static_cast<Util::MouseButton>(button));
	}

	void Game::MouseDown(int button)
	{
		Input::MouseDownEvent(static_cast<Util::MouseButton>(button));
	}

	void Game::KeyDown(int key)
	{
		Input::KeyDownEvent(static_cast<Util::Key>(key));
	}

	void Game::KeyUp(int key)
	{
		Input::KeyUpEvent(static_cast<Util::Key>(key));
	}
};
