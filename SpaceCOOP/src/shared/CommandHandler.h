#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <array>
#include "Command.h"

using std::shared_ptr;

class CommandHandler {
public:
	CommandHandler();
	~CommandHandler();
	shared_ptr<Command> getCommand(CommandID id);
	shared_ptr<Command> getCommand(std::string);
	void registerCommand(shared_ptr<Command> c, CommandID id);
private:
	std::array<shared_ptr<Command>, MAX_COMMANDS> commands;
	std::map<std::string, shared_ptr<Command>> namedCommands;
};

