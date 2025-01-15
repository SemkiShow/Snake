#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

unsigned int cellSize = 50;
unsigned int windowWidth = 16*50*2;
unsigned int windowHeight = 9*50*2;
unsigned int windowSize[2] = {windowWidth, windowHeight};
unsigned int horizontalCellsNumber = windowWidth / cellSize;
bool verticalSync = true;

char* board;

class Snake
{
    public:
        unsigned int position[2];
        std::vector<int> body;
        char direction = 'R';
        sf::Color color = sf::Color(100, 250, 50);
        std::vector<char> keyBuffer;
};

int main()
{
    // Non-SFML init
    Snake snake;
    snake.body.push_back(0);
    snake.body.push_back(1);
    snake.body.push_back(2);

    // SFML init
    sf::RenderWindow window(sf::VideoMode({windowSize[0], windowSize[1]}), "Snake");
    if (verticalSync)
    {
        window.setFramerateLimit(144);
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(0);
        window.setVerticalSyncEnabled(false);
    }

    sf::Clock deltaTimeClock;
    double deltaTime;
    sf::Clock delayClock;
    char lastKeyPress;
    while (window.isOpen())
    {
        deltaTime = deltaTimeClock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && lastKeyPress != 'U' && lastKeyPress != 'D')
        {
            lastKeyPress = 'U';
            snake.keyBuffer.push_back('U');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && lastKeyPress != 'D' && lastKeyPress != 'U')
        {
            lastKeyPress = 'D';
            snake.keyBuffer.push_back('D');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && lastKeyPress != 'L' && lastKeyPress != 'R')
        {
            lastKeyPress = 'L';
            snake.keyBuffer.push_back('L');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && lastKeyPress != 'R' && lastKeyPress != 'L')
        {
            lastKeyPress = 'R';
            snake.keyBuffer.push_back('R');
        }

        if (delayClock.getElapsedTime().asSeconds() >= 0.05)
        {
            delayClock.restart();

            // Change the snake's direction to the first key pressed in between the movement frames
            if (snake.keyBuffer.size() != 0)
            {
                snake.direction = snake.keyBuffer[0];
                snake.keyBuffer.erase(snake.keyBuffer.begin());
            }

            // Calculate the new head position
            int snakeDirectionInteger = 2;
            if (snake.direction == 'U') snakeDirectionInteger = -horizontalCellsNumber;
            if (snake.direction == 'D') snakeDirectionInteger = horizontalCellsNumber;
            if (snake.direction == 'L') snakeDirectionInteger = -1;
            if (snake.direction == 'R') snakeDirectionInteger = 1;
            // Move the head
            snake.body.push_back(snake.body[snake.body.size() - 1] + snakeDirectionInteger);
            // Remove the last tail piece
            snake.body.erase(snake.body.begin());
        }

        for (int i = 0; i < snake.body.size(); i++)
        {
            sf::RectangleShape bodyPiece({cellSize * 1.f, cellSize * 1.f});
            bodyPiece.setFillColor(snake.color);
            if (snake.body[i] > horizontalCellsNumber * windowHeight / cellSize)
                snake.body[i] = snake.body[i] % horizontalCellsNumber;
            bodyPiece.setPosition({(snake.body[i] % horizontalCellsNumber) * cellSize * 1.f, 
                (snake.body[i] / horizontalCellsNumber) * cellSize * 1.f});
            window.draw(bodyPiece);
        }

        std::cout << snake.body[snake.body.size()] << std::endl;

        window.display();
    }
}
