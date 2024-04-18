#pragma once
#include "CelestialBody.h"
class AllPairs
{
public:
	AllPairs(float G);
	sf::Vector2f calculate_force(CelestialBody* body, std::vector<CelestialBody> bodies);
private:
	float g;
	sf::Vector2f calculate_force(CelestialBody* body, CelestialBody otherBody);
};

