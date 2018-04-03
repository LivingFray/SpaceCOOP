#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <array>
#include <functional>
#include "Command.h"

using std::shared_ptr;
using std::function;

class CommandHandler {
public:
	CommandHandler();
	~CommandHandler();
	shared_ptr<Command> getCommand(CommandID id);
	shared_ptr<Command> getCommand(std::string);
	void registerCommand(function<shared_ptr<Command>()> c);
private:
	std::array<function<shared_ptr<Command>()>, MAX_COMMANDS> commands;
	std::map<std::string, function<shared_ptr<Command>()>> namedCommands;
};

