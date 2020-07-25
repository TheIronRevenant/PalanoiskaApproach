#include <iostream>
#include <pugixml.hpp>
#include "resourceloader.hpp"
#include "../game/scene.hpp"
#include "../player.hpp"

void loadTextures(std::vector<sf::Texture>& textures, std::string folder) {
	textures.clear();

	int spacing = 1;
	int tilesize = 16;
	int total = 4;
	std::string file = folder + "\\resources\\player.png";

	/*
	Load Player
	*/
	for (int i = 0; i < total; i++) {
		sf::Texture texture;
		sf::IntRect rect;
		rect.width = tilesize;
		rect.height = tilesize;
		rect.left = (tilesize + spacing) * i;

		if (!texture.loadFromFile(file, rect)) {
			//Texture failed to load
		}

		textures.push_back(texture);
	}

	/*
	Load tileset
	*/
	total = 9;
	file = folder + "\\resources\\tiles.png";

	for (int i = 0; i < total; i++) {
		sf::Texture texture;
		sf::IntRect rect;
		rect.width = tilesize;
		rect.height = tilesize;
		rect.left = (tilesize + spacing) * i;

		if (!texture.loadFromFile(file, rect)) {
			//Texture failed to load
		}

		textures.push_back(texture);
	}

	/*
	Load effects
	*/
	total = 2;
	file = folder + "\\resources\\effects.png";

	for (int i = 0; i < total; i++) {
		sf::Texture texture;
		sf::IntRect rect;
		rect.width = tilesize * 2;
		rect.height = tilesize;
		rect.left = ((tilesize * 2) + spacing) * i;

		if (!texture.loadFromFile(file, rect)) {
			//Texture failed to load
		}

		textures.push_back(texture);
	}
}

void loadScene(Scene& scene, std::vector<sf::Texture>& textures, std::string folder, std::string file) {
	//String to wchar_t
	std::string sceneFile = folder + "\\resources\\scenes\\" + file;
	std::wstring wideFile = std::wstring(sceneFile.begin(), sceneFile.end());
	const wchar_t* wcharFile = wideFile.c_str();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(wcharFile);

	//If file isn't loaded
	if (!result) {
		std::cout << "Failed to load file " << sceneFile << std::endl;
		return;
	}

	pugi::xml_node map = doc.child("map");
	pugi::xml_node tileLayer = map.child("layer").child("data");

	int width = std::stoi(map.attribute("width").value());
	int height = std::stoi(map.attribute("height").value());

	//Create vector of all tiles
	std::vector<pugi::xml_node> tileNodes;
	for (pugi::xml_node_iterator it = tileLayer.begin(); it != tileLayer.end(); ++it) {
		tileNodes.push_back(*it);
	}

	//Recreate the scene
	scene = Scene(width, height);

	for (int i = 0; i < (int)tileNodes.size(); i++) {
		if (tileNodes[i].attribute("gid")) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = std::stoi(tileNodes[i].attribute("gid").value());

			//If this is the player
			if (id == 10) {
				PlayerAnimator animator(
					Animation{ std::vector<sf::Texture*>{ &textures[2], &textures[3] }, 10 }, 
					Animation{ std::vector<sf::Texture*>{ &textures[0], &textures[1] }, 10 });
				scene.addPlayer(Player(x, y, animator));
			} else {
				if (id > 10) {
					scene.addStatic(StaticObject(x, y, textures[id - 10]));
				} else {
					scene.addStatic(StaticObject(x, y, textures[id + 3]));
				}
			}
		}
	}

	scene.generateMeshes();
}