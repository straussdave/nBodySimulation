#include "BarnesHut.h"

BarnesHut::BarnesHut(double theta) : root(create_quad_tree_root_node()), theta(theta) { }

BarnesHut::~BarnesHut() {
    delete root;
}

BarnesHut::QuadtreeNode* BarnesHut::create_quad_tree_root_node() {
    BarnesHut::QuadtreeNode* node = new QuadtreeNode();
    node->bounds = sf::FloatRect(0.0f, 0.0f, 3000, 2000);
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
    print_quadtree(root, 0);
}

void BarnesHut::insert_body_to_quadtree(QuadtreeNode* node, CelestialBody* body)
{
    //If node x does not contain a body, put the new body b here.
    if (node->body == nullptr)
    {
        if (is_internal_node(node)) {
            int q = get_quadrant_to_insert_node(node->bounds, body);
            insert_body_to_quadtree(node->children[q], body);
            node->totalMass = calculate_total_mass(node);
            node->centerOfMass = calculate_center_of_mass(node);
        }
        else {
            node->body = body;
        }
    }
    //If node x is an internal node, update the center-of-mass and total mass of x. Recursively insert the body b in the appropriate quadrant.
    else if (is_internal_node(node))
    {
        int q = get_quadrant_to_insert_node(node->bounds, body);
        insert_body_to_quadtree(node->children[q], body);
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
    }
    node->totalMass = calculate_total_mass(node);
    node->centerOfMass = calculate_center_of_mass(node);
}

bool BarnesHut::is_internal_node(QuadtreeNode* node)
{
    if (node->children[0] != nullptr && node->children[1] != nullptr && node->children[2] != nullptr && node->children[3] != nullptr)
        return true;
    else
        return false;
}

float BarnesHut::calculate_total_mass(QuadtreeNode* node)
{
    float mass = 0.0f;
    for (uint16_t i = 0; i <= 3; i++)
    {
        if (node->children[i] && node->children[i]->body) 
        {
            mass += node->children[i]->body->mass;
        }
    }
    return mass;
}

sf::Vector2f BarnesHut::calculate_center_of_mass(QuadtreeNode* node)
{
    float x = 0.0f;
    float y = 0.0f;
    for (uint16_t i = 0; i <= 3; i++)
    {
        if (node->children[i] && node->children[i]->body) 
        {
            x += node->children[i]->body->getPosition().x * node->children[i]->body->mass;
            y += node->children[i]->body->getPosition().y * node->children[i]->body->mass;
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

    
    

    // Print information about the body (if present)
    if (node->body != nullptr)
    {
        // Print indentation for the current depth
        for (int i = 0; i < depth; ++i)
            std::cout << "-";
        std::cout << "Body: " << node->body->name << std::endl;
    }
        

    // Recursively print children
    for (int i = 0; i < 4; ++i)
        print_quadtree(node->children[i], depth + 1);
}