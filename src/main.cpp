#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Snake Game");

    const Game game(window);

    game.start();

    return 0;
}