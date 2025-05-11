#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <random>

extern unsigned int windowSize[2];
extern unsigned int cellSize;
extern unsigned int horizontalCellsNumber;

extern char lastKeyPress;
extern bool keyBuffer[4];

extern sf::RenderWindow window;
extern sf::Music bgMusic;
extern sf::Music pauseMusic;
extern sf::Texture backgroundTexture;
extern sf::Sound pickupSound;
extern sf::Sound gameOverSound;

extern bool isGameOver;
extern bool isHighscore;
extern std::string mode;

extern std::vector<int> scores;

void Restart();
void ProcessPlayerInput();
std::string GenerateAutoModeKeypresses();

class Apple
{
    public:
        unsigned int position;
        sf::Color color = sf::Color(255, 0, 0);
};
extern std::vector<Apple> apples;

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
        void CheckAppleCollision();
        void CheckWallCollision();
        void CheckSelfCollision();
};
extern Snake snake;
