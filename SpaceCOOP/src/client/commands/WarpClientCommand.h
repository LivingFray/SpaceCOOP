#pragma once
#include "ClientCommand.h"
class WarpClientCommand :
	public ClientCommand {
public:
	WarpClientCommand();
	~WarpClientCommand();
	int destination;
	void parseString(std::string cmd);
	void execute();

	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;
};

