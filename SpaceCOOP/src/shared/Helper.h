#pragma once
#include <SFML/Network.hpp>
//Various helper functions that aren't deserving of their own class
extern void concatPackets(sf::Packet& primary, sf::Packet& extra);

extern inline float toRadians(float deg);
extern inline float toDegrees(float rad);
extern void bv2rgb(float &r, float &g, float &b, float bv);
extern float randF(float min, float max);
extern bool onLeft(sf::Vector2f origin, sf::Vector2f direction, sf::Vector2f point);
extern sf::Vector2f normalise(sf::Vector2f);

#define REGCMD(c) { \
std::function<shared_ptr<Command>()> fun = []() { return std::make_shared<c>();}; \
commandHandler.registerCommand(fun); \
}

#define REGENT(e) { \
std::function<shared_ptr<EntityCore>()> fun = []() { return std::make_shared<e>();}; \
entityHandler.registerEntity(fun); \
}

#define REGPROJ(p) { \
std::function<shared_ptr<Projectile>()> fun = []() { return std::make_shared<p>();}; \
projectileHandler.registerProjectile(fun); \
}