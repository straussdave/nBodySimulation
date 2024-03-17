#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class CelestialBody : public sf::Drawable, public sf::Transformable
{
public:
	CelestialBody(std::string n, int m, double diam, double distance, double ov, sf::Vector2f pos);
	sf::Vector2f position; //position is stored as floats for accurate calculation
	std::string name;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setScale(float factor);

private:
	int mass;
	double velocity;
	double acceleration;
	double diameter;
	sf::Vector2f direction;
	double distanceFromSun;
	sf::Color color = sf::Color::White;
	float scaleFactor = 1.0f;

	void draw_orbit();
	sf::Vector2f update_position(double deltaTime);
	double update_velocity(double deltaTime);
};