#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

unsigned int cellSize = 50;
unsigned int windowWidth = 16*50*2;
unsigned int windowSize[2] = {windowWidth, 9*50*2};
unsigned int horizontalCellsNumber = windowWidth / cellSize;
bool verticalSync = true;

char* board;

class Snake
{
    public:
        unsigned int position[2];
        std::vector<int> body;
        char direction = 'L';
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && lastKeyPress != 'U')
        {
            lastKeyPress = 'U';
            snake.keyBuffer.push_back('U');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && lastKeyPress != 'D')
        {
            lastKeyPress = 'D';
            snake.keyBuffer.push_back('D');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && lastKeyPress != 'L')
        {
            lastKeyPress = 'L';
            snake.keyBuffer.push_back('L');
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && lastKeyPress != 'R')
        {
            lastKeyPress = 'R';
            snake.keyBuffer.push_back('R');
        }

        if (delayClock.getElapsedTime().asSeconds() >= 0.5)
        {
            delayClock.restart();
            snake.direction = snake.keyBuffer[0];
            snake.keyBuffer.
        }

        for (int i = 0; i < snake.body.size(); i++)
        {
            sf::RectangleShape bodyPiece({cellSize * 1.f, cellSize * 1.f});
            bodyPiece.setFillColor(snake.color);
            bodyPiece.setPosition({snake.body[i] * cellSize * 1.f, 0});
            window.draw(bodyPiece);
        }

        window.display();
    }
}
