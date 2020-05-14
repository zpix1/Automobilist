#include <SFML/Graphics.hpp>

#include "Game.h"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Automobilist", sf::Style::Default, settings);
    window.setFramerateLimit(80);
    window.setVerticalSyncEnabled(true);


    int result = 0;
    Game game(&window, &result);
    sf::Clock clock_fps;

#if (!DEBUG)
    srand(time(NULL));
#endif

    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;
    sf::Time ups = sf::seconds(1.f / 60.f);
    int state = 0;
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (state == 0)
            game.render_start_info();
        if (state == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            state++;
            clock.restart();
            continue;
        }
        if ((state == 2 || state == 3) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            window.close();
        }
        if (state == 1 && result == 0) {
            while (accumulator > ups) {
                accumulator -= ups;
                game.update(ups.asSeconds());
            }

            game.render(event);
            int cFPS = 1 / clock.getElapsedTime().asSeconds();

            game.fps = cFPS;

            accumulator += clock.restart();
        }
        if (state == 1 && result == 1) {
            state++;
            game.render_lost_info();
        }
        if (state == 1 && result == 2) {
            state++;
            game.render_won_info();
        }
    }

    return 0;
}