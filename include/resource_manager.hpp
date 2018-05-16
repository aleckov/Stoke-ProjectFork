#pragma once
#include <iostream>
#include <string>
#include <map>
#include "SFML/Graphics.hpp"

class ResourceManager {
	private:
		std::map<std::string, sf::Texture*> mTextureCache;
		std::map<std::string, sf::Font*> mFontCache;
	public:
		ResourceManager() {};
		~ResourceManager() {};
		void loadAll();
		void deleteAll();
		void loadTexture(std::string name, std::string filename);
		void loadFont(std::string name, std::string filename);
		sf::Texture& getTexture(std::string name);
		sf::Font& getFont(std::string name);
};
