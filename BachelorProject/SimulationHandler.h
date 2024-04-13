#pragma once

#include <vector>
#include "CelestialBody.h"
#include "FileHelper.h"
#include "Renderer.h"
#include <cmath>
#include "BarnesHut.h"
using json = nlohmann::json;

class SimulationHandler
{

public:
	SimulationHandler(const std::string& filename, int w, int h, sf::RenderWindow& win);
	void update_bodies(float dt);
	void print_bodies_positions();
	void register_body_to_renderer();
	int save_results();
	
private:
	Renderer* renderer;
	sf::RenderWindow& window;
	int height;
	int width;
	std::vector<CelestialBody> bodies;
	json planetData;
	BarnesHut* bh;
	double theta;
	float g = 0.01f;
	std::vector<float> results;

	void initialize(const std::string& filename);

	void naive_nbody(float dt);
	void fast_multipole(float dt);

	void barnes_hut(float dt);

	
};