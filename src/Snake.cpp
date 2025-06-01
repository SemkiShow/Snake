#include "Game.hpp"
#include "Settings.hpp"

Snake snake;

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
                apples.erase(apples.begin() + i);
                GenerateApple();
                if (body.size() >= windowSize[0] / scale * windowSize[1] / scale)
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
                PlaySound(pickupSound);
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
        if (body[i] < 0 && direction == 'U')
            body[i] += horizontalCellsNumber * windowSize[1] / scale;
        else if (body[i] >= horizontalCellsNumber * windowSize[1] / scale && direction == 'D')
            body[i] -= horizontalCellsNumber * windowSize[1] / scale;
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
                if (body.size() + apples.size() < horizontalCellsNumber * windowSize[1] / scale)
                    PlaySound(gameOverSound);
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
            }
            if (isGameOver) break;
        }
        if (isGameOver) break;
    }
}
