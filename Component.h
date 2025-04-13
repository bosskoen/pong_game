#pragma once

namespace Core
{
	class GameObject;
	class Component {
	public:
		bool markedForDelete{ false };
		GameObject* gameobject{ nullptr };
		virtual ~Component() = default;
	};
}
