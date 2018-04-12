#pragma once
#include "State.h"
#include "client/Client.h"
#include <string>
class ClientState :
	public State {
public:
	ClientState();
	~ClientState();

	void onActivation();

	void update(double dt);
	void draw();
	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);
	void mouseEvent(sf::Event e);

	void connect(std::string ip, unsigned short port);

private:
	Client client;
};

