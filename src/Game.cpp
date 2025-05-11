#include "Game.hpp"
#include "UI.hpp"
#include "Settings.hpp"

unsigned int windowSize[2]{16*50*2, 9*50*2};
unsigned int cellSize = 32;
unsigned int horizontalCellsNumber = 50;

char lastKeyPress = 'R';
bool keyBuffer[4];

sf::RenderWindow window(sf::VideoMode({windowSize[0], windowSize[1]+menuOffset}), "Snake");
sf::Music bgMusic("assets/snake-bg.mp3");
sf::Music pauseMusic("assets/pause-music.mp3");
sf::Texture backgroundTexture;
sf::SoundBuffer pickupBuffer("assets/pickup.wav");
sf::Sound pickupSound(pickupBuffer);
sf::SoundBuffer gameOverBuffer("assets/game-over.wav");
sf::Sound gameOverSound(gameOverBuffer);

bool isGameOver = false;
bool isHighscore = false;
std::string mode = "normal";

std::vector<int> scores;

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
        apples[i].position = rand() % (windowSize[0] / cellSize * windowSize[1] / cellSize);
        apples[i].color = sf::Color(settings.appleColor[0] * 255, settings.appleColor[1] * 255, settings.appleColor[2] * 255);
    }
    
    isGameOver = false;
    isPaused = false;
    (void) backgroundTexture.loadFromFile("assets/china-bg.jpg");

    scores.clear();
    std::fstream scoreListFile;
    scoreListFile.open("score.txt", std::ios::in);
    std::string buf;
    while (std::getline(scoreListFile, buf))
        scores.push_back(stoi(buf));
    scoreListFile.close();

    pauseMusic.stop();
    bgMusic.play();
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
    for (int i = 0; i < windowSize[1] / cellSize - 1; i++)
        output += 'D';
    for (int i = 0; i < horizontalCellsNumber / 2 - 1; i++)
    {
        output += 'L';
        for (int j = 0; j < windowSize[1] / cellSize - 2; j++)
            output += 'U';
        output += 'L';
        for (int j = 0; j < windowSize[1] / cellSize - 2; j++)
            output += 'D';
    }
    output += 'L';
    for (int i = 0; i < windowSize[1] / cellSize - 1; i++)
        output += 'U';
    for (int i = 0; i < 2; i++)
        output += 'R';
    return output;
}

void Snake::CheckAppleCollision()
{
    bool hasCollided = false;
    for (int i = 0; i < apples.size(); i++)
    {
        for (int j = 0; j < body.size(); j++)
        {
            if (body[j] == apples[i].position)
            {
                body.insert(body.begin(), lastTailPosition);
                int newApplePosition = 0;
                bool loop = body.size() + apples.size() - 1 < windowSize[0] / cellSize * windowSize[1] / cellSize;
                if (loop)
                {
                    while (loop)
                    {
                        loop = false;
                        newApplePosition = rand() % (windowSize[0] / cellSize * windowSize[1] / cellSize);
                        for (int k = 0; k < body.size(); k++)
                        {
                            if (body[k] == newApplePosition)
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
                }
                else
                {
                    apples.erase(apples.begin() + i);
                }
                if (body.size() >= windowSize[0] / cellSize * windowSize[1] / cellSize)
                {
                    scores.push_back(body.size());
                    std::sort(scores.begin(), scores.end());
                    std::fstream scoreListFile;
                    scoreListFile.open("score.txt", std::ios::out);
                    for (int k = 0; k < scores.size(); k++)
                    {
                        scoreListFile << scores[k];
                        if (k != scores.size()-1) scoreListFile << "\n";
                    }
                    scoreListFile.close();
                }
                pickupSound.play();
                hasCollided = true;
                break;
            }
        }
        if (hasCollided) break;
    }
}


void Snake::CheckWallCollision()
{
    for (int i = body.size()-1; i < body.size(); i++)
    {
        if (body[i] < 0)
            body[i] += horizontalCellsNumber * (windowSize[1] / cellSize);
        else if (body[i] >= horizontalCellsNumber * (windowSize[1] / cellSize))
            body[i] -= horizontalCellsNumber * (windowSize[1] / cellSize);
        else if ((body[i]+1) % horizontalCellsNumber == 0 && direction == 'L')
            body[i] += horizontalCellsNumber;
        else if (body[i] % horizontalCellsNumber == 0 && direction == 'R')
            body[i] -= horizontalCellsNumber;
    }
}

void Snake::CheckSelfCollision()
{
    for (int i = 0; i < body.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (body[i] == body[j] && i != j)
            {
                if (body.size() + apples.size() < horizontalCellsNumber * windowSize[1] / cellSize)
                    gameOverSound.play();
                scores.push_back(body.size());
                std::sort(scores.begin(), scores.end());
                std::fstream scoreListFile;
                scoreListFile.open("score.txt", std::ios::out);
                for (int k = 0; k < scores.size(); k++)
                {
                    scoreListFile << scores[k];
                    if (k != scores.size()-1) scoreListFile << "\n";
                }
                scoreListFile.close();
                if (body.size() >= scores[scores.size()-1]) isHighscore = true;
                isGameOver = true;
                if (mode == "china") (void) backgroundTexture.loadFromFile("assets/china-bg-gameover.jpg");
            }
            if (isGameOver) break;
        }
        if (isGameOver) break;
    }
}
