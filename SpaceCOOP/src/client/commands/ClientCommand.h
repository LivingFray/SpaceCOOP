#pragma once
#include "../../shared/Command.h"
#include "../Client.h"

class Client;

class ClientCommand :
	virtual public Command {
public:
	ClientCommand();
	virtual ~ClientCommand();
	Client* client;
protected:
	void executeRemote();
};

