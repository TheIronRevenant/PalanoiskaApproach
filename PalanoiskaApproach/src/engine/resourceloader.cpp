#include <iostream>
#include <pugixml.hpp>
#include "resourceloader.hpp"
#include "../game/scene.hpp"
#include "../game/player.hpp"
#include "../game/playerattack.hpp"
#include "../game/interactable.hpp"
#include "../game/dialogue.hpp"

void loadTextures(std::vector<sf::Texture>& textures, std::string folder) {
	textures.clear();

	int spacing = 1;
	int tilesize = 16;
	int total = 2;
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
	total = 11;
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
	Load enemies
	*/
	total = 4;
	file = folder + "\\resources\\enemies.png";
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

void loadUI(std::vector<sf::Texture>& textures, std::string folder) {
	std::string files[7] = { "title.png",
		"play.png",
		"mainmenu.png",
		"resume.png",
		"playerui.png",
		"hpbackground.png",
		"hpbar.png" };

	for (std::string& s : files) {
		std::string file = folder + s;
		sf::Texture texture;

		if (!texture.loadFromFile(file)) {
			//Texture failed to load
		}

		textures.push_back(texture);
	}
}

void loadBackgrounds(std::vector<sf::Texture>& textures, std::string folder) {
	std::string files[1] = { "background.png" };

	for (std::string& s : files) {
		std::string file = folder + s;
		sf::Texture texture;

		if (!texture.loadFromFile(file)) {
			//Texture failed to load
		}

		textures.push_back(texture);
	}
}

void loadScene(Scene& scene, std::vector<sf::Texture>& textures, std::string folder, std::string file) {
	//String to wchar_t
	std::string sceneFile = folder + file;
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
	pugi::xml_node bgLayer = map.child("layer").child("data");
	pugi::xml_node tileLayer = bgLayer.parent().next_sibling().child("data");
	pugi::xml_node interactLayer = tileLayer.parent().next_sibling().child("data");

	int width = std::stoi(map.attribute("width").value());
	int height = std::stoi(map.attribute("height").value());

	//Create vector of all tiles
	std::vector<pugi::xml_node> tileNodes;
	for (pugi::xml_node_iterator it = tileLayer.begin(); it != tileLayer.end(); ++it) {
		tileNodes.push_back(*it);
	}

	//Create vector of bg tiles
	std::vector<pugi::xml_node> bgNodes;
	for (pugi::xml_node_iterator it = bgLayer.begin(); it != bgLayer.end(); ++it) {
		bgNodes.push_back(*it);
	}

	std::vector<pugi::xml_node> interactNodes;
	for (pugi::xml_node_iterator it = interactLayer.begin(); it != interactLayer.end(); ++it) {
		interactNodes.push_back(*it);
	}

	//Recreate the scene
	scene = Scene(width, height, GameSettings::WindowSettings{});

	for (int i = 0; i < (int)bgNodes.size(); i++) {
		if (bgNodes[i].attribute("gid")) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = std::stoi(bgNodes[i].attribute("gid").value());

			scene.addBgStatic(StaticObject(x, y, textures[id + 1]));
		}
	}

	for (int i = 0; i < (int)tileNodes.size(); i++) {
		if (tileNodes[i].attribute("gid")) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = std::stoi(tileNodes[i].attribute("gid").value());

			if (id >= 1 && id < 12) {
				//Tile
				scene.addStatic(StaticObject(x, y, textures[id + 1]));
			} else {
				//Enemy
				switch (id) {
				case 12:
					scene.addEnemy(Enemy(x, y,
						EnemyAnimator(Animation{
							std::vector<sf::Texture*>{ &textures[Globals::EnemyOffset],& textures[Globals::EnemyOffset + 1] },
							40 }),
							EnemyType::OrangeTest));
					break;
				case 14:
					scene.addEnemy(Enemy(x, y,
						EnemyAnimator(Animation{
							std::vector<sf::Texture*>{ &textures[Globals::EnemyOffset + 2],& textures[Globals::EnemyOffset + 3] },
							25 }),
							EnemyType::PinkTest));
					break;
				default:
					std::cout << "Id " << id << " not found" << std::endl;
					break;
				}
			}
		}
	}

	for (int i = 0; i < (int)interactNodes.size(); i++) {
		if (interactNodes[i].attribute("gid")) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = std::stoi(interactNodes[i].attribute("gid").value());
			scene.addInteractable(Interactable(x, y, textures[id + 1], DialogueInfo{}));
		}
	}

	scene.generateMeshes();
}

void loadFont(sf::Font& font, std::string folder) {
	std::string fontFile = folder + "font.ttf";
	if (!font.loadFromFile(fontFile)) {
		//Failed to load font
	}
}
