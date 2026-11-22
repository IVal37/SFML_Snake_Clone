#include "Game.h"

#include <optional>

#include "Utils.h"
#include "GameConfig.h"

using namespace sf;
using namespace std;

Game::Game() : gen(random_device{}()) {
	window = RenderWindow(VideoMode({ 1280, 720 }), "Snake Clone");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	windowSize = Vector2f(window.getSize());
}

void Game::run() {
    while (window.isOpen() && snake.GetAlive())
    {
        // Calculate elapsed time since last frame
        float dt = clock.restart().asSeconds();

        // Process events
        while (const auto event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                optional<Direction> next;

                switch (key->code) {
                    // 'L' or 'A' for left
                    case Keyboard::Key::Left:
                        next = Direction::Left;
                        break;
                    case Keyboard::Key::A:
                        next = Direction::Left;
                        break;
                    // 'R' or 'D' for right
                    case Keyboard::Key::Right:
                        next = Direction::Right;
                        break;
                    case Keyboard::Key::D:
                        next = Direction::Right;
                        break;
                    // 'U' or 'W' for up
                    case Keyboard::Key::Up:
                        next = Direction::Up;
                        break;
                    case Keyboard::Key::W:
                        next = Direction::Up;
                        break;
                    // 'D' or 'S' for down
                    case Keyboard::Key::Down:
                        next = Direction::Down;
                        break;
                    case Keyboard::Key::S:
                        next = Direction::Down;
                        break;
                    // 'Esc' to quit
                    case Keyboard::Key::Escape:
                        window.close();
                        break;
                    //TODO add 'r' for reset
                    default:
                        break;
                }

                if (next.has_value() && ValidTurnDirection((snake.GetHead()->GetCurrDirection()), next.value())) {
                    snake.turn(next.value());
                }
            }
        }

        if (snake.GetAlive()) {
            snake.move(dt);
        }

        if (const auto intersection = snake.GetHead()->getGlobalBounds().findIntersection(apple.getGlobalBounds()) && apple.GetExists()) {
            // Collision detected!
            // You can also access the overlap dimensions via intersection->size or intersection->position

            // Generate and set new coords for apple
            uniform_int_distribution<int> randomX(0, (int)(windowSize.x / INT_UNIT_SIZE) - 1);
            uniform_int_distribution<int> randomY(0, (int)(windowSize.y / INT_UNIT_SIZE) - 1);
            Vector2f newRandPos = {
                (UNIT_SIZE * (float)(randomX(gen))),
                (UNIT_SIZE * (float)(randomY(gen)))
            };
            // TODO check new pos no overlap w snake
            apple.setPosition(newRandPos);

            // Add tail block
            snake.extend();

            // Increase speed
            snake.increaseSpeed();
        }

        auto bounds = snake.GetHead()->getGlobalBounds();
        if (bounds.position.x <= 0 ||
            bounds.position.y <= 0 ||
            bounds.position.x + bounds.size.x >= windowSize.x ||
            bounds.position.y + bounds.size.y >= windowSize.y)
        {
            snake.kill();
            cout << " You Died" << endl;
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        SnakeBlock* tempBlock = snake.GetHead();
        while (true) {
            //cout << "drawing tempBlock with address: " << tempBlock << endl;
            window.draw(*tempBlock);
            //cout << "drew it" << endl;
            //cout << "has_value(): " << tempBlock->GetNextBlock().has_value() << endl;
            if (!tempBlock->GetNextBlock().has_value()) { break; }
            //cout << "assigning: " << tempBlock->GetNextBlock().value() << endl;
            tempBlock = tempBlock->GetNextBlock().value();
            //cout << "tempBlock pointing to: " << tempBlock << endl;
        }
        window.draw(apple);

        // Update the window
        window.display();
    }
}
