#pragma once
#include <cstdlib>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "log.h"

class Engine
{
public:
    sf::RenderWindow window;

    void run();
    void create(int w, int h, sf::String title, sf::Color clr_color);

    virtual void start() = 0;
    virtual void render(sf::RenderWindow *window) = 0;
    virtual void input(sf::Event *event) = 0;

    Engine() {}

private:
    sf::Color clear_color;
};

void Engine::create(int w, int h, sf::String title, sf::Color clr_color)
{
    window.create(sf::VideoMode(w, h), title);
    window.setVerticalSyncEnabled(true);
    clear_color = clr_color;
}

void Engine::run()
{
    log("starting engine");

    std::srand(time(NULL));

    start();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            input(&event);
        }

        window.clear(this->clear_color);
        render(&window);
        window.display();
    }
}