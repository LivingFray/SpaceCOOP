#pragma once
#include "ClientCommand.h"
#include "..\shared\HeldCommand.h"
class ForwardsClientCommand:
	virtual public ClientCommand,
	virtual public HeldCommand{
public:
	ForwardsClientCommand();
	~ForwardsClientCommand();
	void execute();
};

