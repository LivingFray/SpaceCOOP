#include "ServerGalaxy.h"
#include "../shared/Helper.h"
#include <cstdlib>

using std::shared_ptr;

ServerGalaxy::ServerGalaxy() {
}


ServerGalaxy::~ServerGalaxy() {
}

void ServerGalaxy::generateGalaxy() {
	//Test generator, real thing will hopefully have some semblance of balance
	for (int i = 0; i < 150; i++) {
		shared_ptr<Star> s(new Star());
		s->position.x = randF(0.0f, 1200.0f);
		s->position.y = randF(0.0f, 700.0f);
		s->radius = randF(10.0f, 20.0f);
		float r, g, b;
		bv2rgb(r, g, b, randF(-0.4f, 2.0f));
		s->colour.r = static_cast<sf::Uint8>(r * 255.0f);
		s->colour.g = static_cast<sf::Uint8>(g * 255.0f);
		s->colour.b = static_cast<sf::Uint8>(b * 255.0f);
		stars.push_back(s);
	}
}
