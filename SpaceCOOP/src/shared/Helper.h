#pragma once
#include <SFML/Network.hpp>
//Various helper functions that aren't deserving of their own class
extern void concatPackets(sf::Packet& primary, sf::Packet& extra);

extern inline float toRadians(float deg);
extern inline float toDegrees(float rad);
extern void bv2rgb(float &r, float &g, float &b, float bv);
extern float randF(float min, float max);

#define REGCMD(c) { \
std::function<shared_ptr<Command>()> fun = []() { return std::make_shared<c>();}; \
commandHandler.registerCommand(fun); \
}

#define REGENT(e) { \
std::function<shared_ptr<EntityCore>()> fun = []() { return std::make_shared<e>();}; \
entityHandler.registerEntity(fun); \
}
