#include "renderer.h"
#include "GameObject.h"
#include "ObjectManager.h"


namespace Core {
	RectRenderer::RectRenderer(vec2 a_size, Tmpl8::vec2 a_ofset, Color a_color, RenderLayer a_layer, bool a_active) : size(a_size), color(a_color) { this->ofset = a_ofset; this->layer = a_layer; active = a_active; was_active = a_active; }

	void RectRenderer::draw(Surface& screen) {
		float x = gameobject->pos.x + ofset.x, y = gameobject->pos.y + ofset.y;
		screen.Bar(static_cast<int>(x - size.x / 2), static_cast<int>(y - size.y / 2),
			static_cast<int>(x + size.x / 2), static_cast<int>(y + size.y / 2), color.ToInt());

	}
	void RectRenderer::update_color(Color a_color)
	{
		color = a_color;
	}
	BoxRenderer::BoxRenderer(vec2 a_size, vec2 a_ofset, Color a_color, RenderLayer a_layer, bool a_active) : size(a_size), color(a_color) {
		this->ofset = a_ofset; this->layer = a_layer; active = a_active; was_active = a_active;
	}

	void BoxRenderer::draw(Surface& screen)
	{
		float x = gameobject->pos.x + ofset.x, y = gameobject->pos.y + ofset.y;
		screen.Box(static_cast<int>(x - size.x / 2), static_cast<int>(y - size.y / 2),
			static_cast<int>(x + size.x / 2), static_cast<int>(y + size.y / 2), color.ToInt());
	}
	void BoxRenderer::update_color(Color a_color)
	{
		color = a_color;
	}
	RenderLayer IRenderer::get_layer() const
	{
		return layer;
	}
	void IRenderer::set_layer(RenderLayer new_layer)
	{
		layer = new_layer;
		ObjectManager::reorder_renderlist = true;
	}
	void IRenderer::activate()
	{
		was_active = true;
		if (gameobject->isActive()) {
			active = true;
		}
	}
	void IRenderer::deactivate(bool set_by_gameobject)
	{
		if (!set_by_gameobject) {
			was_active = false;
		}
		else {
			was_active = active;
		}
		active = false;
	} // TODO Fix bug - calling deactivate twice causes was_active to be false, even if it shouldn't be.

	TextRenderer::TextRenderer(vec2 ofset, const string& test, int pxl, Color color, Alingment aling, RenderLayer layer, bool a_active)
	{
		this->ofset = ofset;
		set_text(test, pxl, color);
		this->layer = layer;
		alingment = aling;
		active = a_active;
		was_active = a_active;
	}
	void TextRenderer::rerender_text() {
		if (pxl % 5 == 0) {
			// use printScaled function to generate Text natively
			int fond_div_down = pxl / 5;
			int width = ((static_cast<int>(text_string.length()) - 1) * 6 + 5) * fond_div_down;
			int height = 5 * fond_div_down;

			this->text = Surface{ width, height };
			this->text.Clear(0);
			this->text.PrintScaled(text_string.c_str(), 0, 0, fond_div_down, fond_div_down, color.ToInt());
		}
		else {
			// Use printScaled to generate text at the closest native size, then scale it to the target size.
			bool upscale = pxl > 10;

			int chosenSize = upscale ? pxl / 5 : pxl / 5 + 1;

			// Generate text at the closest multiple of 5
			Surface temp{ static_cast<int>((text_string.length() - 1) * 6 * chosenSize + 5 * chosenSize), 5 * chosenSize };
			temp.Clear(0);
			temp.PrintScaled(text_string.c_str(), 0, 0, chosenSize, chosenSize, color.ToInt());

			// Resize to the exact required size
			this->text = Surface{ static_cast<int>((text_string.length() - 1) * (6.0 / 5.0) * pxl + pxl), pxl };
			this->text.Resize(&temp);
		}
		need_update = false;
	}
	void TextRenderer::draw(Surface& screen)
	{
		if (need_update) rerender_text();

		float x = gameobject->pos.x + ofset.x, y = gameobject->pos.y + ofset.y;
		switch (alingment)
		{
		case TOPLEFT:
			text.CopyTo(&screen, static_cast<int> (x), static_cast<int>(y));
			break;
		case TOPCENTER:
			text.CopyTo(&screen, static_cast<int>(x - text.GetWidth() / 2.0), static_cast<int>(y));
			break;
		case TOPRIGHT:
			text.CopyTo(&screen, static_cast<int>(x - text.GetWidth()), static_cast<int> (y));
			break;
		case CENTERLEFT:
			text.CopyTo(&screen, static_cast<int>(x), static_cast<int> (y - text.GetHeight() / 2.0));
			break;
		case CENTER:
			text.BlendCopyTo(&screen, static_cast<int>(x - text.GetWidth() / 2.0), static_cast<int>(y - text.GetHeight() / 2.0));
			break;
		case CENTERRIHGHT:
			text.CopyTo(&screen, static_cast<int>(x - text.GetWidth()), static_cast<int> (y - text.GetHeight() / 2.0));
			break;
		case BOTTEMLEFT:
			text.CopyTo(&screen, static_cast<int>(x), static_cast<int> (y - text.GetHeight()));
			break;
		case BOTTEMCENTER:
			text.CopyTo(&screen, static_cast<int>(x - text.GetWidth() / 2.0), static_cast<int>(y - text.GetHeight()));
			break;
		case BOTTEMRIGHT:
			text.CopyTo(&screen, static_cast<int>(x - text.GetWidth()), static_cast<int>(y - text.GetHeight()));
			break;
		}
	}
	void TextRenderer::set_text(const string& text, int pxl, Color color)
	{

		this->text_string = text;
		this->pxl = pxl;
		this->color = color;
		need_update = true;
	}
	void TextRenderer::update_text(const string& text)
	{
		set_text(text, pxl, color);
	}
	void TextRenderer::update_color(Color a_color)
	{
		set_text(text_string, pxl, a_color);
	}

	void TextRenderer::update_scale(int new_pxl)
	{
		set_text(text_string, new_pxl, color);
	}

	SpriteRenderer::SpriteRenderer(vec2int a_frame_count, vec2int frame, vec2 a_ofset, const string& a_sprite_map, RenderLayer a_layer, bool a_active)
	{
		this->sprite_map = ResourceManager::LoadImage(a_sprite_map.c_str());
		if (this->sprite_map == nullptr) Panic("Sprite map not found");

		// Validate that the sprite map can hold the specified number of frames
		if (a_frame_count.x < 1 || a_frame_count.y < 1) Panic("Frame count must be greater than 0");
		if (sprite_map->GetWidth() % a_frame_count.x != 0 || sprite_map->GetHeight() % a_frame_count.y != 0) Panic("Frame count must be a multiple of the sprite map");

		frame_count = a_frame_count;

		this->frame_size = { sprite_map->GetWidth() / frame_count.x,sprite_map->GetHeight() / frame_count.y };

		this->ofset = a_ofset;
		this->layer = a_layer;

		active = a_active;
		was_active = a_active;
		set_frame(frame);
	}
	void SpriteRenderer::draw(Surface& screen)
	{

		int x_pos = static_cast<int>(gameobject->pos.x + ofset.x - frame_size.x / 2.0);
		int y_pos = static_cast<int>(gameobject->pos.y + ofset.y - frame_size.y / 2.0);

		Pixel* screen_buffer = screen.GetBuffer();
		Pixel* image_buffer = sprite_map->GetBuffer();

		// validate draw position
		if (!screen_buffer || !image_buffer) return;

		if (x_pos >= screen.GetWidth() || y_pos >= screen.GetHeight() ||
			x_pos + frame_size.x <= 0 || y_pos + frame_size.y <= 0) return;

		int draw_x = std::max(0, x_pos);
		int draw_y = std::max(0, y_pos);
		int end_x = std::min(screen.GetWidth(), x_pos + frame_size.x);
		int end_y = std::min(screen.GetHeight(), y_pos + frame_size.y);

		int start_x = std::max(0, -x_pos);
		int start_y = std::max(0, -y_pos);

		int lines_to_draw = end_y - draw_y;
		int pixels_per_line = end_x - draw_x;

		screen_buffer += draw_x + draw_y * screen.GetWidth();
		image_buffer += start_in_buffer + start_x + start_y * sprite_map->GetWidth();

		for (int y = 0; y < lines_to_draw; y++) {
			for (int x = 0; x < pixels_per_line; x++) {
				//Only render pixels with sufficient alpha transparency
				if ((image_buffer[x] >> 24) > 127) screen_buffer[x] = image_buffer[x];
			}
			screen_buffer += screen.GetWidth();
			image_buffer += sprite_map->GetWidth();
		}
	}
	void SpriteRenderer::set_frame(vec2int new_frame)
	{
		if (frame_count.x - 1 < new_frame.x || frame_count.y - 1 < new_frame.y) Panic("Frame out of bounds");
		current_frame = new_frame;
		start_in_buffer = current_frame.x * frame_size.x + current_frame.y * frame_size.y * sprite_map->GetWidth();
	}
	void SpriteRenderer::set_frame(int v) {
		if (v > frame_count.x * frame_count.y - 1) Panic("Frame out of bounds");
		set_frame({ v % frame_count.x, v / frame_count.x });
	}
}