#pragma once
#include "ObjectManager.h"
#include "renderer.h"

#include "Globals.h"
#include "coded_scene.h"

#include "Collider.h"


using namespace Core;


namespace Scripts {

	/// <summary>
	/// small data class that tracks what the last hit is so power ups can be added properly
	/// </summary>
	class BallInfo : public virtual Core::IScript {
	public:
		GameObject* last_hit{ nullptr };
		
		void Start() override {}
		void Stop() override {}
		void Update() override {}
	};

	/// <summary>
	/// a class that lets the player or players control the paddle(s).
	/// </summary>
	class PlayerPadleControler :public virtual Core::IScript {
	private:
		bool two_palyer{ false }, is_right{ true }, mouce{ true };

		RectRenderer* renderer{ nullptr };
	public:
		static float default_speed;
		float speed = default_speed;

		PlayerPadleControler(bool with_AI, bool with_mouce, bool isRight = true) : two_palyer(!with_AI), mouce(with_mouce), is_right(isRight) {}

		void Start() override {
			is_right = gameobject->name == "right_padle";
			renderer = &gameobject->getRenderer<RectRenderer>().expect("no renderer found");
		}
		void Stop() override {}
		void Update() override {

			if (two_palyer) {
				if (is_right) {
					int dir = Input::GetKey(Key::Down) - Input::GetKey(Key::Up);
					gameobject->pos.y += dir * Globals::DeltaTime * speed;
					//arrow
				}
				else {
					// W/S
					int dir = Input::GetKey(Key::S) - Input::GetKey(Key::W);
					gameobject->pos.y += dir * Globals::DeltaTime * speed;
				}
			}
			else {
				if (mouce) {
					// follow mouse
					float delta = Input::mousePos().y - gameobject->pos.y;
					float moved = std::copysign(speed, delta) * Globals::DeltaTime;
					if (fabsf(moved) > fabsf(delta)) {
						gameobject->pos.y += delta;
					}
					else {
						gameobject->pos.y += moved;
					}
					
				}
				else {
					// arrow and w/s
					int dir = (Input::GetKey(Key::Down) || Input::GetKey(Key::S)) - (Input::GetKey(Key::Up) || Input::GetKey(Key::W));
					gameobject->pos.y += dir * Globals::DeltaTime * speed;
				}

			}
			gameobject->pos.y = std::max((renderer->size.y/2.0f)+5.0f, std::min(gameobject->pos.y, Globals::screen_height - ((renderer->size.y / 2.0f) + 5.0f)));
		}
	};

	/// <summary>
	/// a mediocre AI for the paddle
	/// </summary>
	class PongAI : public virtual Core::IScript {
	private:
		GameObject& ball;
		int difuculty;
		bool moving_to_ai{ true }, first_frame{ true };
		int inacrucy = 0;
		float move_to = Globals::screen_height / 2.0f;

		int mis_chance = 0; // chance to miss the ball
		int style{ 0 };

		RectRenderer* renderer{ nullptr };

		float vertical_line_vec_intersect(vec2 pos, vec2 scaler, float line_x) {
			float y = pos.y + ((line_x - pos.x) / scaler.x) * scaler.y;
			return y;
		}
		float horezontal_line_vec_intersect(vec2 pos, vec2 scaler, float line_y) {
			float x = pos.x + ((line_y - pos.y) / scaler.y) * scaler.x;
			return x;
		}
	public:
		float speed = 0;

		PongAI(GameObject& a_ball, int a_difuculty) : difuculty(a_difuculty), ball(a_ball) {};

		void Start() override {
			renderer = &gameobject->getRenderer<RectRenderer>().expect("no renderer found");

			switch (difuculty) {
			case 2: // Medium
				speed = PlayerPadleControler::default_speed * 0.75f;
				inacrucy = 20;
				mis_chance = 25;
				break;
			case 3: // Hard
				speed = PlayerPadleControler::default_speed * 1.05f;
				inacrucy = 30; // intentionally LESS accurate for unpredictability
				break;
			default: // Easy
				speed = PlayerPadleControler::default_speed * 0.55f;
				inacrucy = 10;
				mis_chance = 55;
				break;
			}
			move_to = gameobject->pos.y;
		}
		void Stop() override {}

		void PredictTargetY() {
			float targetY = vertical_line_vec_intersect(ball.pos, ball.velocity, gameobject->pos.x);

			// If the prediction goes off-screen, simulate a bounce
			while (targetY < 0 || targetY > Globals::screen_height) {
				float bounceY = ball.velocity.y < 0 ? 4.0f : (Globals::screen_height - 4.0f);
				float bounceX = horezontal_line_vec_intersect(ball.pos, ball.velocity, bounceY);
				targetY = vertical_line_vec_intersect({ bounceX, bounceY }, { ball.velocity.x, -ball.velocity.y }, gameobject->pos.x);
				if (difuculty < 3) break;
			}

			// Add difficulty-based randomness
			int offset = (rand() % (2 * inacrucy)) - inacrucy;

			if(difuculty < 3){
				int r = rand() % 100;
				if (r < mis_chance) {
					int sign = (r % 2 == 0) ? 1 : -1;
					offset += sign * (40 + (rand() % 25));
				}
			}

			float finalY = targetY + offset;

			// Clamp to screen safe bounds (padding)
			move_to = std::max((renderer->size.y / 2.0f) + 5.0f, std::min(finalY, Globals::screen_height - ((renderer->size.y / 2.0f) + 5.0f)));
		}

		void Update() override {

			if (difuculty == 4) {
				// Impossible mode: track ball exactly
				gameobject->pos.y = ball.pos.y;
				return;
			}

			// calculate where to move to
			if (ball.velocity.x < 0) {
				//ball is moving to AI
				if (!moving_to_ai || first_frame) {
					PredictTargetY();
					moving_to_ai = true;
					first_frame = false;
				}
			}
			else {
				// ball is moving to player

				//TODO how to make not wiggle
				if (moving_to_ai) {
					moving_to_ai = false;
					style = rand() % 3;
				}
				if (move_to == gameobject->pos.y) {
					switch (difuculty) {
					case 2: // medium
						move_to = vertical_line_vec_intersect(ball.pos, ball.velocity, Globals::screen_width - gameobject->pos.x)
							+ (rand() % 120 - 60);
						break;
					case 3: // hard
						switch (style) {
						case 0: // fakeout
							move_to = (rand() % 2 == 0) ? 80.0f : (Globals::screen_height - 80.0f);
							break;
						case 1: // track ball
							move_to = ball.pos.y + (rand() % 40 - 20);
							break;
						default: // idle wobble
							move_to = (Globals::screen_height / 2.0f) + sinf(Globals::Time * 2.0f) * 70.0f;
							break;
						}
						break;

					default: // easy
						move_to = ball.pos.y + (rand() % 40 - 20);
						break;
					}
					move_to = std::max((renderer->size.y / 2.0f) + 5.0f, std::min(move_to, Globals::screen_height - ((renderer->size.y / 2.0f) + 5.0f)));
				}
				moving_to_ai = false;
			}

			// make the paddle move to the specified location
			float direction = move_to < gameobject->pos.y ? -1.0f : 1.0f;
			float to_move = direction * ((!moving_to_ai && style == 2) ? speed/2.0f : speed) * Globals::DeltaTime;
			if (fabsf(gameobject->pos.y - move_to) < fabsf(to_move)) {
				gameobject->pos.y = move_to;
			}
			else {
				gameobject->pos.y += to_move;
			}

			gameobject->pos.y = std::max((renderer->size.y / 2.0f) + 5.0f, std::min(gameobject->pos.y, Globals::screen_height - ((renderer->size.y / 2.0f) + 5.0f)));
		}
	};

	/// <summary>
	/// a class that tracks what power-ups are active in a paddle and changes stats and graphics accordingly
	/// </summary>
	class PowerTracker : public virtual Core::IScript {
	private:

		PongAI* ai{ nullptr };
		PlayerPadleControler* player{ nullptr };
		bool is_player{ false };

		Collider& collider;
		RectRenderer& renderer;

		float speed_timer = 0;
		float size_timer = 0;

		bool is_spedy = false;
		bool is_big = false;

		static const float speed_up_factor;
		static const float speed_up_time;
		static const float size_up_factor;
		static const float size_up_time;
	public:

		PowerTracker(Collider& a_collider, RectRenderer& a_renderer) : collider(a_collider), renderer(a_renderer) {}

		void speedUp() {
			if (speed_timer > 0) {
				speed_timer = speed_up_time;
			}
			else if (is_player) {
				player->speed *= speed_up_factor;
				speed_timer = speed_up_time;
			}
			else {
				ai->speed *= speed_up_factor;
				speed_timer = speed_up_time;
			}
			renderer.update_color(0x8cffe9);
			is_spedy = true;
		}

		void increaceSize() {
			if (size_timer > 0) {
				size_timer = size_up_time;
			}
			else {
				renderer.size.y *= size_up_factor;
				collider.size.y *= size_up_factor;
				size_timer = size_up_time;
			}
			renderer.update_color(0xfff98c);
			is_big = true;
		}

		void Start() override {
			auto& controler = gameobject->getScript<PlayerPadleControler>();
			if (controler) {
				player = &controler.unwrap();
				is_player = true;
			}
			else {
				auto& controler = gameobject->getScript<PongAI>();
				ai = &controler.expect("no controler found");
				is_player = false;
			}
		}
		void Stop() override {}
		void recalculateColor() {
			if (is_spedy && is_big) {
				renderer.update_color(0xd7ffa4); // Mix of both colors
			}
			else if (is_spedy) {
				renderer.update_color(0x8cffe9);
			}
			else if (is_big) {
				renderer.update_color(0xfff98c);
			}
			else {
				renderer.update_color(Color::WHITE);
			}
		}
		void Update() override {
			if (speed_timer > 0) {
				speed_timer -= Globals::DeltaTime;
			}
			else if (is_spedy){
				if (is_player) {
					player->speed /= speed_up_factor;
				}
				else {
					ai->speed /= speed_up_factor;
				}
				is_spedy = false;
				recalculateColor();
			}
			if (size_timer > 0) {
				size_timer -= Globals::DeltaTime;
			}
			else if (is_big) {
				renderer.size.y /= size_up_factor;
				collider.size.y /= size_up_factor;
				is_big = false;
				recalculateColor();
			}
		}

	};

	/// <summary>
	/// a class that handles the trigger events and animation of a power up
	/// </summary>
	class PowerUpManiger : public virtual Core::IScript {
	public:
		enum class PowerUpType
		{
			Speed,
			Health,
			Damage,
			Size,
		};
	private:


		// calculate the time between frames given a certain rmp at with the orb revolves (50 in this case)
		float const rpm = 50;
		float const time_to_wait = (60 / rpm) / 6;

		int frame = 0;
		float time_remaning = time_to_wait;

		SpriteRenderer& sprite_renderer;
		PowerUpType powerup_type{};
	public:

		Event<int> change_right, change_left;

		PowerUpManiger(SpriteRenderer& a_sprite_renderer, PowerUpType a_powerup_type) : sprite_renderer(a_sprite_renderer), powerup_type(a_powerup_type) {};

		void Start() override {
			gameobject->getCollider<Trigger>("trigger").expect("there was no trigger").onTriggerStay.AddListener(this, [this](GameObject& other) {onTrigger(other);});
		}

		void Stop() override {}
		void Update() override {
			if (time_remaning <= 0) {
				frame++;
				frame %= 6;
				time_remaning = time_to_wait + time_remaning;
				sprite_renderer.set_frame({ frame, static_cast <int>(powerup_type) });
			}
			time_remaning -= Globals::DeltaTime;
			//TODO life time??
		}

		void onTrigger(GameObject& other) {
			BallInfo& ball = other.getScript<BallInfo>().expect("powerUP got hit by not ball.");
			if (!ball.last_hit) return;

			switch (powerup_type)
			{
			case PowerUpType::Speed:
				ball.last_hit->getScript<PowerTracker>().expect("object does not have a power tracker").speedUp();
				break;
			case PowerUpType::Health:
				if (ball.last_hit->name == "left_padle") {
					change_left.Invoke(1);
				}
				else if (ball.last_hit->name == "right_padle") {
					change_right.Invoke(1);
				}
				break;
			case PowerUpType::Damage:
				if (ball.last_hit->name == "left_padle") {
					change_right.Invoke(-1);
				}
				else if (ball.last_hit->name == "right_padle") {
					change_left.Invoke(-1);
				}
				break;
			case PowerUpType::Size:
				ball.last_hit->getScript<PowerTracker>().expect("object does not have a power tracker").increaceSize();
				break;
			default:
				break;
			}

			ObjectManager::queueGameObjectForRemoval(*gameobject);
		}
	};

	/// <summary>
	///  a class that handles initializations point scoring win conditions, and power-up spawns.
	/// </summary>
	class PongController : public virtual Core::IScript
	{
	private:
		float time = 0;
		int left_score_val = 0, right_score_val = 0;
		Core::TextRenderer* start_text{ nullptr }, * left_score{ nullptr }, * right_score{ nullptr };
		char start_bool = 0b0000'1111; // 4 bools compacted in one variable to tracks which countdown numbers (3,2,1,GO) have been shown.
		bool game_started = false, with_ai;

		float time_to_next_spawn = 2.0f;

		GameObject& ball;
		Trigger& left_trigger;
		Trigger& right_trigger;
		GameObject& right_padle;
		GameObject& left_padle;

		void setBall() {
			ball.pos = vec2(Globals::screen_width / 2.0f, Globals::screen_height / 2.0f);
			vec2 bal_vel = vec2(static_cast<float>(rand() - RAND_MAX / 2), static_cast<float>(rand() - RAND_MAX / 2));
			if (abs(bal_vel.x) < abs(bal_vel.y)) {
				std::swap(bal_vel.x, bal_vel.y);
			}
			bal_vel = bal_vel.normalized() * ball_speed;

			ball.velocity = bal_vel;
			ball.getScript<BallInfo>().expect("ball info not found").last_hit = nullptr;
		}

		void checkWin() const {
			if (left_score_val == 10) {
				load_win_pong(true, with_ai);
			}
			else if (right_score_val == 10) {
				load_win_pong(false, with_ai);
			}
		}

		void left_scored(GameObject& other) {
			if (ball != other) fmt::println("scored, but it wasn't the ball");
			++left_score_val;
			left_score->update_text(std::to_string(left_score_val));
			setBall();
			checkWin();
		}

		void chang_left_score(int val) {
			left_score_val += val;
			left_score_val = std::max(0, left_score_val);
			left_score->update_text(std::to_string(left_score_val));
			checkWin();
		}
		//TODO: add color animation;
		void chang_right_score(int val) {
			right_score_val += val;
			right_score_val = std::max(0, right_score_val);
			right_score->update_text(std::to_string(right_score_val));
			checkWin();
		}

		void right_scored(GameObject& other) {
			if (ball != other) fmt::println("scored, but it wasn't the ball");
			++right_score_val;
			right_score->update_text(std::to_string(right_score_val));
			setBall();
			checkWin();
		}

		void do_count_down() {
			time += Globals::DeltaTime;
			if (time > 1 && (start_bool & 0b0000'0001)) {
				start_text->update_text("2");
				start_bool -= 0b0000'0001;
			}
			if (time > 2 && (start_bool & 0b0000'0010)) {
				start_text->update_text("1");
				start_bool -= 0b0000'0010;
			}
			if (time > 3 && (start_bool & 0b0000'0100)) {
				start_text->update_text("GO");
				start_bool -= 0b0000'0100;
			}
			if (time > 4 && (start_bool & 0b0000'1000)) {
				gameobject->queueComponentForRemoval(*start_text);
				start_text = nullptr;
				start_bool = 0;
				game_started = true;
				setBall();

				ball.activate();
				left_score->activate();
				right_score->activate();
				right_padle.activate();
				left_padle.activate();

				auto sencery = ObjectManager::get_gameObject("scenery");
				if (sencery) {
					sencery.unwrap().activate();
				}
				else fmt::println("scenery not found");

			}
		}

		void spawn_powerup() {
			int scaledMin = static_cast<int>(min_time_to_next_spawn * 100);
			int scaledMax = static_cast<int>(max_time_to_next_spawn * 100);
			int randInt = (rand() % (scaledMax - scaledMin + 1) + scaledMin);
			time_to_next_spawn = static_cast<float>(randInt / 100.0) + time_to_next_spawn;

			float x = (rand() % static_cast<int>(powerup_spawn_range.x - powerup_size)) - ((powerup_spawn_range.x - powerup_size) / 2.0f) + (Globals::screen_width / 2.0f);
			float y = (rand() % static_cast<int>(powerup_spawn_range.y - powerup_size)) - ((powerup_spawn_range.y - powerup_size) / 2.0f) + (Globals::screen_height / 2.0f);
			GameObject* powerup = new GameObject({ x,y });

			int type = (rand() % 100);

			if (type < 25) {
				type = static_cast<int>(PowerUpManiger::PowerUpType::Speed);
			}
			else if (type < 50) {
				type = static_cast<int>(PowerUpManiger::PowerUpType::Health);
			}
			else if (type < 80) {
				type = static_cast<int>(PowerUpManiger::PowerUpType::Damage);
			}
			else {
				type = static_cast<int>(PowerUpManiger::PowerUpType::Size);
			}

			SpriteRenderer* sprite = new SpriteRenderer({ 6,4 }, { 0, type }, {}, "assets/spinning orb/sprite_sheet_48x48.png", Core::NPC);
			PowerUpManiger* pow = new PowerUpManiger(*sprite, static_cast<PowerUpManiger::PowerUpType>(type));

			pow->change_left.AddListener(this, [this](int val) {chang_left_score(val); });
			pow->change_right.AddListener(this, [this](int val) {chang_right_score(val); });

			powerup->initializeWithComponent(*new Trigger({ static_cast<float>(powerup_size),static_cast<float>(powerup_size) }, {}, "trigger"));
			powerup->initializeWithComponent(*sprite);
			powerup->initializeWithComponent(*pow);

			ObjectManager::queueGameObject(*powerup);
		}

	public:
		static float ball_speed;
		static const float max_ball_speed;

		static float min_time_to_next_spawn;
		static float max_time_to_next_spawn;

		static const vec2 powerup_spawn_range;
		static const int powerup_size;


		PongController(GameObject& ballObj, Trigger& leftTrig, Trigger& rightTrig, GameObject& leftPadle, GameObject& RightPadle, bool a_with_ai) : ball(ballObj), left_trigger(leftTrig), right_trigger(rightTrig), right_padle(RightPadle), left_padle(leftPadle) , with_ai(a_with_ai){}



		void Start() override {

			start_text = new Core::TextRenderer({ Globals::screen_width / 2.0f, Globals::screen_height / 2.0f }, "3", 75, Color::WHITE, Core::CENTER, Core::UI);
			left_score = new Core::TextRenderer({ Globals::screen_width / 4.0f, 35 }, "0", 60, Color::WHITE, Core::TOPCENTER, Core::BackGround, false);
			right_score = new Core::TextRenderer({ Globals::screen_width / 4.0f * 3, 35 }, "0", 60, Color::WHITE, Core::TOPCENTER, Core::BackGround, false);

			gameobject->injectComponent(*start_text);
			gameobject->injectComponent(*left_score);
			gameobject->injectComponent(*right_score);

			left_trigger.onTriggerEnter.AddListener(this, [this](GameObject& other) {right_scored(other);});
			right_trigger.onTriggerEnter.AddListener(this, [this](GameObject& other) {left_scored(other);});

			if (auto AI = left_padle.getScript<PongAI>()) {
				auto* AI_script = &AI.unwrap();
				left_trigger.onTriggerEnter.AddListener(&left_padle, [AI_script](GameObject& other) { if (!AI_script) return;
					AI_script->PredictTargetY();});
			}
		};

		void Stop() override {
			left_trigger.onTriggerEnter.RemoveListener(this);
			right_trigger.onTriggerEnter.RemoveListener(this);
		};


		void Update() override {
			if (!game_started) {
				do_count_down();
			}
			else {
				if (time_to_next_spawn <= 0) {

				}
				time_to_next_spawn -= Globals::DeltaTime;
			}
		}
	};

	
}

