#include <iostream>
#include <random>
#include <optional>
#include <queue>
#include <cmath>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// const game vars
const Vector2f SQUARE_SIZE = { 30.f, 30.f };
const float UNIT_SIZE = 30.f;
const int INT_UNIT_SIZE = 30;

const float BASE_SPEED = 120.f;
const float SPEED_MULT = 1.5f;

// Enums

enum class Direction {Left, Right, Up, Down };
ostream& operator<<(std::ostream& os, Direction d) {
    switch (d) {
    case Direction::Up:   return os << "Up";
    case Direction::Down:   return os << "Down";
    case Direction::Left:   return os << "Left";
    case Direction::Right:   return os << "Right";
    }
    return os;
}

// Structs

struct NextMove {
    Direction direction;
    Vector2f coord;
};

// Random device
random_device rd;
mt19937 gen(rd());

// Func stubs

// Check next direction validity, return true if next direction is opposite of current direction
bool ValidTurnDirection(Direction current, Direction next);
// Get the direction vector based on the set current direction
Vector2f GetDirectionVec(Direction direction);
// Get the direction vector based on the opposite of the set current direction
Vector2f GetOppositeDirectionVec(Direction direction);

// Classes

class SnakeBlock : public RectangleShape {
    private:
        Direction currDirection;

        optional<SnakeBlock*> nextBlock;
        queue<NextMove> nextMoves;
    public:
        // Constructors

        // default pos + dir
        SnakeBlock() {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Green);
            setPosition({ 100.f, 100.f } );
            setOutlineColor(Color::White);
            setOutlineThickness(2.f);
            currDirection = Direction::Right;
        }
        // default dir
        SnakeBlock(Vector2f initPosition) {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Green);
            setPosition(initPosition);
            setOutlineColor(Color::White);
            setOutlineThickness(2.f);
            currDirection = Direction::Right;
        }
        // default pos
        SnakeBlock(Direction direction) {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Green);
            setPosition( { 100.f, 100.f } );
            setOutlineColor(Color::White);
            setOutlineThickness(2.f);
            currDirection = direction;
        }
        // passed pos + dir
        SnakeBlock(Vector2f initPosition, Direction direction) {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Green);
            setPosition(initPosition);
            setOutlineColor(Color::White);
            setOutlineThickness(2.f);
            currDirection = direction;
        }

        // methods
        void PopNextMoves() {
            nextMoves.pop();
        }
        void PushNextMoves(NextMove in) {
            nextMoves.push(in);
        }

        // Getters
        Direction GetCurrDirection() { return currDirection; }
        optional<SnakeBlock*> GetNextBlock() { return nextBlock; }
        queue<NextMove> GetNextMoves() { return nextMoves; }


        // Setters
        void SetCurrDirection(Direction direction) { currDirection = direction; }
        void SetNextBlock(SnakeBlock* next) { nextBlock = next; }
        void SetNextMoves(queue<NextMove> in) { nextMoves = in; }
};

class Snake {
    private:
        bool alive;
        int tailLength;
        float speed;

        SnakeBlock* head;
        SnakeBlock* tail;

    public:
        // Constructors
        
        // default pos + speed
        Snake() {
            alive = true;
            tailLength = 0;
            speed = BASE_SPEED;

            SnakeBlock* tempBlockPtr = new SnakeBlock();
            head = tempBlockPtr;
            tail = tempBlockPtr;
        }
        // default speed
        Snake(Vector2f position) {
            alive = true;
            tailLength = 0;
            speed = BASE_SPEED;

            SnakeBlock* tempBlockPtr = new SnakeBlock(position);
            head = tempBlockPtr;
            tail = tempBlockPtr;
        }
        // default pos
		Snake(float speed) {
            alive = true;
			tailLength = 0;
            this->speed = speed;

            SnakeBlock* tempBlockPtr = new SnakeBlock();
            head = tempBlockPtr;
            tail = tempBlockPtr;
		}
        // passed pos + speed
        Snake(Vector2f position, float speed) {
            alive = true;
            tailLength = 0;
            this->speed = speed;

            SnakeBlock* tempBlockPtr = new SnakeBlock(position);
            head = tempBlockPtr;
            tail = tempBlockPtr;
        }

        // Methods
        void extend() {
            tailLength++;

            Vector2f newTailPosition = tail->getPosition() + GetOppositeDirectionVec(tail->GetCurrDirection()) * UNIT_SIZE;
            SnakeBlock* tempBlockPtr = new SnakeBlock(newTailPosition, tail->GetCurrDirection());
            if (!tail->GetNextMoves().empty()) {
                //cout << "in" << endl;
                tempBlockPtr->SetNextMoves(tail->GetNextMoves());
            }
            //cout << "tail address b4: " << &tail << endl;
            //cout << "tail next b4: " << &tail << endl;
            tail->SetNextBlock(tempBlockPtr);
            //cout << "tail next: " << tail->GetNextBlock().value() << endl;
            //cout << "tempBlock address: " << &tempBlock << endl;
            tail = tempBlockPtr;
            //cout << "new tail at: " << tail << endl;

        }

        void move(float dt) {
            // move head
            head->setPosition(head->getPosition() + GetDirectionVec(head->GetCurrDirection()) * speed * dt);
            if (tailLength <= 0) { return; }

            // move body
            SnakeBlock* iterBlock = head->GetNextBlock().value();
            while (true) {
                Direction currDirection = iterBlock->GetCurrDirection();
                Vector2f currPos = iterBlock->getPosition();
                //cout << "currently: " << currDirection << ", {" << currPos.x << ", " << currPos.y << "} }" << endl;
                
                //update next
                //cout << iterBlock->GetNextMoves().empty() << endl;
                if (!iterBlock->GetNextMoves().empty()) {
                    auto [nextDirection, turnPos] = iterBlock->GetNextMoves().front();
                    //cout << "nextMove is: { " << nextDirection << ", {" << turnPos.x << ", " << turnPos.y << "} }" << endl;

                    // set up if logic - determine working with x or y and when curr coords "pass" the turn coords
                    bool isVertical = (currDirection == Direction::Up || currDirection == Direction::Down);
                    bool isPositive = (currDirection == Direction::Down || currDirection == Direction::Right);

                    // extract x or y vals for logic
                    float currCoord = isVertical ? currPos.y : currPos.x;
                    float turnCoord = isVertical ? turnPos.y : turnPos.x;

                    float distanceToTurn = isPositive ? turnCoord - currCoord : currCoord - turnCoord;
                    float remainingDistance = speed * dt - distanceToTurn;

                    if(distanceToTurn <= speed * dt) {
                        iterBlock->setPosition(
                            Vector2f{
                                iterBlock->getPosition() + 
                                GetDirectionVec(currDirection) * distanceToTurn + 
                                GetDirectionVec(nextDirection) * remainingDistance
                            }
                        );
                        iterBlock->SetCurrDirection(nextDirection);
                        // pop
                        iterBlock->PopNextMoves();
                    }
                    else {
                        iterBlock->setPosition(iterBlock->getPosition() + GetDirectionVec(iterBlock->GetCurrDirection()) * speed * dt);
                    }
                }
                else {
                    iterBlock->setPosition(iterBlock->getPosition() + GetDirectionVec(iterBlock->GetCurrDirection()) * speed * dt);
                }
                // loop to next block or break if tail
                if (iterBlock == tail) { break; }
                iterBlock = iterBlock->GetNextBlock().value();
            }
        }

        void turn(Direction turnDirection) {
            Vector2f currHeadCoords = head->getPosition();

            bool isVertical = (turnDirection == Direction::Up || turnDirection == Direction::Down);
            bool isPositive = (turnDirection == Direction::Down || turnDirection == Direction::Right);
            Vector2f roundedCoords = isVertical ? Vector2f{(isPositive ? ceil(currHeadCoords.x) : floor(currHeadCoords.x)), currHeadCoords.y} : Vector2f{currHeadCoords.x, (isPositive ? ceil(currHeadCoords.y) : floor(currHeadCoords.y))};

            head->SetCurrDirection(turnDirection);
            head->setPosition(roundedCoords);

            if (tailLength <= 0) { return; }

            // add next moves to tail blocks
            NextMove nextMove = { turnDirection, roundedCoords };
            SnakeBlock* iterBlock = head->GetNextBlock().value();
            //cout << "setting nextMove to: { " << turnDirection << ", {" << currHeadCoords.x << ", " << currHeadCoords.y << "} }" << endl;

            while(true) {
                iterBlock->PushNextMoves(nextMove);
                //cout << iterBlock->GetNextMoves().empty() << endl;
                if (iterBlock == tail) { break; }
                iterBlock = iterBlock->GetNextBlock().value();
            }
        }

        void kill() {
            alive = false;

            SnakeBlock* tempBlockPtr = head;
            while (true) {
                tempBlockPtr->setFillColor(Color::Blue);
                if (!tempBlockPtr->GetNextBlock().has_value() || tempBlockPtr->GetNextBlock().value() == nullptr) { return; }
                tempBlockPtr = tempBlockPtr->GetNextBlock().value();
            }
        }

        void increaseSpeed() {
            speed *= SPEED_MULT;
        }
        void decreaseSpeed() {
            speed /= SPEED_MULT;
        }

        // Getters
        bool GetAlive() { return alive; }
        int GettailLength() { return tailLength; }
        float GetSpeed() { return speed; }

        SnakeBlock* GetHead() { return head; }
        SnakeBlock* GetTail() { return tail; }

        // Setters
        void SetAlive(bool in) { alive = in; }
        void SettailLength(bool in) { tailLength = in; }
        void SetSpeed(float in) { speed = in; }
};

class Apple : public RectangleShape {
    private:
        bool exists;

    public:        
        // Constructors

        // Default pos
        Apple() {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Red);
            setPosition({ 200.f, 100.f });
            exists = true;
        }

        // Passed pos
        Apple(Vector2f initPosition) {
            setSize(SQUARE_SIZE);
            setFillColor(Color::Red);
            setPosition(initPosition);
            exists = true;
        }

        // Getters
        bool GetExists() { return this->exists; }

        // Setters
        void SetExists(bool in) { this->exists = in; }
};

int main()
{
    // Create the main window
    RenderWindow window(VideoMode({ 1280, 720 }), "Snake Clone");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

    auto windowSize = Vector2f(window.getSize());

    uniform_int_distribution<int> randomX(0, (int)(windowSize.x / INT_UNIT_SIZE) - 1);
    uniform_int_distribution<int> randomY(0, (int)(windowSize.y / INT_UNIT_SIZE) - 1);

    //TODO extract logic and make this a simple gameplay loop checking for input then passing it off to logic (add R for reset)

    Snake snake;

    Apple apple;

    Clock clock;

    // Start the game loop
    while (window.isOpen() && snake.GetAlive())
    {
		// Calculate elapsed time since last frame
        float dt = clock.restart().asSeconds();

        // Process events
        while (const optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<Event::Closed>()) {
                window.close();
        }

            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                optional<Direction> next;

                switch (key->code) {
                // L< or 'A' for left
                case Keyboard::Key::Left:
                    next = Direction::Left;
                    break;
                case Keyboard::Key::A:
                    next = Direction::Left;
                    break;
                // R> or 'D' for right
                case Keyboard::Key::Right:
                    next = Direction::Right;
                    break;
                case Keyboard::Key::D:
                    next = Direction::Right;
                    break;
                // U^ or 'W' for up
                case Keyboard::Key::Up:
                    next = Direction::Up;
                    break;
                case Keyboard::Key::W:
                    next = Direction::Up;
                    break;
                // D`' or 'S' for down
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
                default:
                    break;
                }

                if (next.has_value() && ValidTurnDirection((snake.GetHead()->GetCurrDirection()), next.value())) {
                    snake.turn(next.value());
                }
            }
        }
        
        if(snake.GetAlive()) {
            snake.move(dt);
        }
		
        if (const auto intersection = snake.GetHead()->getGlobalBounds().findIntersection(apple.getGlobalBounds()) && apple.GetExists()) {
            // Collision detected!
            // You can also access the overlap dimensions via intersection->size or intersection->position

            // Generate and set new coords for apple
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

bool ValidTurnDirection(Direction current, Direction next) {
    return !(
            (current == Direction::Left && next == Direction::Right) ||
            (current == Direction::Right && next == Direction::Left) ||
            (current == Direction::Up && next == Direction::Down) ||
            (current == Direction::Down && next == Direction::Up) ||
            (current == next)
        );
}

Vector2f GetDirectionVec(Direction direction) {
    switch (direction) {
        // left
    case Direction::Left:
        return { -1.f, 0.f };
        // right
    case Direction::Right:
        return { 1.f, 0.f };
        // up
    case Direction::Up:
        return { 0.f, -1.f };
        // down
    case Direction::Down:
        return { 0.f, 1.f };
    }

    return { 0.f, 0.f };
}

Vector2f GetOppositeDirectionVec(Direction direction) {
    switch (direction) {
        // left
    case Direction::Left:
        return { 1.f, 0.f };
        // right
    case Direction::Right:
        return { -1.f, 0.f };
        // up
    case Direction::Up:
        return { 0.f, 1.f };
        // down
    case Direction::Down:
        return { 0.f, -1.f };
    }

    return { 0.f, 0.f };
}

void EatApple(Apple& eatenApple) {
    
}