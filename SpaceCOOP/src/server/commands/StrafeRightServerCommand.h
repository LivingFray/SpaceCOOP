#pragma once
#include "../../shared/HeldCommand.h"
#include "PlayerCommand.h"
class StrafeRightServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	StrafeRightServerCommand();
	~StrafeRightServerCommand();
	void execute();
};

