#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "../engine/gameobject.hpp"
#include "../engine/mesh.hpp"
#include "../globals.hpp"
#include "player.hpp"
#include "playerattack.hpp"

//Forward declarations
namespace sf {
	class RenderWindow;
	class View;
}

class Scene {
public:
	Scene() {}
	Scene(int gridw, int gridh);
	//= is removed because of RenderTexture
	Scene& operator=(const Scene& other) {
		this->staticObjects = other.staticObjects;
		this->meshes = other.meshes;
		this->player = other.player;
		this->gridWidth = other.gridWidth;
		this->gridHeight = other.gridHeight;
		this->playerAttacks = other.playerAttacks;
		//Redraws it rather than copying it
		this->staticTextures.create(this->gridWidth * Globals::TileSize, this->gridHeight * Globals::TileSize);
		redrawTexture();
		return *this;
	}
	void update(sf::RenderWindow& window, sf::View& view);
	void draw(sf::RenderWindow& window);
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
	Player player;
	int gridWidth;
	int gridHeight;
};

#endif