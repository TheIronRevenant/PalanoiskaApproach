#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "../engine/gameobject.hpp"
#include "../engine/mesh.hpp"
#include "../globals.hpp"
#include "player.hpp"
#include "playerattack.hpp"
#include "../engine/settings.hpp"

#pragma warning(disable : 26812 ) //Disable warning about sfml

//Forward declarations
namespace sf {
	class RenderWindow;
	class View;
}

class Scene {
public:
	Scene() { gridWidth = 0; gridHeight = 0; }
	Scene(int gridw, int gridh, GameSettings::WindowSettings windowSettings);
	//= is removed because of RenderTexture
	Scene& operator=(const Scene& other) {
		this->staticObjects = other.staticObjects;
		this->meshes = other.meshes;
		this->player = other.player;
		this->gridWidth = other.gridWidth;
		this->gridHeight = other.gridHeight;
		this->playerAttacks = other.playerAttacks;
		this->gameView = other.gameView;
		this->bgView = other.bgView;
		//Redraws it rather than copying it
		this->staticTextures.create(this->gridWidth * Globals::TileSize, this->gridHeight * Globals::TileSize);
		redrawTexture();
		return *this;
	}
	void update();
	void draw(sf::RenderWindow& window);
	void setBackground(sf::Texture& texture);
	void addStatic(const StaticObject& staticobject);
	void addPlayer(const Player& player);
	void addPlayerAttack(const PlayerAttack& attack);
	void generateMeshes();
private:
	void redrawTexture();
	std::vector<StaticObject> staticObjects;
	std::vector<PlayerAttack> playerAttacks;
	sf::RenderTexture staticTextures;
	std::vector<Mesh> meshes;
	sf::View gameView;
	sf::View bgView;
	sf::Sprite background;
	Player player;
	int gridWidth;
	int gridHeight;
};

#pragma warning(default : 26812 )

#endif