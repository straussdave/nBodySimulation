#include "Renderer.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="window"></param>
Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

/// <summary>
/// Destructor
/// </summary>
Renderer::~Renderer()
{
}

/// <summary>
/// Gets the single instance of singleton class
/// </summary>
/// <param name="window"></param>
/// <returns></returns>
Renderer* Renderer::get_instance(sf::RenderWindow& window) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Renderer(window);
    }
    return pinstance_;
}

/// <summary>
/// Puts the drawable into the drawables vector
/// </summary>
/// <param name="drawable"></param>
void Renderer::add(sf::Drawable& drawable)
{
    drawablesVector.push_back(&drawable);
}

/// <summary>
/// Clears and renders all drawables in vector
/// </summary>
void Renderer::render()
{
    clear();
    for (auto& drawable : drawablesVector) {
        window.draw(*drawable);
    }
    window.display();
}

/// <summary>
/// Clears window
/// </summary>
void Renderer::clear()
{
    window.clear();
}
