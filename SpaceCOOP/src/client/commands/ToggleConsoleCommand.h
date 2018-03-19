#pragma once
#include "ClientCommand.h"
class ToggleConsoleCommand:
	public ClientCommand {
public:
	ToggleConsoleCommand();
	~ToggleConsoleCommand();
	void execute();
};

