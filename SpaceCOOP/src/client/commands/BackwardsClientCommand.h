#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class BackwardsClientCommand :
	virtual public HeldCommand,
	virtual public ClientCommand {
public:
	BackwardsClientCommand();
	~BackwardsClientCommand();
	void execute();
};

