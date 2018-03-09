#pragma once
#include "../../shared/HeldCommand.h"
#include "PlayerCommand.h"
class BackwardsServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	BackwardsServerCommand();
	~BackwardsServerCommand();
	void execute();
};

