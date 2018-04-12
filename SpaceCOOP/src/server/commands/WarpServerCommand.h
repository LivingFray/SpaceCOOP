#pragma once
#include "PlayerCommand.h"
class WarpServerCommand :
	public PlayerCommand {
public:
	WarpServerCommand();
	~WarpServerCommand();
	int destination;
	void execute();

	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;
};

