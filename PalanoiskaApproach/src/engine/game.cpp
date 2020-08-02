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
	int screenwidth = sf::VideoMode::getDesktopMode().width;
	int screenheight = sf::VideoMode::getDesktopMode().height;
	windowSettings.screenw = screenwidth / 2;
	windowSettings.screenh = screenheight / 2;
	window.create(sf::VideoMode(windowSettings.screenw, windowSettings.screenh), "Palanoiska Approach");

	//Init view
	menuView.setSize((float)window.getSize().x / 2, (float)window.getSize().y / 2);
	menuView.setCenter(menuView.getSize().x / 2, menuView.getSize().y / 2);
	window.setView(menuView);

	//Init clock
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	//Load resources
	loadTextures(textures, current_dir());
	loadUI(uiTextures, current_dir() + "\\resources\\ui\\");
	loadBackgrounds(backgroundTextures, current_dir() + "\\resources\\");

	//Init scene
	changeScene("TestScene.tmx");
	currentScene.setBackground(backgroundTextures[0]);

	//Init ui
	uiManager = UIManager(&gameState, windowSettings);
	//Main menu
	uiManager.addMainMenu(UIElement(0, 0, []() {}, uiTextures[0])); //Title
	uiManager.addMainMenu(UIElement(0, 150, [this]() { gameState = GameState::InGame; changeScene("TestScene.tmx"); }, uiTextures[1])); //Play
	//Paused menu
	uiManager.addPaused(UIElement(0, 0, [this]() { gameState = GameState::InGame; }, uiTextures[3])); //Resume
	uiManager.addPaused(UIElement(0, 100, [this]() { gameState = GameState::MainMenu; }, uiTextures[2])); //Main menu
	//Game ui
	uiManager.addInGame(UIElement(-800.f, 295.f, []() {}, uiTextures[5])); //Hp background
	uiManager.addInGame(UIElement(-800.f, 295.f, []() {}, uiTextures[6])); //Hp bar
	uiManager.addInGame(UIElement(-1000.f, 284.f, []() {}, uiTextures[4])); //Main overlay

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

					if (event.key.code == sf::Keyboard::Escape) {
						switch (gameState) {
						case GameState::MainMenu:
							window.close();
							break;
						case GameState::InGame:
							gameState = GameState::Paused;
							break;
						case GameState::Paused:
							gameState = GameState::InGame;
							break;
						}
					}
				}

				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2f coords = window.mapPixelToCoords(
							sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
						uiManager.recieveClick(coords.x, coords.y);
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
	if (gameState == GameState::InGame) {
		currentScene.update();
		uiManager.update(currentScene.getPlayer());
		if (currentScene.getPlayer().isDead()) {
			changeScene("TestScene.tmx");
		}
	}
}

void Game::draw() {
	window.clear(sf::Color(109, 194, 202));

	switch (gameState) {
	case GameState::MainMenu:
		window.setView(menuView);
		uiManager.draw(window);
		break;
	case GameState::InGame:
		currentScene.draw(window);
		uiManager.draw(window);
		break;
	case GameState::Paused:
		window.setView(menuView);
		uiManager.draw(window);
		break;
	}

	window.display();
}

void Game::changeScene(std::string name) {
	loadScene(currentScene, textures, current_dir() + "\\resources\\scenes\\", name);
}