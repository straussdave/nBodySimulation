#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <mutex>

class Renderer {
protected:
    Renderer(sf::RenderWindow& window);
    ~Renderer();

public:
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;
    static Renderer* get_instance(sf::RenderWindow& window);
    void add(sf::Drawable& drawable);
    void render();
    void clear();
private:
    static Renderer* pinstance_;
    static std::mutex mutex_;
    sf::RenderWindow& window;
    std::vector<sf::Drawable*> drawablesVector;
};
