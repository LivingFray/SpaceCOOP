#pragma once
#include <array>
#include <memory>
#include <functional>
#include "entities/EntityCore.h"

using std::shared_ptr;
using std::function;

class EntityHandler {
public:
	EntityHandler();
	~EntityHandler();
	shared_ptr<EntityCore> getEntity(EntityType type);
	void registerEntity(function<shared_ptr<EntityCore>()> c);
private:
	std::array<function<shared_ptr<EntityCore>()>, MAX_ENTITIES> entities;
};

