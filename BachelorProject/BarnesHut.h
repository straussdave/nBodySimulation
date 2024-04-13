#pragma once
#include "CelestialBody.h"
class BarnesHut
{
	
public: 
	struct QuadtreeNode {
		sf::FloatRect bounds;
		float totalMass;
		sf::Vector2f centerOfMass;
		QuadtreeNode* children[4];
		CelestialBody* body;
	};
	BarnesHut(double theta, float g);
	~BarnesHut();
	void print_quadtree(QuadtreeNode* node, int depth = 0);
	void build_quadtree(std::vector<CelestialBody> bodies);
	sf::Vector2f calculate_force_from_quadtree(CelestialBody* body);
	void destroy_quadtree();
	QuadtreeNode* root;
private:
	double theta;
	float g;
	
	void insert_body_to_quadtree(QuadtreeNode* node, CelestialBody* body);
	QuadtreeNode* create_quad_tree_root_node();
	void destroy_quadtree(QuadtreeNode* node);
	bool is_internal_node(QuadtreeNode* node);
	float calculate_total_mass(QuadtreeNode* node);
	sf::Vector2f calculate_center_of_mass(QuadtreeNode* node);
	int get_quadrant_to_insert_node(sf::FloatRect bounds, CelestialBody* body);
	void create_quadrants(QuadtreeNode* node);
	sf::Vector2f calculate_force(CelestialBody* body, QuadtreeNode* node);
	float calculate_distance_ratio(QuadtreeNode* node, CelestialBody* body);
	sf::Vector2f calculate_force_from_node(QuadtreeNode* node, CelestialBody* body);
};

