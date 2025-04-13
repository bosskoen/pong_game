#pragma once
#include "ObjectManager.h"
#include "Event.hpp"

using namespace Core;

namespace Scripts {
	class MyScrip : public virtual Core::IScript
	{
	public:
		void Start() override;
		void Stop() override;
		void Update() override;
	};

	class Button : public virtual Core::IScript
	{
	private:
		enum state { normal, hover, pressed };

		TextRenderer* text{ nullptr };
		BoxRenderer* box{ nullptr };
		vec2 size{ 0 }, top{}, botem{};
		vec2 offset{};
		int font_size{ 0 };
		string text_str{ "" };
		state button_state{ normal };
		Color::HSL color{ Color::WHITE };

		static constexpr int marginX = 4;
		static constexpr int marginY = 4;

		Color::HSL cahnge_color(float dim_bright) const {
			Color::HSL c = color;
			c.l *= (1.0f - dim_bright);
			return c;
		}
	
	public:
		Event<> onClick;

		Button(vec2 a_size, vec2 a_offset, string a_text, Color a_color);
		Button(int a_font_size, vec2 a_offset, string a_text, Color a_color);

		void Start() override;
		void Stop() override;
		void Update() override;

	};
};

 