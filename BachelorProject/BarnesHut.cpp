#include "BarnesHut.h"

BarnesHut::BarnesHut(double theta, float g) : root(create_quad_tree_root_node()), theta(theta), g(g) { }

BarnesHut::~BarnesHut() {
    delete root;
}

BarnesHut::QuadtreeNode* BarnesHut::create_quad_tree_root_node() {
    BarnesHut::QuadtreeNode* node = new QuadtreeNode();
    node->bounds = sf::FloatRect(0.0f, 0.0f, 3000.0f, 2000.0f);
    for (int i = 0; i < 4; ++i) {
        node->children[i] = nullptr;
    }
    return node;
}

void BarnesHut::build_quadtree(std::vector<CelestialBody> bodies)
{
    for (auto& body : bodies) {
        insert_body_to_quadtree(root, &body);
    }
}

void BarnesHut::insert_body_to_quadtree(QuadtreeNode* node, CelestialBody* body)
{
    //If node x does not contain a body, put the new body b here.
    if (node->body == nullptr)
    {
        if (is_internal_node(node)) {
            //If node x is an internal node, update the center-of-mass and total mass of x. Recursively insert the body b in the appropriate quadrant.
            int q = get_quadrant_to_insert_node(node->bounds, body);
            insert_body_to_quadtree(node->children[q], body);
            node->totalMass = calculate_total_mass(node);
            node->centerOfMass = calculate_center_of_mass(node);
        }
        else {
            node->body = body;
            node->totalMass = calculate_total_mass(node);
            node->centerOfMass = calculate_center_of_mass(node);
        }
    }
    //If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
    //Subdivide the region further by creating four children. 
    //Then, recursively insert both b and c into the appropriate quadrant(s). 
    //Finally, update the center-of-mass and total mass of x.
    else if(node->body != nullptr)
    {
        create_quadrants(node);
        int q = get_quadrant_to_insert_node(node->bounds, node->body);
        insert_body_to_quadtree(node->children[q], node->body);
        node->body = nullptr;
        q = get_quadrant_to_insert_node(node->bounds, body);
        insert_body_to_quadtree(node->children[q], body);
        node->totalMass = calculate_total_mass(node);
        node->centerOfMass = calculate_center_of_mass(node);
    }
}

bool BarnesHut::is_internal_node(QuadtreeNode* node)
{
    if (node == nullptr)
        return false;
    if (node->children[0] != nullptr && node->children[1] != nullptr && node->children[2] != nullptr && node->children[3] != nullptr)
        return true;
    else
        return false;
}

float BarnesHut::calculate_total_mass(QuadtreeNode* node)
{
    if (!is_internal_node(node))
    {
        if(node->body != nullptr)
            return node->body->mass;
        else
            return 0.0f;
    }
    float mass = 0.0f;
    for (uint16_t i = 0; i <= 3; i++)
    {
        if (node->children[i]) 
        {
            mass += node->children[i]->totalMass;
        }
    }
    return mass;
}

sf::Vector2f BarnesHut::calculate_center_of_mass(QuadtreeNode* node)
{
    float x = 0.0f;
    float y = 0.0f;
    if (!is_internal_node(node))
    {
        x = node->body->position.x;
        y = node->body->position.y;
        return sf::Vector2f(x, y);
    }
   
    for (uint16_t i = 0; i <= 3; i++)
    {
        if (node->children[i]) 
        {
            x += node->children[i]->centerOfMass.x * node->children[i]->totalMass;
            y += node->children[i]->centerOfMass.y * node->children[i]->totalMass;
        }
    }

    if (node->totalMass != 0.0f)
    {
        x /= node->totalMass;
        y /= node->totalMass;
    }

    return sf::Vector2f(x, y);
}

int BarnesHut::get_quadrant_to_insert_node(sf::FloatRect bounds, CelestialBody* body)
{
    float centerX = bounds.left + bounds.width / 2.0f;
    float centerY = bounds.top + bounds.height / 2.0f;
    bool inLeft = body->position.x <= centerX;
    bool inTop = body->position.y <= centerY;

    if (inLeft && inTop) 
        return 0; // Top-left quadrant
    else if (!inLeft && inTop) 
        return 1; // Top-right quadrant
    else if (inLeft && !inTop) 
        return 2; // Bottom-left quadrant
    else
        return 3; // Bottom-right quadrant
}

void BarnesHut::create_quadrants(QuadtreeNode* node)
{
    sf::FloatRect bounds = node->bounds;
    float sub_width = bounds.width / 2.0f;
    float sub_height = bounds.height / 2.0f;

    sf::FloatRect quadrants[4] = 
    {
        sf::FloatRect(bounds.left, bounds.top, sub_width, sub_height),
        sf::FloatRect(bounds.left + sub_width, bounds.top, sub_width, sub_height),
        sf::FloatRect(bounds.left, bounds.top + sub_height, sub_width, sub_height),
        sf::FloatRect(bounds.left + sub_width, bounds.top + sub_height, sub_width, sub_height)
    };

    for (int i = 0; i < 4; ++i) 
    {
        node->children[i] = new QuadtreeNode();
        node->children[i]->bounds = quadrants[i];
    }
}

void BarnesHut::print_quadtree(QuadtreeNode* node, int depth) {
    if (node == nullptr)
        return;

    // Print indentation for the current depth
    for (int i = 0; i < depth; ++i)
        std::cout << "-";
    std::cout << "node center of mass: " << node->centerOfMass.x << "; " << node->centerOfMass.y << std::endl;

    // Print information about the body (if present)
    if (node->body != nullptr)
    {
        std::cout << "Body: " << node->body->name << std::endl;
    }
        

    // Recursively print children
    for (int i = 0; i < 4; ++i)
        print_quadtree(node->children[i], depth + 1);
}

sf::Vector2f BarnesHut::calculate_force_from_quadtree(CelestialBody* body)
{
    return calculate_force_from_node(root, body);
}

sf::Vector2f BarnesHut::calculate_force_from_node(QuadtreeNode* node, CelestialBody* body)
{
    if (node == nullptr)
        return sf::Vector2f(0.0f, 0.0f);
    //If the current node is an external node (and it is not body b), 
    //calculate the force exerted by the current node on b
    if (!is_internal_node(node) && (node->body != nullptr && node->body->name != body->name))
    {
       return calculate_force(body, node);
    }
    //Otherwise, calculate the ratio s/d. 
    //If s/d < theta, treat this internal node as a single body and calculate the force it exerts on body b
    else
    {
        if (calculate_distance_ratio(node, body) < theta)
        {
            return calculate_force(body, node); 
        }
        //Otherwise, run the procedure recursively on each of the current node’s children.
        else
        {
            sf::Vector2f force;
            for (int i = 0; i < 4; ++i)
                force += calculate_force_from_node(node->children[i], body);
            return force;
        }
    }
}

sf::Vector2f BarnesHut::calculate_force(CelestialBody* body, QuadtreeNode* node)
{
    sf::Vector2f distanceVector = sf::Vector2f(node->centerOfMass.x - body->position.x, node->centerOfMass.y - body->position.y);
    float vectorMagnitude = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
    if (vectorMagnitude <= 0.1f) {
        //avoid division by zero
        vectorMagnitude = 0.1f;
    }
    sf::Vector2f normalizedVector = distanceVector / vectorMagnitude;
    return (g * normalizedVector * body->mass * node->totalMass) / vectorMagnitude;
}

float BarnesHut::calculate_distance_ratio(QuadtreeNode* node, CelestialBody* body)
{
    //s / d
    //s = width
    //d = distance of body and nodes center of mass
    sf::Vector2f distanceVector = sf::Vector2f(body->position.x - node->centerOfMass.x, body->position.y - node->centerOfMass.y);
    float d = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
    return node->bounds.width / d;
}

void BarnesHut::destroy_quadtree()
{
    destroy_quadtree(root);
    root = create_quad_tree_root_node();
}

void BarnesHut::destroy_quadtree(QuadtreeNode* node)
{
    if (node == nullptr)
        return;

    for (int i = 0; i < 4; ++i)
        destroy_quadtree(node->children[i]);

    delete node;
}