#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> Split(std::string input, char delimiter = ' ');

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
        bool vsync = true;
};
extern Settings settings;
    