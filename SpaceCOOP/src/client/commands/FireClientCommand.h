#pragma once
#include "ClientCommand.h"
class FireClientCommand :
	public ClientCommand {
public:
	FireClientCommand();
	~FireClientCommand();

	void execute();
};

