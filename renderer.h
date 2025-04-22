#pragma once
#include "renderer_interface.h"

#include "Color.h"
#include "ResourceManager.h"
#include "Cloning.h"

using namespace Tmpl8;
using namespace Util;
using std::string;

namespace Core {

	class RectRenderer : public virtual Cloneable<RectRenderer, IRenderer>{
		Color color {Color()};
	public:
		vec2 size;
		RectRenderer() = default;
		RectRenderer(vec2 a_size, vec2 a_ofset, Color a_color, RenderLayer a_layer, bool a_active = true);
		void draw(Surface& screen) override;
		void update_color(Color a_color);
	};

	class BoxRenderer : public virtual Cloneable<BoxRenderer, IRenderer> {
		vec2 size;
		int with{ 0 };
		Color color{ Color() };
		public:
			BoxRenderer() = default;
			BoxRenderer(vec2 a_size, vec2 a_ofset, Color a_color, RenderLayer a_layer, bool a_active = true);
			void draw(Surface& screen) override;
			void update_color(Color a_color);
	};


	enum Alingment {
		TOPLEFT,
		TOPCENTER,
		TOPRIGHT,
		CENTERLEFT,
		CENTER,
		CENTERRIHGHT,
		BOTTEMLEFT,
		BOTTEMCENTER,
		BOTTEMRIGHT,
	};

	class TextRenderer : public virtual Cloneable<TextRenderer, IRenderer> {
	public:

		TextRenderer() = default;
		TextRenderer(vec2 ofset , const string& test, int pxl, Color color, Alingment aling ,RenderLayer layer, bool a_active = true);

		void draw(Surface& screen) override;
		void set_text(const string& text, int pxl, Color color);
		void update_text(const string& text);
		void update_color(Color color);
		void update_scale(int new_pxl);

	private:
		void rerender_text();

		Surface text{ 0,0 };
		int pxl{ 0 };
		Color color{ Color() };
		string text_string{ "" };
		Alingment alingment{ TOPLEFT };
		bool need_update{ true };
	};

	class SpriteRenderer : public virtual IRenderer,public virtual ICloneable<IRenderer> {
	public:
		SpriteRenderer() = default;
		SpriteRenderer(vec2int a_frame_count, vec2int frame, vec2 a_ofset, const string& sprite_map, RenderLayer a_layer, bool a_active = true);
		void draw(Surface& screen) override;
		void set_frame(vec2int new_frame);
		void set_frame(int v);
	private:
		std::shared_ptr<Surface> sprite_map{ nullptr };
		vec2int frame_size{ vec2int() };
		vec2int frame_count { vec2int() };
		vec2int current_frame{ vec2int() };
		int start_in_buffer{ 0 };

		MAKE_CLONEABLE(SpriteRenderer, IRenderer);
	};

}

