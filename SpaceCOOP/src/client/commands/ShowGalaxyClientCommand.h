#pragma once
#include "ClientCommand.h"
class ShowGalaxyClientCommand :
	public ClientCommand {
public:
	ShowGalaxyClientCommand();
	~ShowGalaxyClientCommand();
	void execute();
};

