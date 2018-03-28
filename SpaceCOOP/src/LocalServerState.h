#pragma once
#include "State.h"
#include "client/Client.h"
#include "server/Server.h"
class LocalServerState :
	public State {
public:
	LocalServerState();
	~LocalServerState();

	void onActivation();

	void update(double dt);
	void draw();
	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	void startServer(unsigned short port);
	void connect();

private:
	Server server;
	Client client;
};

