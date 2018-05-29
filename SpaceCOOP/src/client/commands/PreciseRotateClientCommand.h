#pragma once
#include "ClientCommand.h"
class PreciseRotateClientCommand :
	public ClientCommand {
public:
	PreciseRotateClientCommand();
	~PreciseRotateClientCommand();

	void execute();
	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;

	float angle;
	static double time;
	static double lastRotateCommand;
	const double sendRate = 1.0 / 15.0;
};

