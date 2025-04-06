#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <algorithm>

unsigned int horizontalCellsNumber = 32;
int menuOffset = 20;
unsigned int windowWidth = 16*50*2;
unsigned int windowHeight = 9*50*2;
unsigned int windowSize[2] = {windowWidth - menuOffset, windowHeight + menuOffset};
unsigned int cellSize = windowWidth / horizontalCellsNumber;
bool verticalSync = true;

bool isGameOver = false;
bool isHighscore = false;
bool isPaused = false;
std::string mode = "normal";
std::string buf;

sf::Music bgMusic;
sf::Music pauseMusic;
char lastKeyPress = 'R';
bool keyBuffer[4];

bool isSettings = false;

class Snake
{
    public:
        std::vector<int> body;
        char direction = 'R';
        sf::Color color = sf::Color(100, 250, 50);
        std::vector<char> keyBuffer;
        double speed = 2;
        double maxSpeed = 20;
        std::vector<char> directionHistory;
        int lastTailPosition = 0;
}snake;
    
class Apple
{
    public:
        unsigned int position;
        sf::Color color = sf::Color(255, 0, 0);
};
std::vector<Apple> apples;
    
class Settings
{
    public:
        void Save(std::string fileName);
        void Load(std::string fileName);
        bool funMode = false;
        int funModeLevel = 5;
        float speed = 2;
        float maxSpeed = 20;
        int scale = 32;
        float snakeColor[3];
        float appleColor[3];
        int applesNumber = 1;
        bool noSpeedLimit = false;
        bool autoMode = false;
}settings;

std::string* Split(std::string input, char delimiter = ' ')
{
    std::vector<std::string> output;
    output.push_back("");
    int index = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == delimiter)
        {
            index++;
            output.push_back("");
            continue;
        }
        output[index] += input[i];
    }
    std::string* outputArray = new std::string[output.size()];
    std::copy(output.begin(), output.end(), outputArray);
    return outputArray;
}

void Settings::Save(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::out);
    settingsFile << "fun-mode=" << (funMode ? "true" : "false") << "\n";
    settingsFile << "fun-mode-level=" << funModeLevel << "\n";
    settingsFile << "speed=" << speed << '\n';
    settingsFile << "max-speed=" << maxSpeed << '\n';
    settingsFile << "scale=" << scale << '\n';
    settingsFile << "snake-color=" << snakeColor[0] << ',' << snakeColor[1] << ',' << snakeColor[2] << '\n';
    settingsFile << "apple-color=" << appleColor[0] << ',' << appleColor[1] << ',' << appleColor[2] << '\n';
    settingsFile << "apples-number=" << applesNumber << '\n';
    settingsFile << "no-speed-limit=" << (noSpeedLimit ? "true" : "false") << "\n";
    settingsFile << "auto-mode=" << (autoMode ? "true" : "false") << "\n";
    settingsFile.close();
}

void Settings::Load(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::in);
    std::vector<std::string> settingsList;
    while (std::getline(settingsFile, buf))
        settingsList.push_back(buf);
    settingsFile.close();

    // Process the file
    funMode = (settingsList[0] == "fun-mode=true") ? true : false;
    funModeLevel = stoi(settingsList[1].substr(15));
    speed = stof(settingsList[2].substr(6));
    maxSpeed = stof(settingsList[3].substr(10));
    scale = stoi(settingsList[4].substr(6));
    std::string* colorBuffer = Split(settingsList[5].substr(12), ',');
    for (int i = 0; i < 3; i++)
        snakeColor[i] = stof(colorBuffer[i]);
    colorBuffer = Split(settingsList[6].substr(12), ',');
    for (int i = 0; i < 3; i++)
        appleColor[i] = stof(colorBuffer[i]);
    delete[] colorBuffer;
    applesNumber = stoi(settingsList[7].substr(14));
    noSpeedLimit = (settingsList[8] == "no-speed-limit=true") ? true : false;
    autoMode = (settingsList[9] == "auto-mode=true") ? true : false;    
}

void ShowSettings(bool* isOpen)
{
    if (!ImGui::Begin("Settings", isOpen))
    {
        ImGui::End();
        return;
    }
    ImGui::Checkbox("fun-mode", &settings.funMode);
    ImGui::SliderInt("fun-mode-level", &settings.funModeLevel, 0, 100);
    ImGui::SliderFloat("speed", &settings.speed, 0.1f, 100);
    ImGui::SliderFloat("max-speed", &settings.maxSpeed, 1, 100);
    ImGui::SliderInt("scale", &settings.scale, 5, 100);
    ImGui::ColorEdit3("snake-color", settings.snakeColor);
    ImGui::ColorEdit3("apple-color", settings.appleColor);
    ImGui::SliderInt("apples-number", &settings.applesNumber, 1, 50);
    ImGui::Checkbox("no-speed-limit", &settings.noSpeedLimit);
    ImGui::Checkbox("auto-mode", &settings.autoMode);
    ImGui::End();
}

void Restart()
{
    snake.body.clear();
    snake.body.push_back(0);
    snake.body.push_back(1);
    snake.body.push_back(2);
    // snake.body.push_back(3);
    // snake.body.push_back(4);
    // snake.body.push_back(5);
    // snake.body.push_back(6);
    snake.color = sf::Color(settings.snakeColor[0] * 255, settings.snakeColor[1] * 255, settings.snakeColor[2] * 255);
    snake.direction = 'R';
    snake.keyBuffer.clear();
    snake.lastTailPosition = 0;
    lastKeyPress = 'R';
    keyBuffer[0] = false;
    keyBuffer[1] = false;
    keyBuffer[2] = false;
    keyBuffer[3] = true;
    apples.clear();
    for (int i = 0; i < settings.applesNumber; i++)
    {
        apples.push_back(Apple());
        apples[i].position = rand() % (horizontalCellsNumber * windowHeight / cellSize);
        apples[i].color = sf::Color(settings.appleColor[0] * 255, settings.appleColor[1] * 255, settings.appleColor[2] * 255);
    }
    isGameOver = false;
    isPaused = false;
    pauseMusic.stop();
    bgMusic.play();
}

void ShowMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (isPaused) if (ImGui::MenuItem("Unpause")) isPaused = false;
            if (!isPaused) if (ImGui::MenuItem("Pause")) isPaused = true;
            if (ImGui::MenuItem("Restart")) Restart();
            if (ImGui::MenuItem("Settings"))
            {
                isSettings = true;
                ShowSettings(&isSettings);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    return;
}

void ProcessPlayerInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && lastKeyPress != 'D')
    {
        if (keyBuffer[0] == false)
        {
            keyBuffer[0] = true;
            lastKeyPress = 'U';
            snake.keyBuffer.push_back('U');
        }
    }
    else keyBuffer[0] = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && lastKeyPress != 'U')
    {
        if (keyBuffer[1] == false)
        {
            keyBuffer[1] = true;
            lastKeyPress = 'D';
            snake.keyBuffer.push_back('D');
        }
    }
    else keyBuffer[1] = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && lastKeyPress != 'R')
    {
        if (keyBuffer[2] == false)
        {
            keyBuffer[2] = true;
            lastKeyPress = 'L';
            snake.keyBuffer.push_back('L');
        }
    }
    else keyBuffer[2] = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && lastKeyPress != 'L')
    {
        if (keyBuffer[3] == false)
        {
            keyBuffer[3] = true;
            lastKeyPress = 'R';
            snake.keyBuffer.push_back('R');
        }
    }
    else keyBuffer[3] = false;
}

std::string GenerateAutoModeKeypresses()
{
    std::string output = "";
    for (int i = 0; i < horizontalCellsNumber - 3; i++)
        output += 'R';
    for (int i = 0; i < windowHeight / cellSize - 1; i++)
        output += 'D';
    for (int i = 0; i < horizontalCellsNumber / 2 - 1; i++)
    {
        output += 'L';
        for (int j = 0; j < windowHeight / cellSize - 2; j++)
            output += 'U';
        output += 'L';
        for (int j = 0; j < windowHeight / cellSize - 2; j++)
            output += 'D';
    }
    output += 'L';
    for (int i = 0; i < windowHeight / cellSize - 1; i++)
        output += 'U';
    for (int i = 0; i < 2; i++)
        output += 'R';
    return output;
}

int main()
{
    // Non-SFML init
    Restart();
    srand(time(0));
    settings.Load("settings.txt");
    std::fstream versionFile;
    versionFile.open("version.txt", std::ios::in);
    std::vector<std::string> versionFileContents;
    while (std::getline(versionFile, buf))
        versionFileContents.push_back(buf);
    versionFile.close();
    // SFML init
    sf::RenderWindow window(sf::VideoMode({windowSize[0], windowSize[1]}), "Snake " + versionFileContents[0]);
    (void) bgMusic.openFromFile("assets/snake-bg.mp3");
    bgMusic.setLooping(true);
    bgMusic.setVolume(50.f);
    bgMusic.play();
    (void) pauseMusic.openFromFile("assets/pause-music.mp3");
    pauseMusic.setLooping(true);
    pauseMusic.setVolume(50.f);
    sf::SoundBuffer pickupBuffer("assets/pickup.wav");
    sf::Sound pickupSound(pickupBuffer);
    sf::SoundBuffer gameOverBuffer("assets/game-over.wav");
    sf::Sound gameOverSound(gameOverBuffer);
    sf::Font font("assets/JetBrainsMonoNerdFont-Medium.ttf");
    sf::Texture backgroundTexture;
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
    
    // ImGUI init
    (void) ImGui::SFML::Init(window);
    
    sf::Clock deltaTimeClock;
    sf::Time deltaTime;
    sf::Clock delayClock;
    int previousScale = horizontalCellsNumber;
    int previousApplesNumber = settings.applesNumber;
    bool previousAutoMode = settings.autoMode;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
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

            // Add keypresses to a keybuffer if it's empty and autoModee is true
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
            bool hasCollided = false;
            for (int i = 0; i < apples.size(); i++)
            {
                for (int j = 0; j < snake.body.size(); j++)
                {
                    if (snake.body[j] == apples[i].position)
                    {
                        snake.body.insert(snake.body.begin(), snake.lastTailPosition);
                        int newApplePosition = 0;
                        bool loop = snake.body.size() + apples.size() < horizontalCellsNumber * windowHeight / cellSize;
                        // if (!loop) youWonSound.play();
                        while (loop)
                        {
                            loop = false;
                            newApplePosition = rand() % (horizontalCellsNumber * windowHeight / cellSize);
                            for (int k = 0; k < snake.body.size(); k++)
                            {
                                if (snake.body[k] == newApplePosition)
                                {
                                    loop = true;
                                    break;
                                }
                            }
                            for (int k = 0; k < apples.size(); k++)
                            {
                                if (apples[k].position == newApplePosition)
                                {
                                    loop = true;
                                    break;
                                }
                            }
                        }
                        apples[i].position = newApplePosition;
                        pickupSound.play();
                        hasCollided = true;
                        break;
                    }
                }
                if (hasCollided) break;
            }

            // Check collision with the walls
            for (int i = snake.body.size()-1; i < snake.body.size(); i++)
            {
                if (snake.body[i] < 0)
                    snake.body[i] += horizontalCellsNumber * (windowHeight / cellSize);
                else if (snake.body[i] >= horizontalCellsNumber * (windowHeight / cellSize))
                    snake.body[i] -= horizontalCellsNumber * (windowHeight / cellSize);
                else if ((snake.body[i]+1) % horizontalCellsNumber == 0 && snake.direction == 'L')
                    snake.body[i] += horizontalCellsNumber;
                else if (snake.body[i] % horizontalCellsNumber == 0 && snake.direction == 'R')
                    snake.body[i] -= horizontalCellsNumber;
            }

            // Check self-collision
            for (int i = 0; i < snake.body.size(); i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (snake.body[i] == snake.body[j] && i != j)
                    {
                        if (snake.body.size() + apples.size() < horizontalCellsNumber * windowHeight / cellSize)
                            gameOverSound.play();
                        std::fstream scoreListFile;
                        scoreListFile.open("score.txt", std::ios::in);
                        std::vector<int> scores;
                        while (std::getline(scoreListFile, buf))
                            scores.push_back(stoi(buf));
                        scores.push_back(snake.body.size());
                        std::sort(scores.begin(), scores.end());
                        scoreListFile.close();
                        scoreListFile.open("score.txt", std::ios::out);
                        for (int k = 0; k < scores.size(); k++)
                        {
                            scoreListFile << scores[k];
                            if (k != scores.size()-1) scoreListFile << std::endl;
                        }
                        scoreListFile.close();
                        if (snake.body.size() >= scores[scores.size()-1]) isHighscore = true;
                        isGameOver = true;
                    }
                    if (isGameOver) break;
                }
                if (isGameOver) break;
            }
        
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

            snake.speed = settings.speed * 1.0;
            snake.maxSpeed = settings.maxSpeed * 1.0;
            // horizontalCellsNumber = settings.scale;
            // cellSize = windowWidth / horizontalCellsNumber;
            cellSize = settings.scale;
            horizontalCellsNumber = windowWidth / cellSize;
            if (settings.scale != previousScale || settings.applesNumber != previousApplesNumber || settings.autoMode != previousAutoMode)
            {
                previousScale = settings.scale;
                previousApplesNumber = settings.applesNumber;
                previousAutoMode = settings.autoMode;
                Restart();
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
            if (isGameOver)
                (void) backgroundTexture.loadFromFile("assets/china-bg-gameover.jpg");
            else
                (void) backgroundTexture.loadFromFile("assets/china-bg.jpg");
            backgroundTexture.setSmooth(true);
            sf::Sprite backgroundSprite(backgroundTexture);
            backgroundSprite.setScale({0.84, 0.84});
            backgroundSprite.setPosition({0, menuOffset * 1.f});
            window.draw(backgroundSprite);
        }

        // Drawing the snake
        for (int i = 0; i < snake.body.size(); i++)
        {
            sf::RectangleShape bodyPiece({cellSize * 1.f, cellSize * 1.f});
            bodyPiece.setFillColor(snake.color);
            bodyPiece.setPosition({(snake.body[i] % horizontalCellsNumber) * cellSize * 1.f, 
                (snake.body[i] / horizontalCellsNumber) * cellSize * 1.f + menuOffset});
            window.draw(bodyPiece);
        }

        // Drawing the apples
        for (int i = 0; i < apples.size(); i++)
        {
            sf::RectangleShape apple({cellSize * 1.f, cellSize * 1.f});
            if (snake.body.size() + apples.size() < horizontalCellsNumber * windowHeight / cellSize)
                apple.setFillColor(apples[i].color);
            else
                apple.setFillColor(snake.color);
            apple.setPosition({(apples[i].position % horizontalCellsNumber) * cellSize * 1.f, 
                (apples[i].position / horizontalCellsNumber) * cellSize * 1.f + menuOffset});
            window.draw(apple);
        }

        // Print game over is necessary
        if (isGameOver && mode == "china") bgMusic.stop();
        if (isGameOver && mode == "normal" && snake.body.size() + apples.size() < horizontalCellsNumber * windowHeight / cellSize)
        {
            bgMusic.stop();
            sf::Text gameOverText(font);
            gameOverText.setCharacterSize(0.135f * sqrt(windowWidth * windowWidth + windowHeight * windowHeight));
            gameOverText.setPosition({windowWidth * .05f, windowHeight * .3f});
            gameOverText.setFillColor(sf::Color(255, 0, 0));
            gameOverText.setString("Game Over!");
            window.draw(gameOverText);
        }
        // Print the highscore text is necessary
        if (isHighscore && isGameOver && snake.body.size() + apples.size() < horizontalCellsNumber * windowHeight / cellSize)
        {
            sf::Text highscoreText(font);
            highscoreText.setCharacterSize(0.05f * sqrt(windowWidth * windowWidth + windowHeight * windowHeight));
            highscoreText.setPosition({windowWidth * .35f, windowHeight * .6f});
            highscoreText.setFillColor(sf::Color(255, 255, 0));
            highscoreText.setString("Highscore!");
            window.draw(highscoreText);

            std::fstream scoreListFile;
            scoreListFile.open("score.txt", std::ios::in);
            std::vector<int> scores;
            while (std::getline(scoreListFile, buf))
                scores.push_back(stoi(buf));
            scoreListFile.close();

            sf::Text lastBestScoreText(font);
            lastBestScoreText.setCharacterSize(0.03f * sqrt(windowWidth * windowWidth + windowHeight * windowHeight));
            lastBestScoreText.setPosition({windowWidth * .325f, windowHeight * .7f});
            lastBestScoreText.setFillColor(sf::Color(255, 255, 0));
            lastBestScoreText.setString("Last best score: " + std::to_string(scores[scores.size()-2]));
            window.draw(lastBestScoreText);
        }

        // Print score text
        sf::Text scoreText(font);
        scoreText.setCharacterSize(50);
        scoreText.setPosition({0, menuOffset * 1.f});
        std::string scorePrefix = (mode == "normal") ? "Score: " : "Social  credit: ";
        scoreText.setString(scorePrefix + std::to_string(snake.body.size()));
        window.draw(scoreText);

        // Print You Won! text
        if (snake.body.size() + apples.size() >= horizontalCellsNumber * windowHeight / cellSize)
        {
            bgMusic.stop();
            sf::Text youWonText(font);
            youWonText.setCharacterSize(0.135f * sqrt(windowWidth * windowWidth + windowHeight * windowHeight));
            youWonText.setPosition({windowWidth * .15f, windowHeight * .3f});
            youWonText.setFillColor(sf::Color(255, 255, 0));
            youWonText.setString("You won!");
            window.draw(youWonText);
        }

        // Print debug info
        // std::cout << snake.body.size() << std::endl;

        ImGui::SFML::Render(window);
        window.display();
    }

    settings.Save("settings.txt");
    ImGui::SFML::Shutdown();
}
