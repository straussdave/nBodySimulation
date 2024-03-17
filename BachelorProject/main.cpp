#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include "SimulationHandler.h"
#include "Renderer.h"

#define WIN_SIZE 1500, 1000

int main() 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE), "My window", sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);
    Renderer* renderer = Renderer::get_instance(window);

    SimulationHandler sim("planet_data.json", WIN_SIZE, window);

    sf::View view = window.getDefaultView();
    float zoomLevel = 1.0f; // Initial zoom level

    while (window.isOpen())
    {
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
            }
        }

        window.setView(view);
        renderer->clear();
        sim.draw_bodies();
        renderer->render();
    }
	return 0;
}