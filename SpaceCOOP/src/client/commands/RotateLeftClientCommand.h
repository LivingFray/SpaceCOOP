#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class RotateLeftClientCommand :
	public HeldCommand,
	public ClientCommand {
public:
	RotateLeftClientCommand();
	~RotateLeftClientCommand();
	void execute();
};

