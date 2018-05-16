#include "include/resource_manager.hpp"

void ResourceManager::loadAll()
{
	loadTexture("player", "flame.png");
	loadTexture("title", "stoke.png");
	loadTexture("menuImage", "menu_image.png");
	loadTexture("droplet", "droplet.png");
	loadTexture("matches", "matches.png");
	loadTexture("background", "background.png"); 
	loadFont("scoreFont", "joystix_monospace.ttf");
}

void ResourceManager::deleteAll()
{
	for (auto t = mTextureCache.begin(); t != mTextureCache.end(); ++t)
		delete t->second;

	for (auto f = mFontCache.begin(); f != mFontCache.end(); ++f)
		delete f->second;
}

void ResourceManager::loadTexture(std::string name, std::string filename)
{
	std::string path = "resources/textures/" + filename;

	sf::Texture* tex = new sf::Texture();

	if (!tex->loadFromFile(path))
	{
		std::cerr << "Could not load: " << path << std::endl;
	}
	else
		mTextureCache.insert(std::make_pair(name, tex));
}

void ResourceManager::loadFont(std::string name, std::string filename)
{
	std::string path = "resources/fonts/" + filename;

	sf::Font* fnt = new sf::Font();

	if (!fnt->loadFromFile(path))
	{
		std::cerr << "Could not load: " << path << std::endl;
	}
	else
		mFontCache.insert(std::make_pair(name, fnt));
}


sf::Texture& ResourceManager::getTexture(std::string name)
{
	return *mTextureCache.at(name);
}

sf::Font& ResourceManager::getFont(std::string name)
{
	return *mFontCache.at(name);
}
