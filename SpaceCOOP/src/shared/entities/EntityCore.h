#pragma once
#include <SFML/Network.hpp>

//Set to the highest id a command may have, default is based on size of CommandID
#define MAX_ENTITIES 1<<(8*sizeof(EntityType))

typedef sf::Uint64 UUID;
typedef sf::Uint8 EntityType;
//TODO: Some kind of efficient update send that sends differences

class EntityCore {
public:
	EntityCore();
	virtual ~EntityCore();
	UUID id;
	EntityType type;
	sf::Vector2f pos;
	sf::Vector2f vel;
	friend sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command);
	friend sf::Packet& operator>>(sf::Packet& packet, EntityCore& command);
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
};

