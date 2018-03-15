#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class StrafeLeftClientCommand :
	virtual public HeldCommand,
	virtual public ClientCommand {
public:
	StrafeLeftClientCommand();
	~StrafeLeftClientCommand();
	void execute();
};

