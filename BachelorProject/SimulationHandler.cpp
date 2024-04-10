#include "SimulationHandler.h"
Renderer* Renderer::pinstance_{ nullptr };
std::mutex Renderer::mutex_;
using Clock = std::chrono::steady_clock;

/// <summary>
/// Constructor
/// </summary>
/// <param name="filename"></param>
SimulationHandler::SimulationHandler(const std::string& filename, int w, int h, sf::RenderWindow& win)
    : width(w), height(h), window(win)
{
    initialize(filename);
    renderer = Renderer::get_instance(window);
    register_body_to_renderer();
    bh = new BarnesHut(theta);
    barnes_hut(0.1f);
}

/// <summary>
/// Prints every bodies position to console
/// </summary>
void SimulationHandler::print_bodies_positions()
{
	for (auto& body : bodies)
	{
        std::cout << body.name << ": " << body.position.x << "; " << body.position.y << std::endl;
	}
}

/// <summary>
/// Sends bodies to renderer
/// </summary>
void SimulationHandler::register_body_to_renderer()
{
    for (auto& body : bodies) 
    {
        renderer->add(body);
    }
}

/// <summary>
/// Calculates forces and applies them to all bodies
/// </summary>
void SimulationHandler::update_bodies(float dt)
{
}

/// <summary>
/// Creates all celestial bodies with mass, diameter, distance from sun, velocity and direction
/// </summary>
void SimulationHandler::initialize(const std::string& filename)
{
	FileHelper fh(filename);
	planetData = fh.get_planet_data();
    int heightMid = height / 2;
    int widthMid = width / 2;
    for (auto& planet : planetData.items()) 
    {
        try 
        {
            const auto& planetName = planet.key();
            const auto& planetInfo = planet.value();
            
            float mass = static_cast<float>(planetInfo["Mass"].get<double>());
            float diameter = static_cast<float>(planetInfo["Diameter"].get<double>());
            float distanceFromSun = static_cast<float>(planetInfo["Distance from Sun"].get<double>());
            float orbitalVelocity = static_cast<float>(planetInfo["Orbital Velocity"].get<double>());
            int maxPos = planetInfo["Max Positions stored"].get<int>();
            // Extract color components from JSON
            int red = planetInfo["Color"][0].get<int>();
            int green = planetInfo["Color"][1].get<int>();
            int blue = planetInfo["Color"][2].get<int>();
            int alpha = planetInfo["Color"][3].get<int>();

            // Construct sf::Color object
            sf::Color color(red, green, blue, alpha);
            sf::Vector2f position = sf::Vector2f(static_cast<float>(widthMid), static_cast<float>(heightMid));
            position.x += distanceFromSun;
            CelestialBody body(planetName, mass, diameter, distanceFromSun, sf::Vector2f(0, orbitalVelocity), position, maxPos, color);
            bodies.emplace_back(body);
        }
        catch(const std::exception& e){
            std::cerr << "Error: Could not create Object" << ": " << e.what() << std::endl;
        }
    }
}

void SimulationHandler::naive_nbody(float dt) 
{
    for (auto& body : bodies) 
    {
        body.calculate_force(dt, bodies);
    }
    for (auto& body : bodies)
    {
        body.update_position(dt);
    }
}

void SimulationHandler::fast_multipole(float dt)
{
}

void SimulationHandler::barnes_hut(float dt)
{
    bh->build_quadtree(bodies);
    //for (auto& body : bodies) {
    //    sf::Vector2f force = calculate_force_from_quadtree(root, &body);
    //    body.apply_force(force, dt);
    //}
}
