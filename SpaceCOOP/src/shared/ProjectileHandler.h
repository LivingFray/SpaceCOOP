#pragma once
#include "Projectile.h"
#include <functional>
#include <memory>
#include <array>

#define MAX_PROJECTILES 1<<(8*sizeof(sf::Uint8))

using std::shared_ptr;
using std::function;

class ProjectileHandler {
public:
	ProjectileHandler();
	~ProjectileHandler();
	shared_ptr<Projectile> getProjectile(sf::Uint8 type);
	void registerProjectile(function<shared_ptr<Projectile>()> c);
private:
	std::array<function<shared_ptr<Projectile>()>, MAX_PROJECTILES> projectiles;
};

