#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <array>
#include "Command.h"

class CommandHandler {
public:
	CommandHandler();
	~CommandHandler();
	std::shared_ptr<Command> getCommand(CommandID id);
	std::shared_ptr<Command> getCommand(std::string);
	void registerCommand(std::shared_ptr<Command> c, CommandID id);
private:
	std::array<std::shared_ptr<Command>, MAX_COMMANDS> commands;
	std::map<std::string, std::shared_ptr<Command>> namedCommands;
};

