#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

#include "game.hpp"
#include "resourceloader.hpp"
#include "../globals.hpp"
#include "gameobject.hpp"
#include <iostream>

//Get current directory
std::string current_dir() {
	char buffer[FILENAME_MAX];
	GetCurrentDir(buffer, FILENAME_MAX);
	return std::string(buffer);
}	

int Game::init() {
	//Init window
	window.create(sf::VideoMode(Globals::WindowWidth, Globals::WindowHeight), "Palanoiska Approach");

	//Init view
	view.setSize(Globals::WindowWidth / 2, Globals::WindowHeight / 2);
	view.setCenter(0, 0);
	window.setView(view);

	//Init clock
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	//Load resources
	loadTextures(textures, current_dir());

	//Init scene
	loadScene(currentScene, textures, current_dir(), "TestScene.tmx");

	while (window.isOpen()) {
		//Update clock
		sf::Time elapsedTime = clock.restart();
		timeSinceUpdate += elapsedTime;

		while (timeSinceUpdate > timePerFrame) {
			timeSinceUpdate -= timePerFrame;

			//Poll events
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::RBracket) {
						//Toggle debugging
						Globals::Debugging = !Globals::Debugging;
						if (Globals::Debugging) {
							std::cout << "Debugging enabled" << std::endl;
						} else {
							std::cout << "Debugging disabled" << std::endl;
						}
					}
				}
			}

			/*
			Update
			*/
			update();

			/*
			Draw
			*/
			draw();
		}
	}

	return 0;
}

void Game::update() {
	currentScene.update(view);

	window.setView(view);
}

void Game::draw() {
	window.clear(sf::Color(109, 194, 202));

	currentScene.draw(window);

	window.display();
}