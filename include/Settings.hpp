#include "raylib.h"
#include <vector>
#include <string>
#include <fstream>

extern bool funMode;
extern int funModeLevel;
extern float speed;
extern float maxSpeed;
extern int scale;
extern Color appleColor;
extern int applesNumber;
extern bool noSpeedLimit;
extern bool autoMode;
extern bool vsync;

void Save(std::string fileName);
void Load(std::string fileName);
