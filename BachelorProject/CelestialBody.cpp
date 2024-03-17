#include "CelestialBody.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="n"></param>
/// <param name="m"></param>
/// <param name="diam"></param>
/// <param name="distance"></param>
/// <param name="ov"></param>
CelestialBody::CelestialBody(std::string n, int m, double diam, double distance, double ov, sf::Vector2f pos)
{
	name = n;
	mass = m;
	diameter = diam;
	distanceFromSun = distance;
	velocity = ov;
	acceleration = 0;
	position = pos;
}

void CelestialBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform transform = getTransform();
	//transform.scale(scaleFactor, scaleFactor);
	sf::CircleShape circle(diameter / 2.0f);
	circle.setOrigin(diameter / 2.0f, diameter / 2.0f);
	circle.setPosition(position);
	circle.setFillColor(color);
	target.draw(circle, transform);
}

/// <summary>
/// Draws the Orbit of the body to the window
/// </summary>
void CelestialBody::draw_orbit()
{
}

/// <summary>
/// Updates the position, calculated using a combination of position, velocity and direction
/// </summary>
/// <returns>updated position</returns>
sf::Vector2f CelestialBody::update_position(double deltaTime)
{
	position.x += static_cast<int>(velocity * direction.x * deltaTime);
	position.y += static_cast<int>(velocity * direction.y * deltaTime);
	return position;
}

/// <summary>
/// Updates the velocity using the acceleration of the body
/// </summary>
/// <returns>updated velocity</returns>
double CelestialBody::update_velocity(double deltaTime)
{
	velocity += acceleration * deltaTime;
	return velocity;
}

void CelestialBody::setScale(float factor)
{
	scaleFactor = factor;
}