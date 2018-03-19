#include "CommandHandler.h"
#include "HeldCommand.h"

CommandHandler::CommandHandler() {
}


CommandHandler::~CommandHandler() {
}

shared_ptr<Command> CommandHandler::getCommand(CommandID id) {
	if (id >= commands.size() || id < 0) {
		throw std::exception("Command not recognised");
	}
	auto cmd = commands[id]();
	cmd->id = id;
	return cmd;
}

shared_ptr<Command> CommandHandler::getCommand(std::string name) {
	if (namedCommands.find(name) != namedCommands.end()) {
		return namedCommands[name]();
	}
	return NULL;
}

void CommandHandler::registerCommand(function<shared_ptr<Command>()> c, CommandID id) {
	if (id >= commands.size() || id < 0) {
		throw std::exception("Command ID out of range");
	}
	if (commands[id]) {
		throw std::exception("Command already registered to ID " + id);
	}
	auto cmd = c();
	commands[id] = c;
	namedCommands[cmd->name] = c;
}
