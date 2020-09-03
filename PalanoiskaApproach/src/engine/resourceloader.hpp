#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class Scene;

void loadTextures(std::vector<sf::Texture>& textures, std::string folder);
void loadUI(std::vector<sf::Texture>& textures, std::string folder);
void loadBackgrounds(std::vector<sf::Texture>& textures, std::string folder);
void loadScene(Scene& scene, std::vector<sf::Texture>& textures, std::string folder, std::string file);
void loadFont(sf::Font& font, std::string folder);
void loadDialogue(std::unordered_map<std::string, std::string>& dialogueText, std::string file);

#endif