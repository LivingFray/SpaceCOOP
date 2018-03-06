#pragma once
#include <memory>
#include "../shared/HeldCommand.h"
#include "Player.h"
#include "PlayerCommand.h"
class StrafeRightServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	StrafeRightServerCommand();
	~StrafeRightServerCommand();
	void execute();
};

