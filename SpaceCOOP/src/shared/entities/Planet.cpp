#include "Planet.h"



Planet::Planet(): Planet(1.0f) {
}

Planet::Planet(float radius): EntityCore() {
	width = radius * 2;
	height = radius * 2;
	loadSpriteAndResize("assets/tempplanet.png");
	type = 1;
}


Planet::~Planet() {
}
