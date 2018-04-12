#include "ServerGalaxy.h"
#include "../shared/Helper.h"
#include "Server.h"

using std::shared_ptr;

ServerGalaxy::ServerGalaxy() {
}


ServerGalaxy::~ServerGalaxy() {
}

void ServerGalaxy::generateGalaxy() {
	//Test generator, real thing will hopefully have some semblance of balance
	for (int i = 0; i < 150; i++) {
		shared_ptr<Star> s(new Star());
		s->position.x = randF(0.0f, 1200.0f);
		s->position.y = randF(0.0f, 700.0f);
		s->radius = randF(10.0f, 20.0f);
		float r, g, b;
		bv2rgb(r, g, b, randF(-0.4f, 2.0f));
		s->colour.r = static_cast<sf::Uint8>(r * 255.0f);
		s->colour.g = static_cast<sf::Uint8>(g * 255.0f);
		s->colour.b = static_cast<sf::Uint8>(b * 255.0f);
		stars.push_back(s);
	}
	generateSystem(0);
}

void ServerGalaxy::generateSystem(int id) {
	//TODO: Threading
	systemLock.lock();
	if (!systems[id]) {
		shared_ptr<ServerSolarSystem> sys = std::make_shared<ServerSolarSystem>();
		sys->generateSystem(server);
		systems[id] = sys;
	}
	systemLock.unlock();
}

void ServerGalaxy::updateSystems(double dt) {
	systemLock.lock();
	for (auto system : systems) {
		system.second->update(dt);
	}
	systemLock.unlock();
}

void ServerGalaxy::sendUpdates() {
	systemLock.lock();
	for (auto system : systems) {
		system.second->sendUpdates();
	}
	systemLock.unlock();
}

void ServerGalaxy::addPlayer(shared_ptr<Player> player) {
	systemLock.lock();
	//TODO: Handle properly
	systems[0]->addPlayer(player);
	systemLock.unlock();
}

void ServerGalaxy::movePlayer(shared_ptr<Player> player, int systemNum) {
	//TODO: Pointers in player to get current system faster?
	if (systemNum < 0 || systemNum >= stars.size()) {
		return;
	}
	systemLock.lock();
	//Remove player from current system
	for (auto system : systems) {
		system.second->removePlayer(player);
	}
	if (!systems[systemNum]) {
		systemLock.unlock();
		generateSystem(systemNum);
		systemLock.lock();
	}
	//Add player to new system
	systems[systemNum]->addPlayer(player);
	systemLock.unlock();
}

void ServerGalaxy::removePlayer(shared_ptr<Player> player) {
	systemLock.lock();
	for (auto system : systems) {
		system.second->removePlayer(player);
	}
	systemLock.unlock();
}
