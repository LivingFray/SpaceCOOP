#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <string>
#include "../shared/Console.h"
class GraphicalConsole :
	public sf::Drawable,
	public Console{
public:
	GraphicalConsole();
	~GraphicalConsole();

	void log(std::string msg, LogLevel level);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static void loadFont(std::string font);

	void resize(unsigned int width, unsigned int height);

	struct Message {
		std::string msg;
		LogLevel level;
	};
	sf::String command;
	static sf::Font font;
private:
	std::vector<Message> logs;
	mutable std::mutex lock;
	size_t viewOffset = 0;
	static const unsigned int fontSize;

	//Positioning variables
	static const float borderX;
	static const float borderY;
	static const float lineGap;

	sf::View consoleView;
};

