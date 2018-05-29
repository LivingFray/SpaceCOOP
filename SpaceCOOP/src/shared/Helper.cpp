#include "Helper.h"

void concatPackets(sf::Packet& primary, sf::Packet& extra) {
	sf::Uint8 byte;
	while (extra >> byte) {
		primary << byte;
	}
}

inline float toRadians(float deg) {
	return 0.01745329251f * deg;
}

inline float toDegrees(float rad) {
	return 57.2957795457f * rad;
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

float randF(float min, float max) {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r * (max - min) + min;
}

bool onLeft(sf::Vector2f origin, sf::Vector2f direction, sf::Vector2f point) {
	sf::Vector2f pointDir = point - origin;

	return (direction.x * pointDir.y - pointDir.x * direction.y) > 0;
}

sf::Vector2f normalise(sf::Vector2f vec) {
	float mag = vec.x * vec.x + vec.y * vec.y;
	mag = sqrtf(mag);
	return vec / mag;
}