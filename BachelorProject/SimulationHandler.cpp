#include "SimulationHandler.h"
Renderer* Renderer::pinstance_{ nullptr };
std::mutex Renderer::mutex_;

/// <summary>
/// Constructor
/// </summary>
/// <param name="filename"></param>
SimulationHandler::SimulationHandler(const std::string& filename, int w, int h, sf::RenderWindow& win)
    : width(w), height(h), window(win)
{
    initialize(filename);
    renderer = Renderer::get_instance(window);
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
/// Sends bodies positions and size to renderer
/// </summary>
void SimulationHandler::draw_bodies()
{
    for (auto& body : bodies) 
    {
        renderer->add(body);
    }
}

/// <summary>
/// Scales all bodies up or down
/// </summary>
/// <param name="scaleFactor"></param>
void SimulationHandler::scale(float scaleFactor)
{
    for (auto& body : bodies)
    {
        body.setScale(scaleFactor);
    }
}

/// <summary>
/// Calculates forces and applies them to all bodies
/// </summary>
void SimulationHandler::update_bodies()
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
    for (auto& planet : planetData.items()) {
        try {
            const auto& planetName = planet.key();
            const auto& planetInfo = planet.value();
            
            double mass = planetInfo["Mass"].get<double>();
            double diameter = planetInfo["Diameter"].get<double>();
            double distanceFromSun = planetInfo["Distance from Sun"].get<double>();
            double orbitalVelocity = planetInfo["Orbital Velocity"].get<double>();
            sf::Vector2f position = sf::Vector2f(static_cast<float>(widthMid), static_cast<float>(heightMid));
            position.x += distanceFromSun*100;
            if (planetName != "Sun")
                diameter = 5;
            diameter *= 2;
            CelestialBody body(planetName, mass, diameter, distanceFromSun, orbitalVelocity, position);
            bodies.emplace_back(body);
        }
        catch(const std::exception& e){
            std::cerr << "Error: Could not create Object" << ": " << e.what() << std::endl;
        }
    }
}