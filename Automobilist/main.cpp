#include <SFML/Graphics.hpp>

#include "Game.h"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Automobilist", sf::Style::Default, settings);
    window.setFramerateLimit(80);
    window.setVerticalSyncEnabled(true);


    Game game(&window);
    sf::Clock clock_fps;

#if (!DEBUG)
    srand(time(NULL));
#endif
    

    /*while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        while (clock_fps.getElapsedTime().asSeconds() > step) {
            game.update(step);
            clock_fps.restart();
        }
        game.render(event);

        printf("%f\r", 1.0 / clock_fps.getElapsedTime().asSeconds());
    }*/

    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;
    sf::Time ups = sf::seconds(1.f / 60.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        while (accumulator > ups) {
            accumulator -= ups;
            game.update(ups.asSeconds());
        }

        game.render(event);
        int cFPS = 1 / clock.getElapsedTime().asSeconds();

        game.fps = cFPS;

        accumulator += clock.restart();
    }

    return 0;
}