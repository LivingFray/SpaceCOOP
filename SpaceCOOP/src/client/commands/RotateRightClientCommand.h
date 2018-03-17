#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class RotateRightClientCommand :
	public HeldCommand,
	public ClientCommand {
public:
	RotateRightClientCommand();
	~RotateRightClientCommand();
	void execute();
	float amount = 10.0f;
};

