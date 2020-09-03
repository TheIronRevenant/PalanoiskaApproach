#include <iostream>
#include <sstream>
#include <fstream>
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
	total = 32;
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
	Load npcs
	*/
	total = 2;
	file = folder + "\\resources\\npcs.png";
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
	int height = std::stoi(map.attribute("height").value()) + 1;

	//Create vector of all tiles
	std::vector<int> bgIds;
	std::string bgData(bgLayer.text().as_string());
	std::stringstream bgStream(bgData);
	while(bgStream.good()) {
		std::string sub;
		std::getline(bgStream, sub, ',');
		int val = std::stoi(sub);
		bgIds.push_back(val);
	}

	std::vector<int> tileIds;
	std::string tileData(tileLayer.text().as_string());
	std::stringstream tileStream(tileData);
	while(tileStream.good()) {
		std::string sub;
		std::getline(tileStream, sub, ',');
		int val = std::stoi(sub);
		tileIds.push_back(val);
	}

	std::vector<int> interactIds;
	std::string interactData(interactLayer.text().as_string());
	std::stringstream interactStream(interactData);
	while(interactStream.good()) {
		std::string sub;
		std::getline(interactStream, sub, ',');
		int val = std::stoi(sub);
		interactIds.push_back(val);
	}

	//Recreate the scene
	scene = Scene(width, height, GameSettings::WindowSettings{});

	for (int i = 0; i < (int)bgIds.size(); i++) {
		if (bgIds[i] != 0) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = bgIds[i];

			scene.addBgStatic(StaticObject(x, y, textures[id + 1]));
		}
	}
	
	for (int i = 0; i < (int)tileIds.size(); i++) {
		if (tileIds[i] != 0) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = tileIds[i];

			if (id >= 1 && id < 18) { //Update this to add tile ----------------
				//Tile
				scene.addStatic(StaticObject(x, y, textures[id + 1]));
			} else {
				//Enemy
				switch (id) { //Update this to add tile ------------------------
				case 33:
					scene.addEnemy(Enemy(x, y,
						EnemyAnimator(Animation{
							std::vector<sf::Texture*>{ &textures[Globals::EnemyOffset],& textures[Globals::EnemyOffset + 1] },
							40 }),
							EnemyType::OrangeTest));
					break;
				case 35:
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

	for (int i = 0; i < (int)interactIds.size(); i++) {
		if (interactIds[i] != 0) {
			//Get grid x and y from i
			int x = i % width;
			int y = i / width;
			int id = interactIds[i];
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

void loadDialogue(std::unordered_map<std::string, std::string>& dialogueText, std::string file) {
	dialogueText.clear();
	std::fstream fs(file);
	while (fs.good()) {
		std::string key;
		std::string value;
		std::getline(fs, key, '{');
		std::getline(fs, value, '}');
		fs.ignore();
		
		size_t pos = value.find("\\n");
		while (pos != std::string::npos) {
			value.replace(pos, 2, "\n");
			pos = value.find("\\n");
		}
		
		dialogueText[key] = value;
	}
}


/*
---------------------------------------------
Adding a tile
1. Add tile to tilesheet
2. Add to offsets in Globals
3. Add to total tiles to load in loadTextures
4. Add to values for adding tiles to scene
5. Update scenes in tiled
----------------------------------------------
*/