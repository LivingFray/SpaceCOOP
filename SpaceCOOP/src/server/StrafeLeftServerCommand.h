#pragma once
#include <memory>
#include "../shared/HeldCommand.h"
#include "Player.h"
#include "PlayerCommand.h"
class StrafeLeftServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	StrafeLeftServerCommand();
	~StrafeLeftServerCommand();
	void execute();
};

