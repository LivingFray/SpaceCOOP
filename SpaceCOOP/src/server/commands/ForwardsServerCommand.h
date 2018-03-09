#pragma once
#include "../../shared/HeldCommand.h"
#include "PlayerCommand.h"
class ForwardsServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	ForwardsServerCommand();
	~ForwardsServerCommand();
	void execute();
};

