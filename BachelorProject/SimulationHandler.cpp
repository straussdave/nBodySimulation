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
    bh = new BarnesHut(0.5, g);
    ap = new AllPairs(g);
}

SimulationHandler::~SimulationHandler()
{
    delete bh;
    delete ap;
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

int SimulationHandler::save_results()
{
    FileHelper fh("results.txt");
    return fh.write_results_to_file(results);
}

/// <summary>
/// Calculates forces and applies them to all bodies
/// </summary>
void SimulationHandler::update_bodies(float dt)
{
    auto start_time = Clock::now();

    barnes_hut(dt);

    auto end_time = Clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
    float deltaTime = elapsed_time.count();
    results.push_back(deltaTime);
    renderer->render();
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
    // we first need g, to initialize the bodies with it
    for (auto& planet : planetData.items()) 
    {
        try 
        {
            const auto& key = planet.key();
            const auto& value = planet.value();
            if (key == "g")
            {
                g = value;
            }
        }
        catch(const std::exception& e){
            std::cerr << "Error: Did not find G" << ": " << e.what() << std::endl;
        }
    }
    for (auto& planet : planetData.items())
    {
        try
        {
            const auto& key = planet.key();
            const auto& value = planet.value();
            if (key != "g")
            {
                const auto& key = planet.key();
                const auto& value = planet.value();
                float mass = static_cast<float>(value["Mass"].get<double>());
                float diameter = static_cast<float>(value["Diameter"].get<double>());
                float distanceFromSun = static_cast<float>(value["Distance from Sun"].get<double>());
                float orbitalVelocity = static_cast<float>(value["Orbital Velocity"].get<double>());
                int maxPos = value["Max Positions stored"].get<int>();
                // Extract color components from JSON
                int red = value["Color"][0].get<int>();
                int green = value["Color"][1].get<int>();
                int blue = value["Color"][2].get<int>();
                int alpha = value["Color"][3].get<int>();

                // Construct sf::Color object
                sf::Color color(red, green, blue, alpha);
                sf::Vector2f position = sf::Vector2f(static_cast<float>(widthMid), static_cast<float>(heightMid));
                position.x += distanceFromSun;
                CelestialBody body(key, mass, diameter, distanceFromSun, sf::Vector2f(0, orbitalVelocity), position, maxPos, color, g);
                bodies.emplace_back(body);
            }
            
        }
        catch (const std::exception& e) {
            std::cerr << "Error:Could not create body" << ": " << e.what() << std::endl;
        }
    }
}

void SimulationHandler::naive_nbody(float dt) 
{
    for (auto& body : bodies) 
    {
        body.apply_force(ap->calculate_force(&body, bodies), dt);
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

    for (auto& body : bodies) {
        body.apply_force(bh->calculate_force_from_quadtree(&body), dt);
    }

    bh->destroy_quadtree();

    for (auto& body : bodies)
    {
        body.update_position(dt);
    }
}
