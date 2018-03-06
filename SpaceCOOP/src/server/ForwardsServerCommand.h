#pragma once
#include "../shared/HeldCommand.h"
#include "PlayerCommand.h"
#include "Player.h"
class ForwardsServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	ForwardsServerCommand();
	~ForwardsServerCommand();
	void execute();
};

