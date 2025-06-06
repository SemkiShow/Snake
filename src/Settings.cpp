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
    std::string buf, label, value;
    while (std::getline(settingsFile, buf))
    {
        label = Split(buf, '=')[0];
        value = Split(buf, '=')[1];

        if (label == "fun-mode") funMode = value == "true";
        if (label == "fun-mode-level") funModeLevel = stoi(value);
        if (label == "speed") snake.speed = stof(settingsList[2].substr(6));
        if (label == "max-speed") snake.maxSpeed = stof(settingsList[3].substr(10));
        if (label == "scale") scale = stoi(settingsList[4].substr(6));
        if (label == "snake-color")
        {
            std::vector<std::string> colorBuffer = Split(value, ',');
            snake.color.r = stoi(colorBuffer[0]);
            snake.color.g = stof(colorBuffer[1]);
            snake.color.b = stof(colorBuffer[2]);
            snake.color.a = 255;
        }
        if (label == "apple-color")
        {
            std::vector<std::string> colorBuffer = Split(value, ',');
            appleColor.r = stoi(colorBuffer[0]);
            appleColor.g = stoi(colorBuffer[1]);
            appleColor.b = stoi(colorBuffer[2]);
            appleColor.a = 255;
        }
        if (label == "apples-number") applesNumber = stoi(value);
        if (label == "no-speed-limit") noSpeedLimit = value == "true";
        if (label == "auto-mode") autoMode = value == "true";
        if (label == "vsync") vsync = value == "true";
        if (label == "audio-volume") audioVolume = stof(value);
    }
    settingsFile.close();

    // Load scores
    std::fstream scoreListFile;
    scoreListFile.open("score.txt", std::ios::in);
    while (std::getline(scoreListFile, buf))
        scores.push_back(stoi(buf));
    scoreListFile.close();
}
