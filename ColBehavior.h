#pragma once
#include "Collider_interface.h"

using Core::ColDir; using Core::IAABB;

namespace PreBuild {
	void DefaultCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment);
	/// <summary>
	/// perfect elastic collisions.
	/// </summary>
	void PerfecBounceCollision(IAABB& other, IAABB& static_collider, ColDir direction, float displasment);
}