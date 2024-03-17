#pragma once

#include <vector>
#include "CelestialBody.h"
#include "FileHelper.h"
#include "Renderer.h"
using json = nlohmann::json;

class SimulationHandler
{
public:
	SimulationHandler(const std::string& filename, int w, int h, sf::RenderWindow& win);
	void update_bodies();
	void print_bodies_positions();
	void draw_bodies();
	void scale(float scaleFactor);
private:
	Renderer* renderer;
	sf::RenderWindow& window;
	int height;
	int width;
	std::vector<CelestialBody> bodies;
	json planetData;
	void initialize(const std::string& filename);
};