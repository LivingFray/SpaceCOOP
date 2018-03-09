#include "ClientCommand.h"



ClientCommand::ClientCommand() {
}


ClientCommand::~ClientCommand() {
}

void ClientCommand::executeRemote() {
	//TODO: UDP/TCP specifiers for commands
	client->sendCommand(this);
}
