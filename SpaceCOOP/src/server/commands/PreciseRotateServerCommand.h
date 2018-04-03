#pragma once
#include "PlayerCommand.h"
class PreciseRotateServerCommand :
	public PlayerCommand {
public:
	PreciseRotateServerCommand();
	~PreciseRotateServerCommand();

	void execute();
	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;

	float angle;
};

