#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class StrafeRightClientCommand :
	virtual public HeldCommand,
	virtual public ClientCommand {
public:
	StrafeRightClientCommand();
	~StrafeRightClientCommand();
	void execute();
};

