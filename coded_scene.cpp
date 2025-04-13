#include "coded_scene.h"

#include "ObjectManager.h"
#include "PongScripts.hpp"

using namespace Core;

namespace Util {

		void load_pong(bool with_AI, bool with_mouce, int difuculty) {
			ObjectManager::clear_scene();

			// Creating the ball
			GameObject* ball = new GameObject("ball", { Globals::screen_width / 2.0f, Globals::screen_height / 2.0f }, false);
			{
				ball->initializeWithComponent(*new RectRenderer({ 10,10 }, {}, Color::WHITE, Core::Player));
				ball->initializeWithComponent(*new Collider{ {10,10},{}, false });
				ball->initializeWithComponent(*new Scripts::BallInfo());
			}
			ObjectManager::queueGameObject(*ball);

			// Creating the left paddle
			GameObject* leftPadle = new GameObject{ "left_padle", {30, Globals::screen_height / 2.0f}, false };
			{
				RectRenderer* renderer = new RectRenderer({ 10,100 }, {}, Color::WHITE, Core::Player);
				leftPadle->initializeWithComponent(*renderer);
				Collider* col = new Collider{ {10,104},{},ColFunc::PongPadleCollision };
				leftPadle->initializeWithComponent(*col); // idk how static will do
				leftPadle->initializeWithComponent(*(with_AI ? static_cast<IScript*>(new Scripts::PongAI(*ball, difuculty)) : static_cast<IScript*>(new Scripts::PlayerPadleControler(with_AI, with_mouce))));
				leftPadle->initializeWithComponent(*new Scripts::PowerTracker(*col, *renderer));
			}
			ObjectManager::queueGameObject(*leftPadle);

			// Creating the right paddle
			GameObject* RightPadle = new GameObject{ "right_padle", {Globals::screen_width - 30.0f, Globals::screen_height / 2.0f}, false };
			{
				RectRenderer* renderer = new RectRenderer({ 10,100 }, {}, Color::WHITE, Core::Player);
				RightPadle->initializeWithComponent(*renderer);
				Collider* col = new Collider{ {10,104},{},ColFunc::PongPadleCollision };
				RightPadle->initializeWithComponent(*col); // idk how static will do
				RightPadle->initializeWithComponent(*new Scripts::PlayerPadleControler(with_AI, with_mouce));
				RightPadle->initializeWithComponent(*new Scripts::PowerTracker(*col, *renderer));
			}
			ObjectManager::queueGameObject(*RightPadle);

			//creating scenery 
			GameObject* scenery = new GameObject("scenery", { 0,0 }, false);
			{
				//top and bottom walls
				scenery->initializeWithComponent(*new Collider({ 1000,4 }, { Globals::screen_width / 2.0f , 2 }, ColFunc::PerfecBounceCollision));
				scenery->initializeWithComponent(*new RectRenderer{ { 1000, 4}, { Globals::screen_width / 2.0f , 2 },Color::WHITE, Core::BackGround });
				scenery->initializeWithComponent(*new Collider({ 1000,4 }, { Globals::screen_width / 2.0f , Globals::screen_height - 2.0f }, ColFunc::PerfecBounceCollision));
				scenery->initializeWithComponent(*new RectRenderer{ { 1000,4 }, { Globals::screen_width / 2.0f , Globals::screen_height - 2.0f },Color::WHITE, Core::BackGround });

				// middle divider
				for (int y = 10; y < Globals::screen_height - 6; y += 30) {
					scenery->initializeWithComponent(*new RectRenderer({ 4, 20 }, { Globals::screen_width / 2.0f, static_cast<float>(y) }, Color::WHITE, Core::BackGround));
				}
			}
			ObjectManager::queueGameObject(*scenery);

			// Creating the game controler
			GameObject* gameControler = new GameObject("GameControler", {});
			{
				auto left_trigger = new Trigger({ 100, 1000 }, { -50, Globals::screen_height / 2.0f }, "left_trigger");
				auto right_trigger = new Trigger({ 100, 1000 }, { Globals::screen_width + 49.0f, Globals::screen_height / 2.0f }, "right_trigger");

				gameControler->initializeWithComponent(*new Scripts::PongController(*ball, *left_trigger, *right_trigger, *leftPadle, *RightPadle, with_AI));

				gameControler->initializeWithComponent(*left_trigger);
				gameControler->initializeWithComponent(*right_trigger);

			}
			ObjectManager::queueGameObject(*gameControler);

		}

		void load_pong_menu()
		{
			ObjectManager::clear_scene();

			GameObject* menu = new GameObject("menu", { 0,0 });
			{
				menu->initializeWithComponent(*new Scripts::PongMenu());
			}
			ObjectManager::queueGameObject(*menu);

		}

		void load_win_pong(bool left_won, bool was_AI)
		{
			ObjectManager::clear_scene();

			GameObject* menu = new GameObject("menu", { 0,0 });
			{
				menu->initializeWithComponent(*new Scripts::PongWin(left_won, was_AI));
			}
			ObjectManager::queueGameObject(*menu);
		}

}