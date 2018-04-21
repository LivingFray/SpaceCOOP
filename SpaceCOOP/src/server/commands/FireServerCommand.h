#pragma once
#include "PlayerCommand.h"
class FireServerCommand :
	public PlayerCommand {
public:
	FireServerCommand();
	~FireServerCommand();

	void execute();
};

