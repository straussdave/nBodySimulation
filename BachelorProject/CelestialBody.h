#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <chrono>
#include <deque>

class CelestialBody : public sf::Drawable, public sf::Transformable
{
public:
	CelestialBody(std::string n, float m, float diam, float distance, sf::Vector2f ov, sf::Vector2f pos, int maxPos, sf::Color clr, float G);
	sf::Vector2f position; //position is stored as floats for accurate calculation
	std::string name;
	float mass;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void apply_force(sf::Vector2f force, float deltaTime);
	sf::Vector2f update_position(float deltaTime);
	sf::Color get_color();
private:
	sf::Color color;
	int counter = 0;
	float timeFactor = 200.0f;
	float g;
	int maxOrbitPositions;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f direction;
	float diameter;
	float distanceFromSun;
	float scaleFactor = 1.0f;
	std::deque<sf::Vector2f> orbitPositions;
};