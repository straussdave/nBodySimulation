#pragma once

#include <vector>
#include "CelestialBody.h"
#include "FileHelper.h"
#include "Renderer.h"
#include <cmath>
using json = nlohmann::json;

class SimulationHandler
{
public:
	SimulationHandler(const std::string& filename, int w, int h, sf::RenderWindow& win);
	void update_bodies(float dt);
	void print_bodies_positions();
	void draw_bodies();
private:
	Renderer* renderer;
	sf::RenderWindow& window;
	int height;
	int width;
	std::vector<CelestialBody> bodies;
	json planetData;
	void initialize(const std::string& filename);
	void naive_nbody(float dt);
	void fast_multipole();
	void barnes_hut();
};