#pragma once
#include <SFML/Network.hpp>
//Various helper functions that aren't deserving of their own class
void concatPackets(sf::Packet& primary, sf::Packet& extra);
