#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Scene;

void loadTextures(std::vector<sf::Texture>& textures, std::string folder);
void loadUI(std::vector<sf::Texture>& textures, std::string folder);
void loadScene(Scene& scene, std::vector<sf::Texture>& textures, std::string folder, std::string file);

#endif