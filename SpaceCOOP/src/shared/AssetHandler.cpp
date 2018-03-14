#include "AssetHandler.h"

std::unordered_map<string, sf::Texture> AssetHandler::textures;

AssetHandler::AssetHandler() {
}


AssetHandler::~AssetHandler() {
}

sf::Texture& AssetHandler::getTexture(string filename) {
	auto index = textures.find(filename);
	if (index != textures.end()) {
		return index->second;
	}
	sf::Texture& t = textures[filename];

	if (!t.loadFromFile(filename)) {
		throw new std::exception("Could not load texture");
	}

	return t;
}
