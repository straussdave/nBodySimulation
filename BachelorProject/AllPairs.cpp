#include "AllPairs.h"

AllPairs::AllPairs(float G) : g(G) { }

sf::Vector2f AllPairs::calculate_force(CelestialBody* body, std::vector<CelestialBody> bodies)
{
	sf::Vector2f force;
	for (auto& otherBody : bodies)
	{
		if (otherBody.name != body->name)
		{
			sf::Vector2f distanceVector = sf::Vector2f(otherBody.position.x - body->position.x, otherBody.position.y - body->position.y);
			float vectorMagnitude = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
			vectorMagnitude += 0.01f;
			sf::Vector2f normalizedVector = distanceVector / vectorMagnitude;
			//g is 0 here because it doesnt get initialized properly. fix this
			force += (g * normalizedVector * body->mass * otherBody.mass) / pow(vectorMagnitude, 2.0f);
		}
	}
	return force;
}
