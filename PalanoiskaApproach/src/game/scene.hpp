#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "../engine/gameobject.hpp"
#include "../engine/mesh.hpp"
#include "../globals.hpp"
#include "player.hpp"
#include "playerattack.hpp"
#include "../engine/settings.hpp"
#include "enemy.hpp"
#include "../engine/floatingtext.hpp"
#include "interactable.hpp"

//Forward declarations
namespace sf {
	class RenderWindow;
	class View;
}

class Scene {
public:
	Scene() {
		player = nullptr;
		gridWidth = 0; gridHeight = 0;
		playerAttackSpeed = 7; playerAttackTimer = 0; }
	Scene(int gridw, int gridh, GameSettings::WindowSettings windowSettings);
	//= is removed because of RenderTexture
	Scene& operator=(const Scene& other) {
		//Clear vectors
		this->bgObjects.clear();
		this->staticObjects.clear();
		this->interactableObjects.clear();
		this->playerAttacks.clear();
		this->enemies.clear();
		this->floatText.clear();
		this->meshes.clear();

		this->player = nullptr;
		this->gridWidth = other.gridWidth;
		this->gridHeight = other.gridHeight;
		this->gameView = other.gameView;
		this->bgView = other.bgView;
		this->playerAttackSpeed = other.playerAttackSpeed;
		this->playerAttackTimer = other.playerAttackTimer;

		//Redraws it rather than copying it
		this->staticTextures.create(this->gridWidth * Globals::TileSize, this->gridHeight * Globals::TileSize);
		redrawTexture();

		this->backgroundTextures.create(this->gridWidth * Globals::TileSize, this->gridHeight * Globals::TileSize);
		redrawBackground();

		return *this;
	}
	void update();
	void draw(sf::RenderWindow& window);
	void setBackground(sf::Texture& texture);
	void addEnemy(const Enemy& enemy);
	void addBgStatic(StaticObject&& staticobject);
	void addStatic(const StaticObject& staticobject);
	void addInteractable(const Interactable& interactable);
	void addPlayer(Player& player);
	void addPlayerAttack(const PlayerAttack& attack);
	void generateMeshes();
	const Player& getPlayer() const { return *player; }
	std::vector<Interactable>& getInteractables() { return interactableObjects; }
private:
	void redrawTexture();
	void redrawBackground();
	std::vector<StaticObject> bgObjects;
	std::vector<StaticObject> staticObjects;
	std::vector<Interactable> interactableObjects;
	std::vector<PlayerAttack> playerAttacks;
	std::vector<Enemy> enemies;
	std::vector<FloatingText> floatText;
	std::vector<Mesh> meshes;
	sf::RenderTexture backgroundTextures;
	sf::RenderTexture staticTextures;
	sf::View gameView;
	sf::View bgView;
	sf::Sprite background;
	Player* player;
	int gridWidth;
	int gridHeight;
	//Used to time when attacks should do damage
	int playerAttackSpeed;
	int playerAttackTimer;
};

#endif