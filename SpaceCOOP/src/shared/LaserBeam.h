#pragma once
#include "Projectile.h"
class LaserBeam :
	public Projectile {
public:
	LaserBeam();
	~LaserBeam();

	virtual void onCreation();
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
