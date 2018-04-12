#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

using std::shared_ptr;

class State {
public:
	State();
	virtual ~State();

	virtual void onActivation() {};

	virtual void update(double dt) = 0;
	virtual void draw() = 0;
	virtual void keyEvent(sf::Event e) = 0;
	virtual void textEvent(sf::Event e) = 0;
	virtual void resizeEvent(sf::Event e) = 0;
	virtual void mouseEvent(sf::Event e) = 0;
	sf::RenderWindow* window;

	//Sets the state to be used next
	static void setState(shared_ptr<State> newState);
	//Returns the state currently active
	static shared_ptr<State> getState();
	//Advances the state if possible, returning the new active state
	static shared_ptr<State> advanceState();
private:
	static shared_ptr<State> currentState;
	static shared_ptr<State> nextState;
};

