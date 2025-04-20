#pragma once
#include "button.h"
#include "GameObject.h"
#include "Globals.h"
#include "coded_scene.h"

using PreBuild::Button;

namespace Scripts {

	/// <summary>
	/// setup and manages the win/lose screen of the pong game
	/// </summary>
	class PongWin : public virtual Core::IScript {
	private:
		bool left_won{ false };
		bool With_AI{ false };
	public:
		PongWin(bool a_left_won, bool a_with_AI) : left_won(a_left_won), With_AI(a_with_AI) {}

		void Start() override {
			string text;
			if (With_AI) {
				if (!left_won) {
					text = "You won!";
				}
				else {
					text = "You lost!";
				}
			}
			else {
				if (left_won) {
					text = "Left won!";
				}
				else {
					text = "Right won!";
				}
			}

			gameobject->initializeWithComponent(*new Core::TextRenderer({ Globals::screen_width / 2.0f, Globals::screen_height / 4.0f }, text, 50, Color::WHITE, Core::CENTER, Core::UI));

			Button* restart = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 40 }, "restart", Color::WHITE),
				* exit = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 120 }, "exit", Color::WHITE);
			gameobject->injectComponent(*restart);
			gameobject->injectComponent(*exit);
			restart->onClick.AddListener(this, [this]() {load_pong_menu(); });
			exit->onClick.AddListener(this, [this]() {Globals::ExitApp(); });
		}
		void Stop() override {}
		void Update() override {}
	};

	/// <summary>
	/// setup and manages the begin screen in the pong game were you can select how you want to play
	/// </summary>
	class PongMenu : public virtual Core::IScript {
	private:
		Button* TopButton{ nullptr }, * BottomButton{ nullptr }, * button3{ nullptr }, * button4{ nullptr };
		bool with_mouse = false;

		void singelplayer() {
			gameobject->queueComponentForRemoval(*TopButton);
			gameobject->queueComponentForRemoval(*BottomButton);

			TopButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) - 40 }, "KeyBoard", Color::WHITE);
			BottomButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 40 }, "mouse", Color::WHITE);

			gameobject->queueComponent(*TopButton);
			gameobject->queueComponent(*BottomButton);

			TopButton->onClick.AddListener(this, [this]() {keyboard();});
			BottomButton->onClick.AddListener(this, [this]() {mouce(); });
		}
		static void multiplayer() {
			load_pong(false, false, 1);
		}

		void mouce() {
			with_mouse = true;
			set_difecultybutons();
		}

		void keyboard() {
			with_mouse = false;
			set_difecultybutons();
		}

		void set_difecultybutons() {
			gameobject->queueComponentForRemoval(*TopButton);
			gameobject->queueComponentForRemoval(*BottomButton);

			TopButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) - 120 }, "easy", Color::WHITE);
			BottomButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) - 40 }, "medium", Color::WHITE);
			button3 = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 40 }, "hard", Color::WHITE);
			button4 = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 120 }, "impossible", Color::WHITE);

			gameobject->queueComponent(*TopButton);
			gameobject->queueComponent(*BottomButton);
			gameobject->queueComponent(*button3);
			gameobject->queueComponent(*button4);

			TopButton->onClick.AddListener(this, [this]() {chuse_dificulty_1(); });
			BottomButton->onClick.AddListener(this, [this]() {chuse_dificulty_2(); });
			button3->onClick.AddListener(this, [this]() {chuse_dificulty_3(); });
			button4->onClick.AddListener(this, [this]() {chuse_dificulty_4(); });
		}

		void chuse_dificulty_1() {
			load_pong(true, with_mouse, 1);
		}
		void chuse_dificulty_2() {
			load_pong(true, with_mouse, 2);
		}
		void chuse_dificulty_3() {
			load_pong(true, with_mouse, 3);
		}
		void chuse_dificulty_4() {
			load_pong(true, with_mouse, 4);
		}

	public:
		void Start() override {
			TopButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) - 40 }, "one player", Color::WHITE);
			BottomButton = new Button({ 300, 65 }, { Globals::screen_width / 2.0f, (Globals::screen_height / 2.0f) + 40 }, "two player", Color::WHITE);
			gameobject->injectComponent(*TopButton);
			gameobject->injectComponent(*BottomButton);

			TopButton->onClick.AddListener(this, [this]() {singelplayer(); });
			BottomButton->onClick.AddListener(this, multiplayer);

		};

		void Stop() override {};
		void Update() override {};
	};
}