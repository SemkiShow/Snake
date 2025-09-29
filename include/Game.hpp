// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#pragma once

#include <raylib.h>
#include <string>
#include <vector>

extern Vector2 windowSize;
extern unsigned int horizontalCellsNumber;

extern char lastKeyPress;

extern Music bgMusic;
extern Music funMusic;
extern Music pauseMusic;
extern Texture2D funTexture;
extern Texture2D funGameOverTexture;
extern Sound pickupSound;
extern Sound gameOverSound;

extern bool isGameOver;
extern bool isHighscore;
extern std::string mode;

extern std::vector<size_t> scores;

class Apple
{
  public:
    size_t position;
    Color color = Color{255, 0, 0, 255};
};
extern std::vector<Apple> apples;

class Snake
{
  public:
    std::vector<size_t> body;
    char direction = 'R';
    Color color = Color{100, 250, 50, 255};
    std::vector<char> keyBuffer;
    float speed = 2;
    float maxSpeed = 20;
    std::vector<char> directionHistory;
    int lastTailPosition = 0;
    void CheckAppleCollision();
    void CheckWallCollision();
    void CheckSelfCollision();
};
extern Snake snake;

void GenerateApple();
void Restart();
void ProcessPlayerInput();
std::string GenerateAutoModeKeypresses();
void DrawFrame();
void CleanUp();
