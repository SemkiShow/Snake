#include "Game.hpp"

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
