#include "Settings.hpp"
#include "Game.hpp"

bool funMode = false;
int funModeLevel = 5;
float speed = 2;
float maxSpeed = 20;
int scale = 32;
Color appleColor = Color{255, 0, 0, 255};
int applesNumber = 1;
bool noSpeedLimit = false;
bool autoMode = false;
bool vsync = true;
float audioVolume = 0.5f;

std::vector<std::string> Split(std::string input, char delimiter)
{
    std::vector<std::string> output;
    output.push_back("");
    int index = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == delimiter)
        {
            index++;
            output.push_back("");
            continue;
        }
        output[index] += input[i];
    }
    return output;
}

void Save(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::out);
    settingsFile << "fun-mode=" << (funMode ? "true" : "false") << "\n";
    settingsFile << "fun-mode-level=" << funModeLevel << "\n";
    settingsFile << "speed=" << snake.speed << '\n';
    settingsFile << "max-speed=" << snake.maxSpeed << '\n';
    settingsFile << "scale=" << scale << '\n';
    settingsFile << "snake-color=" << (int)snake.color.r << ',' << (int)snake.color.g  << ',' << (int)snake.color.b << '\n';
    settingsFile << "apple-color=" << (int)appleColor.r << ',' << (int)appleColor.g << ',' << (int)appleColor.b << '\n';
    settingsFile << "apples-number=" << applesNumber << '\n';
    settingsFile << "no-speed-limit=" << (noSpeedLimit ? "true" : "false") << "\n";
    settingsFile << "auto-mode=" << (autoMode ? "true" : "false") << "\n";
    settingsFile << "vsync=" << (vsync ? "true" : "false") << "\n";
    settingsFile << "audio-volume=" << audioVolume << '\n';
    settingsFile.close();

    // Save score
    std::fstream scoreListFile;
    scoreListFile.open("score.txt", std::ios::out);
    for (int k = 0; k < scores.size(); k++)
    {
        scoreListFile << scores[k];
        if (k != scores.size()-1) scoreListFile << "\n";
    }
    scoreListFile.close();
}

void Load(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::in);
    std::vector<std::string> settingsList;
    std::string buf;
    while (std::getline(settingsFile, buf))
        settingsList.push_back(buf);
    settingsFile.close();

    // Process the file
    funMode = settingsList[0] == "fun-mode=true";
    funModeLevel = stoi(settingsList[1].substr(15));
    snake.speed = stof(settingsList[2].substr(6));
    snake.maxSpeed = stof(settingsList[3].substr(10));
    scale = stoi(settingsList[4].substr(6));
    std::vector<std::string> colorBuffer = Split(settingsList[5].substr(12), ',');
    snake.color.r = stoi(colorBuffer[0]);
    snake.color.g = stof(colorBuffer[1]);
    snake.color.b = stof(colorBuffer[2]);
    snake.color.a = 255;
    colorBuffer = Split(settingsList[6].substr(12), ',');
    appleColor.r = stoi(colorBuffer[0]);
    appleColor.g = stoi(colorBuffer[1]);
    appleColor.b = stoi(colorBuffer[2]);
    appleColor.a = 255;
    applesNumber = stoi(settingsList[7].substr(14));
    noSpeedLimit = settingsList[8] == "no-speed-limit=true";
    autoMode = settingsList[9] == "auto-mode=true";
    vsync = settingsList[10] == "vsync=true";
    audioVolume = stof(settingsList[11].substr(13));

    // Load scores
    std::fstream scoreListFile;
    scoreListFile.open("score.txt", std::ios::in);
    while (std::getline(scoreListFile, buf))
        scores.push_back(stoi(buf));
    scoreListFile.close();
}
