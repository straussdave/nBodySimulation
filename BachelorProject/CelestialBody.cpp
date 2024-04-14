#include "CelestialBody.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="n"></param>
/// <param name="m"></param>
/// <param name="diam"></param>
/// <param name="distance"></param>
/// <param name="ov"></param>
CelestialBody::CelestialBody(std::string n, float m, float diam, float distance, sf::Vector2f ov, sf::Vector2f pos, int maxPos, sf::Color clr, float G)
{
	direction = sf::Vector2f(0.0f, 1.0f);
	name = n;
	mass = m;
	diameter = diam;
	distanceFromSun = distance;
	velocity = ov;
	position = pos;
	maxOrbitPositions = maxPos;
	orbitPositions.push_back(position);
	color = clr;
	g = G;
}

void CelestialBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform transform = getTransform();
	if (orbitPositions.size() > 1) {
		sf::VertexArray orbitLine(sf::LineStrip, orbitPositions.size());
		for (size_t i = 0; i < orbitPositions.size(); ++i) {
			orbitLine[i].position = orbitPositions[i];
			orbitLine[i].color = color;
		}
		target.draw(orbitLine, transform);
	}
	//transform.scale(scaleFactor, scaleFactor);
	sf::CircleShape circle(diameter / 2.0f);
	circle.setOrigin(diameter / 2.0f, diameter / 2.0f);
	circle.setPosition(position);
	circle.setFillColor(color);
	target.draw(circle, transform);
}

/// <summary>
/// Updates the position, calculated using a combination of position, velocity and direction
/// </summary>
/// <returns>updated position</returns>
sf::Vector2f CelestialBody::update_position(float dt)
{
	position += velocity * dt;
	sf::Vector2f distanceVector = sf::Vector2f(orbitPositions.back().x - position.x, orbitPositions.back().y - position.y);
	if (sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2)) > 5) {
		orbitPositions.push_back(position);
		while (orbitPositions.size() > maxOrbitPositions) {
			//this makes the line decay after reaching max size
			orbitPositions.pop_front();
		}
	}
	
	
	return position;
}

sf::Color CelestialBody::get_color()
{
	return color;
}

/// <summary>
/// Updates the velocity using the acceleration of the body
/// </summary>
/// <returns>updated velocity</returns>
void CelestialBody::calculate_force(float dt, std::vector<CelestialBody> otherBodies)
{
	sf::Vector2f force;
	for (auto& body : otherBodies)
	{
		if (body.name != name)
		{
			sf::Vector2f distanceVector = sf::Vector2f(body.position.x - position.x, body.position.y - position.y);
			float vectorMagnitude = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
			vectorMagnitude += 0.01f;
			sf::Vector2f normalizedVector = distanceVector / vectorMagnitude;
			force += (g * normalizedVector * mass * body.mass) / pow(vectorMagnitude, 2.0f);
		}
	}
	apply_force(force, dt);
}

void CelestialBody::apply_force(sf::Vector2f force, float deltaTime) {
	acceleration = force / mass;
	velocity += acceleration * deltaTime;
}