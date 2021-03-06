#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

#include "game.hpp"
#include "resourceloader.hpp"
#include "../globals.hpp"
#include "gameobject.hpp"
#include <iostream>

GameSettings::Settings settings{};
GameSettings::WindowSettings windowSettings{};

//Get current directory
std::string current_dir() {
	char buffer[FILENAME_MAX];
	if (!GetCurrentDir(buffer, FILENAME_MAX)) {
		//_getcwd failed
	}
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
	loadFont(font, current_dir() + "\\resources\\");
	settings.font = &font;
	loadDialogue(dialogueText, "resources\\localization\\pn_en.pnl");

	//Init player
	player = Player(0, 0, 
		PlayerAnimator(Animation{ 
			std::vector<sf::Texture*>{ &textures[Globals::PlayerOffset],& textures[Globals::PlayerOffset + 1] },
			10 }));

	PlayerAttack slash(Animation{ 
		std::vector<sf::Texture*>{ &textures[Globals::EffectOffset], &textures[Globals::EffectOffset + 1] },
		7 }, 2, AttackType::Slash);
	player.addAttack("slash", slash);

	PlayerAttack hammer(Animation{
		std::vector<sf::Texture*>{ &textures[Globals::EffectOffset + 2], &textures[Globals::EffectOffset + 3] },
		35 }, 2, AttackType::Overhead);
	player.addAttack("hammer", hammer);
	player.setAttack("hammer");

	//Init scene
	changeScene("Thisehn.tmx");
	player.setPosition(0 * Globals::TileSize, 0 * Globals::TileSize);
	currentScene.setBackground(backgroundTextures[0]);

	//Init ui
	uiManager = UIManager(&gameState);
	//Main menu
	uiManager.addMainMenu(UIElement(0, 0, []() {}, uiTextures[0])); //Title
	uiManager.addMainMenu(UIElement(0, 150, [this]() { 
			gameState = GameState::InGame; changeScene("Thisehn.tmx"); 
			player.setPosition(17 * Globals::TileSize, 32 * Globals::TileSize);
			player.reset();
		}, uiTextures[1])); //Play
	//Paused menu
	uiManager.addPaused(UIElement(0, 0, [this]() { gameState = GameState::InGame; }, uiTextures[3])); //Resume
	uiManager.addPaused(UIElement(0, 100, [this]() { gameState = GameState::MainMenu; }, uiTextures[2])); //Main menu
	//Game ui
	uiManager.addInGame(UIElement(-800.f, 295.f, []() {}, uiTextures[5])); //Hp background
	uiManager.addInGame(UIElement(-800.f, 295.f, []() {}, uiTextures[6])); //Hp bar
	uiManager.addInGame(UIElement(-1000.f, 284.f, []() {}, uiTextures[4])); //Main overlay
	sf::Text hpText;
	hpText.setPosition(-795.f, 292.5f);
	hpText.setFont(font);
	hpText.setCharacterSize(20);
	uiManager.addInGame(hpText);

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
			gameState = GameState::MainMenu;
			player.reset();
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
	currentScene.addPlayer(player);
	std::vector<Interactable>& interactables = currentScene.getInteractables();
	if (name == "TestScene.tmx") {
		interactables[0].dialogueInfo = DialogueInfo{ 
			{
				Dialogue(interactables[0].getPosition().x - (2 * Globals::TileSize), interactables[0].getPosition().y - (2 * Globals::TileSize), dialogueText["testscene_test1"], 20, font),
				Dialogue(interactables[0].getPosition().x - (2 * Globals::TileSize), interactables[0].getPosition().y - (2 * Globals::TileSize), dialogueText["testscene_test2"], 20, font)
			},
			true, false, true, 2, 0, 60, 0 };
		interactables[0].interactfunc = [](DialogueInfo& diag) {
			if (diag.hasDialogue && !diag.visible) {
				diag.visible = true;
				diag.showCount = 0;
			}
		};
		interactables[1].interactfunc = [this](DialogueInfo& diag) { 
			changeScene("TestScene2.tmx"); 
			player.setPosition(33 * Globals::TileSize, 52 * Globals::TileSize); 
		};
	} else
	if (name == "TestScene2.tmx") {
		interactables[0].interactfunc = [this](DialogueInfo& diag) {
			changeScene("TestScene.tmx"); 
			player.setPosition(23 * Globals::TileSize, 73 * Globals::TileSize);
		};
	} else
	if (name == "Thisehn.tmx") {
		Interactable& cartGuy = interactables[0];
		Interactable& cart = interactables[1];

		//Cart Guy
		interactables[0].disable();
		interactables[0].dialogueInfo = DialogueInfo{
			{
				Dialogue(interactables[0].getPosition().x - (4 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_cartguy1"], 16, font),
				Dialogue(interactables[0].getPosition().x - (4 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_cartguy2"], 16, font)
			},
			true, false, true, 2, 0, 60, 0 };
		interactables[0].interactfunc = [&cart](DialogueInfo& diag) mutable {
			if (diag.hasDialogue && !diag.visible) {
				diag.visible = true;
				diag.showCount = 0;
				cart.enable();
			}
		};

		//Cart
		interactables[1].disable();
		interactables[1].dialogueInfo = DialogueInfo{};
		interactables[1].interactfunc = [this](DialogueInfo& diag) {
			changeScene("TestScene.tmx");
			player.setPosition(23 * Globals::TileSize, 73 * Globals::TileSize);
		};

		//Kral Hierophant
		interactables[2].dialogueInfo = DialogueInfo{
			{
				Dialogue(interactables[2].getPosition().x - (3 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_kral1"], 16, font),
				Dialogue(interactables[2].getPosition().x - (4 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_kral2"], 16, font),
				Dialogue(interactables[2].getPosition().x - (3 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_kral3"], 16, font),
				Dialogue(interactables[2].getPosition().x - (4 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_kral4"], 16, font),
				Dialogue(interactables[2].getPosition().x - (3 * Globals::TileSize), interactables[2].getPosition().y - (3 * Globals::TileSize), dialogueText["thisehn_kral5"], 16, font),
				Dialogue(interactables[2].getPosition().x - (3 * Globals::TileSize), interactables[2].getPosition().y - (2 * Globals::TileSize), dialogueText["thisehn_kral6"], 16, font)
			},
			true, false, true, 6, 0, 60, 0 };
		interactables[2].interactfunc = [&cartGuy](DialogueInfo& diag) mutable {
			if (diag.hasDialogue && !diag.visible) {
				diag.visible = true;
				diag.showCount = 0;
				cartGuy.enable();
			}
		};
	}
}