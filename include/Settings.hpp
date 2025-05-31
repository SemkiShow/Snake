#include "raylib.h"
#include <vector>
#include <string>
#include <fstream>

extern bool funMode;
extern int funModeLevel;
extern float funModeLevelFloat;
extern float speed;
extern float maxSpeed;
extern int scale;
extern float scaleFloat;
extern Color appleColor;
extern int applesNumber;
extern float applesNumberFloat;
extern bool noSpeedLimit;
extern bool autoMode;
extern bool vsync;
extern float audioVolume;

void Save(std::string fileName);
void Load(std::string fileName);
