#pragma once
#include "EntityCore.h"
class Planet :
	public EntityCore {
public:
	Planet();
	Planet(float radius);
	virtual ~Planet();
};

