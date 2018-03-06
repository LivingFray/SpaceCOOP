#pragma once
#include <memory>
#include "../shared/HeldCommand.h"
#include "Player.h"
#include "PlayerCommand.h"
class BackwardsServerCommand :
	virtual public HeldCommand,
	virtual public PlayerCommand {
public:
	BackwardsServerCommand();
	~BackwardsServerCommand();
	void execute();
};

