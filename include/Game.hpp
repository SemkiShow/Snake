#include "raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

extern unsigned int windowSize[2];
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

extern std::vector<int> scores;

class Apple
{
    public:
        unsigned int position;
        Color color = Color{255, 0, 0, 255};
};
extern std::vector<Apple> apples;

class Snake
{
    public:
    std::vector<int> body;
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

void Restart();
void ProcessPlayerInput();
std::string GenerateAutoModeKeypresses();
void DrawFrame();
void CleanUp();
