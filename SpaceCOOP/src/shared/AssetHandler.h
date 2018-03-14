#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

using std::string;

class AssetHandler {
public:
	AssetHandler();
	~AssetHandler();
	static sf::Texture& getTexture(string filename);
private:
	static std::unordered_map<string, sf::Texture> textures;
};

