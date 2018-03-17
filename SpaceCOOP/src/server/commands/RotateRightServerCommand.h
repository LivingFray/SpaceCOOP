#pragma once
#include "../../shared/HeldCommand.h"
#include "PlayerCommand.h"
class RotateRightServerCommand :
	public HeldCommand,
	public PlayerCommand {
public:
	RotateRightServerCommand();
	~RotateRightServerCommand();
	void execute();
	float amount = 10.0f;
};

