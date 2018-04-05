#include "EntityHandler.h"



EntityHandler::EntityHandler() {
}


EntityHandler::~EntityHandler() {
}

shared_ptr<EntityCore> EntityHandler::getEntity(EntityType type) {
	if (type >= entities.size() || type < 0 || !entities[type]) {
		throw "Entity type not registered: " + type;
	}
	//Create new entity
	auto e = entities[type]();
	//Ensure type is set
	e->type = type;
	return e;
}

void EntityHandler::registerEntity(function<shared_ptr<EntityCore>()> c) {
	auto inst = c();
	EntityType type = inst->type;
	if (type >= entities.size() || type < 0) {
		throw "Entity ID out of range";
	}
	if (entities[type]) {
		throw "Entity already registered to ID " + type;
	}
	entities[type] = c;
}
