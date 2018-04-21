#include "ProjectileHandler.h"



ProjectileHandler::ProjectileHandler() {
}


ProjectileHandler::~ProjectileHandler() {
}

shared_ptr<Projectile> ProjectileHandler::getProjectile(sf::Uint8 type) {
	if (type >= projectiles.size() || type < 0 || !projectiles[type]) {
		throw "Projectile type not registered: " + type;
	}
	//Create new projectile
	auto p = projectiles[type]();
	//Ensure type is set
	p->type = type;
	return p;
}

void ProjectileHandler::registerProjectile(function<shared_ptr<Projectile>()> c) {
	auto inst = c();
	sf::Uint8 type = inst->type;
	if (type >= projectiles.size() || type < 0) {
		throw "Projectile ID out of range";
	}
	if (projectiles[type]) {
		throw "Projectile already registered to ID " + type;
	}
	projectiles[type] = c;
}
