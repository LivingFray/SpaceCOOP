#pragma once
#include <string>
#include <SFML\Network.hpp>

//Set to the highest id a command may have, default is based on size of CommandID
#define MAX_COMMANDS 1<<(8*sizeof(CommandID))

//Seriously VS? You display a warning for perfectly valid code because it is not a commonly used feature?
#pragma warning( disable : 4250)

typedef sf::Uint8 CommandID;

class Command {
public:
	Command();
	virtual ~Command();
	CommandID id;

	//The string name of the command
	std::string name;

	//Parses a command from the console and executes it
	virtual void parseString(std::string cmd) { execute(); };
	//Directly executes a command
	virtual void execute() {};

	virtual sf::Packet& packetIn(sf::Packet& packet);
	virtual sf::Packet& packetOut(sf::Packet& packet) const;

	friend sf::Packet& operator<<(sf::Packet& packet, const Command& command);
	friend sf::Packet& operator>>(sf::Packet& packet, Command& command);
};
