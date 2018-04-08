#pragma once
#include "../../shared/HeldCommand.h"
#include "ClientCommand.h"
class ShowSystemClientCommand :
	virtual public HeldCommand,
	virtual public ClientCommand {
public:
	ShowSystemClientCommand();
	~ShowSystemClientCommand();
	void execute();
};

