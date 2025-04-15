#pragma once
#include "Component.h"

namespace Core{

class IScript : public virtual Component
{
public:
	
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Stop() = 0;

	virtual ~IScript() = default;
};
}

