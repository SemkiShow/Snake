#include "Settings.hpp"

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

void Settings::Save(std::string fileName)
{
    // Read the file
    std::fstream settingsFile;
    settingsFile.open(fileName, std::ios::out);
    settingsFile << "fun-mode=" << (funMode ? "true" : "false") << "\n";
    settingsFile << "fun-mode-level=" << funModeLevel << "\n";
    settingsFile << "speed=" << speed << '\n';
    settingsFile << "max-speed=" << maxSpeed << '\n';
    settingsFile << "scale=" << scale << '\n';
    settingsFile << "snake-color=" << snakeColor[0] << ',' << snakeColor[1] << ',' << snakeColor[2] << '\n';
    settingsFile << "apple-color=" << appleColor[0] << ',' << appleColor[1] << ',' << appleColor[2] << '\n';
    settingsFile << "apples-number=" << applesNumber << '\n';
    settingsFile << "no-speed-limit=" << (noSpeedLimit ? "true" : "false") << "\n";
    settingsFile << "auto-mode=" << (autoMode ? "true" : "false") << "\n";
    settingsFile << "vsync=" << (vsync ? "true" : "false") << "\n";
    settingsFile.close();
}

void Settings::Load(std::string fileName)
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
    funMode = (settingsList[0] == "fun-mode=true") ? true : false;
    funModeLevel = stoi(settingsList[1].substr(15));
    speed = stof(settingsList[2].substr(6));
    maxSpeed = stof(settingsList[3].substr(10));
    scale = stoi(settingsList[4].substr(6));
    std::vector<std::string> colorBuffer = Split(settingsList[5].substr(12), ',');
    for (int i = 0; i < 3; i++)
        snakeColor[i] = stof(colorBuffer[i]);
    colorBuffer = Split(settingsList[6].substr(12), ',');
    for (int i = 0; i < 3; i++)
        appleColor[i] = stof(colorBuffer[i]);
    applesNumber = stoi(settingsList[7].substr(14));
    noSpeedLimit = (settingsList[8] == "no-speed-limit=true") ? true : false;
    autoMode = (settingsList[9] == "auto-mode=true") ? true : false;
    vsync = (settingsList[10] == "vsync=true") ? true : false;
}
