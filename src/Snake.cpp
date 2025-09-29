// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#include "Game.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <fstream>

Snake snake;

void Snake::CheckAppleCollision()
{
    bool hasCollided = false;
    for (size_t i = 0; i < apples.size(); i++)
    {
        for (size_t j = 0; j < body.size(); j++)
        {
            if (body[j] == apples[i].position)
            {
                body.insert(body.begin(), lastTailPosition);
                apples.erase(apples.begin() + i);
                GenerateApple();
                if (body.size() >= windowSize.x / scale * windowSize.y / scale)
                {
                    scores.push_back(body.size());
                    std::sort(scores.begin(), scores.end());
                    std::fstream scoreListFile;
                    scoreListFile.open("score.txt", std::ios::out);
                    for (size_t k = 0; k < scores.size(); k++)
                    {
                        scoreListFile << scores[k];
                        if (k < scores.size() - 1) scoreListFile << "\n";
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
    for (size_t i = body.size() - 1; i < body.size(); i++)
    {
        if (body[i] < 0 && direction == 'U')
            body[i] += horizontalCellsNumber * windowSize.y / scale;
        else if (body[i] >= horizontalCellsNumber * windowSize.y / scale && direction == 'D')
            body[i] -= horizontalCellsNumber * windowSize.y / scale;
        else if ((body[i] + 1) % horizontalCellsNumber == 0 && direction == 'L')
            body[i] += horizontalCellsNumber;
        else if (body[i] % horizontalCellsNumber == 0 && direction == 'R')
            body[i] -= horizontalCellsNumber;
    }
}

void Snake::CheckSelfCollision()
{
    for (size_t i = 0; i < body.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            if (body[i] == body[j] && i != j)
            {
                if (body.size() + apples.size() < horizontalCellsNumber * windowSize.y / scale)
                    PlaySound(gameOverSound);
                scores.push_back(body.size());
                std::sort(scores.begin(), scores.end());
                if (body.size() >= scores[scores.size() - 1]) isHighscore = true;
                isGameOver = true;
            }
            if (isGameOver) break;
        }
        if (isGameOver) break;
    }
}
