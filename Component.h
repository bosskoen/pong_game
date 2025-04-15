#pragma once

namespace Core
{
	class GameObject;

	/// <summary>
	/// a base class for all component, thing a game object can hold.
	/// </summary>
	class Component {
	public:
		bool markedForDelete{ false };
		GameObject* gameobject{ nullptr };
		virtual ~Component() = default;
	};
}
