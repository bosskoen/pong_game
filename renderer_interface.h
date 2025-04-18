#pragma once
#include "Component.h"
#include "template.h"
#include "surface.h"

using Tmpl8::Surface;
using Tmpl8::vec2;

namespace Core {
	enum RenderLayer {
		Sky,
		BackGround,
		Map,
		NPC,
		Player,
		ForeGround,
		UI
	};

	class IRenderer : public virtual Component
	{
	protected:
		RenderLayer layer{ Sky };
		bool active{ true }, was_active{ true };

	public:

		virtual void draw(Surface& screen) = 0;
		vec2 ofset{ vec2() };
		virtual ~IRenderer() = default;

		RenderLayer get_layer() const;
		void set_layer(RenderLayer new_layer);

		void activate();
		void deactivate(bool set_by_gameobject = false);
		bool isActive() const { return active; };
		bool wasActive() const { return was_active; };
	};
}