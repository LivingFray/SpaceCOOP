#pragma once
#include "../../shared/HeldCommand.h"
#include "PlayerCommand.h"
class StrafeLeftServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	StrafeLeftServerCommand();
	~StrafeLeftServerCommand();
	void execute();
};

