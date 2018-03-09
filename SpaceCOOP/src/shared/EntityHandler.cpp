#include "EntityHandler.h"



EntityHandler::EntityHandler() {
}


EntityHandler::~EntityHandler() {
}

shared_ptr<EntityCore> EntityHandler::getEntity(EntityType type) {
	if (type >= entities.size() || type < 0) {
		throw std::exception("Command not recognised");
	}
	//Create new entity
	auto e = entities[type]();
	//Ensure type is set
	e->type = type;
	return e;
}

void EntityHandler::registerEntity(function<shared_ptr<EntityCore>()> c) {
	EntityType type = c()->type;
	if (type >= entities.size() || type < 0) {
		throw std::exception("Entity ID out of range");
	}
	if (entities[type]) {
		throw std::exception("Entity already registered to ID " + type);
	}
	entities[type] = c;
}
