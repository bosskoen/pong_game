#pragma once
#include <cstdint>
namespace Util {
	class Color
	{
	public:
		/// <summary>
		/// a HSL color representation with a range of [0 , 1] .
		/// </summary>
		class HSL {
		public:
			float h{ 0 }, s{ 0 }, l{ 0 };

			HSL() = default;
			HSL(float a_h, float a_s, float a_l) : h(a_h), s(a_s), l(a_l) {}
			HSL(Color a_color) : HSL(a_color.ToHSL()) {}

			Color ToColor() const;

			operator Color() const
			{
				return ToColor();
			}
		};


		uint8_t r{ 0 }, g{ 0 }, b{ 0 };
		Color() = default;
		constexpr Color(uint8_t a_r, uint8_t a_g, uint8_t a_b): r(a_r),g(a_g),b(a_b){}
		Color(int hex);

		unsigned int ToInt() const;

		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color YELLOW;
		static const Color CYAN;
		static const Color MAGENTA;

		HSL ToHSL() const;

		operator HSL() const { return ToHSL(); }
		operator int() const
		{
			return ToInt();
		}

	};



}

