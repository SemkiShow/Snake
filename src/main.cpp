#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Settings.hpp"
#include "UI.hpp"
#include "Game.hpp"

Snake snake;
std::vector<Apple> apples;
Settings settings;

int main()
{
    // Non-SFML init
    Restart();
    srand(time(0));
    settings.Load("settings.txt");
    
    // SFML init
    bgMusic.setLooping(true);
    bgMusic.setVolume(50.f);
    bgMusic.play();
    pauseMusic.setLooping(true);
    pauseMusic.setVolume(50.f);
    backgroundTexture.setSmooth(true);
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(settings.vsync);
    
    // ImGUI init
    (void) ImGui::SFML::Init(window);
    
    sf::Clock deltaTimeClock;
    sf::Time deltaTime;
    sf::Clock delayClock;
    int previousScale = cellSize;
    int previousApplesNumber = settings.applesNumber;
    bool previousAutoMode = settings.autoMode;
    bool previousVsync = settings.vsync;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                {
                    isPaused = !isPaused;
                    if (isPaused)
                    {
                        bgMusic.pause();
                        pauseMusic.play();
                    }
                    else
                    {
                        pauseMusic.pause();
                        bgMusic.play();
                    }
                }
            }
        }
        deltaTime = deltaTimeClock.restart();
        ImGui::SFML::Update(window, deltaTime);
        
        // Draw ImGUI GUI
        ShowMenuBar();
        if (isSettings) ShowSettings(&isSettings);
        
        window.clear();

        // Detect player input
        ProcessPlayerInput();

        // Snake physics stuff
        if ((delayClock.getElapsedTime().asSeconds() >= std::max(1 / (snake.speed * sqrt(snake.body.size())), 1 / snake.maxSpeed) || settings.noSpeedLimit) && !isGameOver && !isPaused)
        {
            delayClock.restart();

            // Add keypresses to a keybuffer if it's empty and autoMode is true
            std::string buf;
            if (snake.keyBuffer.size() == 0 && settings.autoMode == true)
            {
                buf = GenerateAutoModeKeypresses();
                for (int i = 0; i < buf.size(); i++)
                    snake.keyBuffer.push_back(buf[i]);
            }

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
            snake.lastTailPosition = snake.body[0];
            // Remove the last tail piece
            snake.body.erase(snake.body.begin());
            // Move the head
            snake.body.push_back(snake.body[snake.body.size()-1] + snakeDirectionInteger);

            // Check apple collision
            snake.CheckAppleCollision();

            // Check collision with the walls
            snake.CheckWallCollision();

            // Check self-collision
            snake.CheckSelfCollision();
        
            // China mode check
            if (snake.body.size() >= settings.funModeLevel && mode != "china" && settings.funMode == true)
            {
                mode = "china";
                bgMusic.stop();
                (void) bgMusic.openFromFile("assets/china-bg.ogg");
                bgMusic.setVolume(200.f);
                bgMusic.play();
            }
            if (!(snake.body.size() >= settings.funModeLevel && settings.funMode == true) && mode != "normal")
            {
                mode = "normal";
                bgMusic.stop();
                (void) bgMusic.openFromFile("assets/snake-bg.mp3");
                bgMusic.setVolume(50.f);
                bgMusic.play();
            }

            // Update data fom settings
            snake.speed = settings.speed * 1.0;
            snake.maxSpeed = settings.maxSpeed * 1.0;
            cellSize = settings.scale;
            horizontalCellsNumber = floor(windowSize[0] / cellSize);
            if (settings.applesNumber != previousApplesNumber)
            {
                previousApplesNumber = settings.applesNumber;
                apples.clear();
                for (int i = 0; i < settings.applesNumber; i++)
                {
                    apples.push_back(Apple());
                    apples[i].position = rand() % (windowSize[0] / cellSize * windowSize[1] / cellSize);
                    apples[i].color = sf::Color(settings.appleColor[0] * 255, settings.appleColor[1] * 255, settings.appleColor[2] * 255);
                }
            }
            if (settings.scale != previousScale || settings.applesNumber != previousApplesNumber || settings.autoMode != previousAutoMode)
            {
                previousScale = settings.scale;
                previousApplesNumber = settings.applesNumber;
                previousAutoMode = settings.autoMode;
                Restart();
            }
            if (previousVsync != settings.vsync)
            {
                previousVsync = settings.vsync;
                window.setVerticalSyncEnabled(settings.vsync);
            }
            snake.color = sf::Color(settings.snakeColor[0] * 255, settings.snakeColor[1] * 255, settings.snakeColor[2] * 255);
            for (int i = 0; i < apples.size(); i++)
            {
                apples[i].color = sf::Color(settings.appleColor[0] * 255, settings.appleColor[1] * 255, settings.appleColor[2] * 255);
            }
        }

        // Print the background if it's china mode
        if (mode == "china")
        {
            sf::Sprite backgroundSprite(backgroundTexture);
            backgroundSprite.setScale({0.84, 0.84});
            backgroundSprite.setPosition({0, menuOffset * 1.f});
            window.draw(backgroundSprite);
        }

        // Draw the snake
        for (int i = 0; i < snake.body.size(); i++)
        {
            sf::RectangleShape bodyPiece({cellSize * 1.f, cellSize * 1.f});
            bodyPiece.setFillColor(snake.color);
            bodyPiece.setPosition({(snake.body[i] % horizontalCellsNumber) * cellSize * 1.f, 
                (snake.body[i] / horizontalCellsNumber) * cellSize * 1.f + menuOffset});
            window.draw(bodyPiece);
        }

        // Draw the apples
        for (int i = 0; i < apples.size(); i++)
        {
            sf::RectangleShape apple({cellSize * 1.f, cellSize * 1.f});
            apple.setFillColor(apples[i].color);
            apple.setPosition({(apples[i].position % horizontalCellsNumber) * cellSize * 1.f, 
                (apples[i].position / horizontalCellsNumber) * cellSize * 1.f + menuOffset});
            window.draw(apple);
        }

        // Print game over is necessary
        if (isGameOver && mode == "china") bgMusic.stop();
        if (isGameOver && mode == "normal" && snake.body.size() < horizontalCellsNumber * windowSize[1] / cellSize)
        {
            bgMusic.stop();
            DrawText("Game Over!", 0.135f, .05f, .3f, sf::Color(255, 0, 0));
        }
        // Print the highscore text is necessary
        if (isHighscore && isGameOver && snake.body.size() < horizontalCellsNumber * windowSize[1] / cellSize)
        {
            DrawText("Highscore!", 0.05f, .35f, .6f, sf::Color(255, 255, 0));
            DrawText("Last best score: " + std::to_string(scores[scores.size()-2]), .03f, .325f, .7f, sf::Color(255, 255, 0));
        }

        // Print score text
        DrawText((mode == "normal" ? "Score: " : "Social  credit: ") + std::to_string(snake.body.size()), 
            .03f, 0.f, 0.f, sf::Color(255, 255, 255));

        // Print You Won! text
        if (snake.body.size() >= horizontalCellsNumber * windowSize[1] / cellSize)
        {
            DrawText("You won!", .135f, .15f, .3f, sf::Color(255, 255, 0));
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    settings.Save("settings.txt");
    ImGui::SFML::Shutdown();
    return 0;
}
