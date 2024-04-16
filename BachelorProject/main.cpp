#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "SimulationHandler.h"
#include "Renderer.h"
#include <chrono>

#define WIN_SIZE 1500, 1000
using Clock = std::chrono::steady_clock;

int main() 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE), "My window", sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);

    SimulationHandler sim("100bodies.json", WIN_SIZE, window);
    
    sf::View view = window.getDefaultView();
    float zoomLevel = 1.0f; // Initial zoom level
    auto start_time = Clock::now();
    auto end_time = Clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
    float deltaTime = elapsed_time.count();
    while (window.isOpen())
    {
        start_time = Clock::now();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::MouseButtonPressed)
                sim.print_bodies_positions();
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                zoomLevel *= 1 - event.mouseWheelScroll.delta * 0.1f;
                zoomLevel = std::max(0.1f, std::min(zoomLevel, 10.0f)); //clamp zoom lvl
                view.setSize(window.getDefaultView().getSize().x * zoomLevel, window.getDefaultView().getSize().y * zoomLevel);
                window.setView(view);
            }
        }
        
        sim.update_bodies(deltaTime);
        end_time = Clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(end_time - start_time);
        deltaTime = elapsed_time.count(); //the delta time is always delayed by one loop, but thats ok
    }

    sim.save_results();

	return 0;
}