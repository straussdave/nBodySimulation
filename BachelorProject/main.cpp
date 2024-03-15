#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::CircleShape shape(50.f);

    // set the shape color to green
    shape.setFillColor(sf::Color(100, 250, 50));
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        shape.setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        window.clear();
        window.draw(shape);
        window.display();
        std::cout << shape.getPosition().x << ';' << shape.getPosition().y << std::endl;
    }
	return 0;
}