#pragma once
#include "ClientCommand.h"
class ShowConsoleCommand:
	public ClientCommand {
public:
	ShowConsoleCommand();
	~ShowConsoleCommand();
	void execute();
};

