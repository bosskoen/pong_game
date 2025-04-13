#include "Color.h"
#include <algorithm>
namespace Util {
	Color::Color(int hex) {
		r = (hex >> 16) & 0xFF;
		g = (hex >> 8) & 0xFF;
		b = hex & 0xFF;
	}

	unsigned int Color::ToInt() const {
		return (r << 16) | (g << 8) | b;
	}

    /*
     * Converts an RGB color value to HSL. Conversion formula
     * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
     * Assumes r, g, and b are contained in the set [0, 255] and
     * returns HSL in the set [0, 1].
     * 
     * https://gist.github.com/ciembor/1494530
     */
    Color::HSL Color::ToHSL() const
    {
        HSL result;

        float r_norm = r/255.0f;
        float g_norm = g/255.0f;
        float b_norm = b/255.0f;

        float max = fmaxf(fmaxf(r_norm, g_norm), b_norm);
        float min = fminf(fminf(r_norm, g_norm), b_norm);

        result.h = result.s = result.l = (max + min) / 2;

        if (max == min) {
            result.h = result.s = 0; // achromatic
        }
        else {
            float d = max - min;
            result.s = (result.l > 0.5) ? d / (2 - max - min) : d / (max + min);

            if (max == r_norm) {
                result.h = (g_norm - b_norm) / d + (g_norm < b_norm ? 6 : 0);
            }
            else if (max == g_norm) {
                result.h = (b_norm - r_norm) / d + 2;
            }
            else if (max == b_norm) {
                result.h = (r_norm - g_norm) / d + 4;
            }

            result.h /= 6;
        }

        return result;
    }

	constexpr Color Color::BLACK(1, 1, 1);
	constexpr Color Color::WHITE(255, 255, 255);
	constexpr Color Color::RED(255, 0, 0);
	constexpr Color Color::GREEN(0, 255, 0);
	constexpr Color Color::BLUE(0, 0, 255);
	constexpr Color Color::YELLOW(255, 255, 0);
	constexpr Color Color::CYAN(0, 255, 255);
	constexpr Color Color::MAGENTA(255, 0, 255);

    /*
     * Converts an HUE to r, g or b.
     * returns float in the set [0, 1].
     * 
     * https://gist.github.com/ciembor/1494530
     */
    float hue2rgb(float p, float q, float t) {

        if (t < 0)
            t += 1;
        if (t > 1)
            t -= 1;
        if (t < 1. / 6)
            return p + (q - p) * 6 * t;
        if (t < 1. / 2)
            return q;
        if (t < 2. / 3)
            return p + (q - p) * (2.0f / 3 - t) * 6;

        return p;
    }

    /*
     * Converts an HSL color value to RGB. Conversion formula
     * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
     * Assumes h, s, and l are contained in the set [0, 1] and
     * returns RGB in the set [0, 255].
     * 
     * https://gist.github.com/ciembor/1494530
     */
	Color Color::HSL::ToColor() const
	{
        Color result;

        if(0 == s) {
        result.r = result.g = result.b = static_cast<uint8_t>((l * 255)); // achromatic
        }
        else {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        result.r = static_cast<uint8_t>(hue2rgb(p, q, h + 1.0f/3) * 255);
        result.g = static_cast<uint8_t>(hue2rgb(p, q, h) * 255);
        result.b = static_cast<uint8_t>(hue2rgb(p, q, h - 1.0f/3) * 255);
        }

        return result;
	}

}