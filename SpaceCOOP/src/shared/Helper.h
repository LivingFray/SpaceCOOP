#pragma once
#include <SFML/Network.hpp>
//Various helper functions that aren't deserving of their own class
extern void concatPackets(sf::Packet& primary, sf::Packet& extra);

extern inline float toDegrees(float rad);


#define REGCMD(c, i) { \
std::function<shared_ptr<Command>()> fun = []() { auto cmd = std::make_shared<c>(); cmd->id = i; return cmd;}; \
commandHandler.registerCommand(fun, i); \
}

#define REGENT(e) { \
std::function<shared_ptr<EntityCore>()> fun = []() { return std::make_shared<e>();}; \
entityHandler.registerEntity(fun); \
}
