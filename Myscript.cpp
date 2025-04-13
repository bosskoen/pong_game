#include "Myscript.h"

namespace Scripts {


	void Button::Update() {
		vec2int mouse_pos = Input::mousePos();
		if (mouse_pos.x > top.x && mouse_pos.x < botem.x && mouse_pos.y > top.y && mouse_pos.y < botem.y) {
			if (button_state < hover) {
				Color c = cahnge_color(0.2f);

				text->update_color(c);
				box->update_color(c);
				button_state = hover;
			}

			if (Input::GetMouseDown(MouseButton::Left)) {
				Color c = cahnge_color(0.4f);
				text->update_color(c);
				box->update_color(c);
				button_state = pressed;
			}

			if (Input::GetMouseUp(MouseButton::Left)) {
				Color c = cahnge_color(0.2f);
				text->update_color(c);
				box->update_color(c);
				button_state = hover;
				onClick.Invoke();
			}
		}
		else {
			if (button_state != normal) {
				text->update_color(color);
				box->update_color(color);
				button_state = normal;
			}
		}
	}
	void Button::Stop() {
		gameobject->removeComponent(*box);
		gameobject->removeComponent(*text);
		box = nullptr;
		text = nullptr;
	}
	void Button::Start() {
		top = { gameobject->pos.x - size.x / 2.0f + offset.x ,-gameobject->pos.y - size.y / 2.0f + offset.y };
		botem = { size.x / 2.0f + gameobject->pos.x + offset.x,size.y / 2.0f + gameobject->pos.y + offset.y };

		box = new BoxRenderer(size, offset, color, Core::UI);
		text = new TextRenderer(offset, text_str, font_size, color, Core::CENTER, Core::UI);
		gameobject->injectComponent(*box);
		gameobject->injectComponent(*text);
	}

	Button::Button(int a_font_size, vec2 a_offset, string a_text, Color a_color) : font_size(a_font_size), offset(a_offset), text_str(a_text), color(a_color) {
		float y = static_cast<float>(font_size + marginY * 2);
		float x = static_cast<float>((text_str.length() - 1) * (6.0 / 5.0) * font_size + font_size + marginX * 2);
		size = { x, y };
	}

	Button::Button(vec2 a_size, vec2 a_offset, string a_text, Color a_color) : size(a_size), offset(a_offset), text_str(a_text), color(a_color) {
		int max_fond_y = static_cast<int>(size.y - 2 * marginY);
		int max_fond_x = static_cast<int>((size.x - 2 * marginX) / ((6.0f / 5.0) * (text_str.length() - 1) + 1));
		font_size = std::min(max_fond_y, max_fond_x);
	}
}