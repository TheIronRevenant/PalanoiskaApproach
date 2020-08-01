#include <utility>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "scene.hpp"
#include <iostream>

Scene::Scene(int gridw, int gridh, GameSettings::WindowSettings windowSettings) {
	gridWidth = gridw;
	gridHeight = gridh;
	staticTextures.create(gridw * Globals::TileSize, gridh * Globals::TileSize);
	staticTextures.clear(sf::Color::Transparent);

	gameView.setSize(windowSettings.resw * 36.f, windowSettings.resh * 36.f);
	gameView.setCenter(0, 0);

	bgView.reset(sf::FloatRect(-1000.f, 0.f, (float)windowSettings.resw * 36.f, (float)windowSettings.resh * 36.f));
	background.setPosition(-1000.f, 0.f);
}

void Scene::update() {
	player.update(meshes, *this);

	sf::Vector2f pos = player.getPosition();
	sf::Vector2f size = player.getSize();
	float vx = pos.x + (size.x / 2);
	float vy = pos.y + (size.y / 2);
	gameView.setCenter(vx, vy);

	for (PlayerAttack& p : playerAttacks) {
		p.update();
	}

	//Checks if the attack is finished
	if (playerAttacks.size() != 0) {
		playerAttacks.erase(
			std::remove_if(
				playerAttacks.begin(), 
				playerAttacks.end(), 
				[](PlayerAttack const& p) { return p.isTerminated(); }), 
			playerAttacks.end());
	}
}

void Scene::draw(sf::RenderWindow& window) {
	//Draw background
	window.setView(bgView);
	window.draw(background);
	window.setView(gameView);

	//Draws static objects from the render texture
	window.draw(sf::Sprite(staticTextures.getTexture()));

	player.draw(window);

	for (PlayerAttack& p : playerAttacks) {
		p.draw(window);
	}

	if (Globals::Debugging) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(window);
		}
	}
}

void Scene::setBackground(sf::Texture& texture) {
	background.setTexture(texture);
	background.setPosition(-1000.f, 0.f);
}

void Scene::addStatic(const StaticObject& staticobject) {
	if ((staticobject.getPosition().x / 16) > gridWidth || (staticobject.getPosition().y / 16) > gridHeight) {
		std::cout << "Object at " << staticobject.getPosition().x << " " << staticobject.getPosition().y << " is out of scene bounds, not added" << std::endl;
		return;
	}

	staticobject.draw(staticTextures);
	staticTextures.display();

	staticObjects.emplace_back(staticobject);
}

void Scene::addPlayer(const Player& player) {
	this->player = player;
}

void Scene::addPlayerAttack(const PlayerAttack& attack) {
	playerAttacks.emplace_back(attack);
}

//Redraws texture for when copying scene
void Scene::redrawTexture() {
	staticTextures.clear(sf::Color::Transparent);

	for (StaticObject s : staticObjects) {
		s.draw(staticTextures);
	}

	staticTextures.display();
}

void Scene::generateMeshes() {
	sf::Clock timer;

	//Init matrix
	std::vector<std::vector<std::pair<StaticObject, bool>*>> matrix;
	matrix.resize(gridWidth);
	for (int i = 0; i < gridWidth; i++) {
		matrix[i].resize(gridHeight);
	}

	//Add objects to matrix
	for (StaticObject s : staticObjects) {
		int x = (int)(s.getPosition().x / 16);
		int y = (int)(s.getPosition().y / 16);
		matrix[x][y] = new std::pair<StaticObject, bool>(s, false);
	}

	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			//If an object is at this position
			if (matrix[i][j]) {
				//If the object has not been used yet
				if (!matrix[i][j]->second) {
					Mesh m(i, j, 1, 1);
					matrix[i][j]->second = true;

					bool moveRight = true;
					int r = 1;
					while (moveRight) {
						//If it is within bounds
						if ((i + r) < gridWidth) {
							//If it exists at position
							if (matrix[i + r][j]) {
								//If it has not been used yet
								if (!matrix[i + r][j]->second) {
									matrix[i + r][j]->second = true;
									m.expandRight();
									r++;
								} else {
									moveRight = false;
								}
							} else {
								moveRight = false;
							}
						} else {
							moveRight = false;
						}
					}

					bool moveDown = true;
					int d = 1;
					while (moveDown) {
						bool canMoveDown = true;
						//If it is within bounds
						if ((j + d) < gridHeight) {
							for (int right = 0; right < r; right++) {
								//If it exists at position
								if (matrix[i + right][j + d]) {
									//If it has already been used
									if (matrix[i + right][j + d]->second) {
										canMoveDown = false;
									}
								} else {
									canMoveDown = false;
								}
							}
						} else {
							moveDown = false;
						}

						if (canMoveDown) {
							for (int right = 0; right < r; right++) {
								matrix[i + right][j + d]->second = true;
							}
							m.expandDown();
							d++;
						} else {
							moveDown = false;
						}
					}

					meshes.emplace_back(m);
				}
			}
		}
	}

	//Delete pointers
	for (std::vector<std::pair<StaticObject, bool>*> r : matrix) {
		for (std::pair<StaticObject, bool>* p : r) {
			delete p;
		}
		r.clear();
	}
	matrix.clear();

	std::cout << "Generated scene meshes in " << timer.restart().asSeconds() << " seconds" << std::endl;
}