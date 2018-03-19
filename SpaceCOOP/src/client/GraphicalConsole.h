#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
class GraphicalConsole :
	public sf::Drawable {
public:
	GraphicalConsole();
	~GraphicalConsole();
	enum class LogLevel { INFO, WARNING, ERROR };

	void log(std::string msg, LogLevel level);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static void loadFont(std::string font);

	struct Message {
		std::string msg;
		LogLevel level;
	};
private:
	std::vector<Message> logs;
	mutable std::mutex lock;
	size_t viewOffset = 0;
	static const unsigned int fontSize;
	static sf::Font font;

	//Positioning variables
	static const float borderX;
	static const float borderY;
	static const float lineGap;
};

