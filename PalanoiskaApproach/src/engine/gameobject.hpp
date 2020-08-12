#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>

//Forward declarations
class Mesh;

class GameObject {
public:
	GameObject() {}
	GameObject(unsigned int gridx, unsigned int gridy, sf::Texture& texture);
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) const;
	virtual void draw(sf::RenderTexture& texture) const;
	virtual void move(sf::Vector2f vector);
	virtual void move(float x, float y);
	virtual void setPosition(sf::Vector2f vector);
	virtual void setPosition(float x, float y);
	virtual void setColor(sf::Color color);
	const sf::Vector2f& getPosition() const { return boundingBox.getPosition(); }
	const sf::Vector2f& getSize() const { return boundingBox.getSize(); }
	const sf::RectangleShape& getBoundingBox() const { return boundingBox; }
protected:
	sf::Sprite sprite;
	sf::RectangleShape boundingBox;
};

class StaticObject : public GameObject {
public:
	StaticObject() {}
	StaticObject(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {};
	void update() {} //Override the abstract function
};

#endif