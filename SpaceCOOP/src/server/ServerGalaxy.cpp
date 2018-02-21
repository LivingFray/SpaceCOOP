#include "ServerGalaxy.h"
#include <cstdlib>


float randF(float min, float max) {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r * (max - min) + min;
}

//Satanic ritual that turns a stars temperature into RGB
//---------------------------------------------------------------------------
void bv2rgb(float &r, float &g, float &b, float bv)    // RGB <0,1> <- BV <-0.4,+2.0> [-]
{
	float t;  r = 0.0f; g = 0.0f; b = 0.0f; if (bv<-0.4f) bv = -0.4f; if (bv> 2.0f) bv = 2.0f;
	if ((bv >= -0.40f) && (bv<0.00f)) { t = (bv + 0.40f) / (0.00f + 0.40f); r = 0.61f + (0.11f*t) + (0.1f*t*t); } else if ((bv >= 0.00f) && (bv<0.40f)) { t = (bv - 0.00f) / (0.40f - 0.00f); r = 0.83f + (0.17f*t); } else if ((bv >= 0.40f) && (bv<2.10f)) { t = (bv - 0.40f) / (2.10f - 0.40f); r = 1.00f; }
	if ((bv >= -0.40f) && (bv<0.00f)) { t = (bv + 0.40f) / (0.00f + 0.40f); g = 0.70f + (0.07f*t) + (0.1f*t*t); } else if ((bv >= 0.00f) && (bv<0.40f)) { t = (bv - 0.00f) / (0.40f - 0.00f); g = 0.87f + (0.11f*t); } else if ((bv >= 0.40f) && (bv<1.60f)) { t = (bv - 0.40f) / (1.60f - 0.40f); g = 0.98f - (0.16f*t); } else if ((bv >= 1.60f) && (bv<2.00f)) { t = (bv - 1.60f) / (2.00f - 1.60f); g = 0.82f - (0.5f*t*t); }
	if ((bv >= -0.40f) && (bv<0.40f)) { t = (bv + 0.40f) / (0.40f + 0.40f); b = 1.00f; } else if ((bv >= 0.40f) && (bv<1.50f)) { t = (bv - 0.40f) / (1.50f - 0.40f); b = 1.00f - (0.47f*t) + (0.1f*t*t); } else if ((bv >= 1.50f) && (bv<1.94f)) { t = (bv - 1.50f) / (1.94f - 1.50f); b = 0.63f - (0.6f*t*t); }
}
//---------------------------------------------------------------------------

ServerGalaxy::ServerGalaxy() {
}


ServerGalaxy::~ServerGalaxy() {
}

void ServerGalaxy::generateGalaxy() {
	//Test generator, real thing will hopefully have some semblance of balance
	for (int i = 0; i < 150; i++) {
		std::shared_ptr<Star> s(new Star());
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
