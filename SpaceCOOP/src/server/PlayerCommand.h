#pragma once
#include "Player.h"
#include "../shared/Command.h"

class PlayerCommand:
virtual public Command {
public:
	PlayerCommand();
	virtual ~PlayerCommand();
	Player* player;
};

