#pragma once
#include "Command.h"
/*
A held command is a command than runs constantly from +CMD being called
until -CMD is called. Keys bound to a held command call +CMD on press
and -CMD on release.
*/
class HeldCommand :
	virtual public Command {
public:
	HeldCommand();
	virtual ~HeldCommand();
	bool held;
	virtual void parseString(std::string cmd);

	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;
};

