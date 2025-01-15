#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

unsigned int horizontalCellsNumber = 32;
unsigned int windowWidth = 16*50*2;
unsigned int windowHeight = 9*50*2;
unsigned int windowSize[2] = {windowWidth, windowHeight};
unsigned int cellSize = windowWidth / horizontalCellsNumber;
bool verticalSync = true;

bool isGameOver = false;

class Snake
{
    public:
        std::vector<int> body;
        char direction = 'R';
        sf::Color color = sf::Color(100, 250, 50);
        std::vector<char> keyBuffer;
        double speed = 2;
};

class Apple
{
    public:
        unsigned int position;
        sf::Color color = sf::Color(255, 0, 0);
        unsigned int nutritionValue = 1;
};

int main()
{
    // Non-SFML init
    Snake snake;
    snake.body.push_back(0);
    snake.body.push_back(1);
    snake.body.push_back(2);
    snake.body.push_back(3);
    snake.body.push_back(4);
    srand(time(0));

    // SFML init
    sf::RenderWindow window(sf::VideoMode({windowSize[0], windowSize[1]}), "Snake");
    sf::Font font("./JetBrainsMonoNerdFont-Medium.ttf");
    std::vector<Apple> apples;
    apples.push_back(Apple());
    apples[0].position = rand() % (horizontalCellsNumber * windowHeight / cellSize);
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

        // Detect player input
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

        // Snake physics stuff
        if (delayClock.getElapsedTime().asSeconds() >= 1 / (snake.speed * sqrt(snake.body.size())) && !isGameOver)
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
            // Remove the last tail piece
            snake.body.erase(snake.body.begin());
            // Move the head
            snake.body.push_back(snake.body[snake.body.size()-1] + snakeDirectionInteger);

            // Check apple collision
            for (int i = 0; i < apples.size(); i++)
            {
                for (int j = 0; j < snake.body.size(); j++)
                {
                    if (snake.body[j] == apples[i].position)
                    {
                        apples[0].position = rand() % (horizontalCellsNumber * windowHeight / cellSize);
                        snake.body.push_back(snake.body[snake.body.size()-1] + snakeDirectionInteger);
                    }
                }
            }

            // Check collision with the walls
            if (snake.body[snake.body.size()-1]+horizontalCellsNumber < horizontalCellsNumber && snake.direction == 'U')
                snake.body[snake.body.size()-1] += horizontalCellsNumber * windowHeight / cellSize;
            if (snake.body[snake.body.size()-1] > (horizontalCellsNumber-1) * windowHeight / cellSize && snake.direction == 'D')
                snake.body[snake.body.size()-1] -= horizontalCellsNumber * windowHeight / cellSize;
            if ((snake.body[snake.body.size()-1]+1) % horizontalCellsNumber == 0 && snake.direction == 'L')
                snake.body[snake.body.size()-1] += horizontalCellsNumber;
            if (snake.body[snake.body.size()-1] % horizontalCellsNumber == 0 && snake.direction == 'R')
                snake.body[snake.body.size()-1] -= horizontalCellsNumber;

            // Check self-collision
            for (int i = 0; i < snake.body.size(); i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (snake.body[i] == snake.body[j] && i != j)
                    {
                        isGameOver = true;
                    }
                    if (isGameOver) break;
                }
                if (isGameOver) break;
            }
        }

        // Drawing the snake
        for (int i = 0; i < snake.body.size(); i++)
        {
            sf::RectangleShape bodyPiece({cellSize * 1.f, cellSize * 1.f});
            bodyPiece.setFillColor(snake.color);
            bodyPiece.setPosition({(snake.body[i] % horizontalCellsNumber) * cellSize * 1.f, 
                (snake.body[i] / horizontalCellsNumber) * cellSize * 1.f});
            window.draw(bodyPiece);
        }

        // Drawing the apples
        for (int i = 0; i < apples.size(); i++)
        {
            sf::RectangleShape apple({cellSize * 1.f, cellSize * 1.f});
            apple.setFillColor(apples[i].color);
            apple.setPosition({(apples[i].position % horizontalCellsNumber) * cellSize * 1.f, 
                (apples[i].position / horizontalCellsNumber) * cellSize * 1.f});
            window.draw(apple);
        }

        // Print game over is necessary
        if (isGameOver)
        {
            sf::Text gameOver(font);
            gameOver.setCharacterSize(0.135f * sqrt(windowWidth * windowWidth + windowHeight * windowHeight));
            gameOver.setPosition({windowWidth * .05f, windowHeight * .3f});
            gameOver.setFillColor(sf::Color(255, 0, 0));
            gameOver.setString("Game Over!");
            window.draw(gameOver);
        }

        // Print debug info
        // std::cout << snake.body.size() << std::endl;

        window.display();
    }
}
