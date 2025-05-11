#include "UI.hpp"
#include "Settings.hpp"
#include "Game.hpp"

sf::Font font("assets/JetBrainsMonoNerdFont-Medium.ttf");

int menuOffset = 20;

bool isPaused = false;
bool isSettings = false;


void ShowSettings(bool* isOpen)
{
    if (!ImGui::Begin("Settings", isOpen))
    {
        ImGui::End();
        return;
    }
    ImGui::Checkbox("fun-mode", &settings.funMode);
    ImGui::SliderInt("fun-mode-level", &settings.funModeLevel, 0, 100);
    ImGui::SliderFloat("speed", &settings.speed, 0.1f, 100);
    ImGui::SliderFloat("max-speed", &settings.maxSpeed, 1, 100);
    ImGui::SliderInt("scale", &settings.scale, 5, 100);
    ImGui::ColorEdit3("snake-color", settings.snakeColor);
    ImGui::ColorEdit3("apple-color", settings.appleColor);
    ImGui::SliderInt("apples-number", &settings.applesNumber, 1, 50);
    ImGui::Checkbox("no-speed-limit", &settings.noSpeedLimit);
    ImGui::Checkbox("auto-mode", &settings.autoMode);
    ImGui::Checkbox("vsync", &settings.vsync);
    ImGui::End();
}

void ShowMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (isPaused) if (ImGui::MenuItem("Unpause")) isPaused = false;
            if (!isPaused) if (ImGui::MenuItem("Pause")) isPaused = true;
            if (ImGui::MenuItem("Restart")) Restart();
            if (ImGui::MenuItem("Settings"))
            {
                isSettings = true;
                ShowSettings(&isSettings);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    return;
}

void DrawText(std::string textString, float size, float positionX, float positionY, sf::Color textColor, sf::Font textFont)
{
    sf::Text text(textFont);
    text.setCharacterSize(size * sqrt(pow(windowSize[0], 2) + pow(windowSize[1], 2)));
    text.setPosition({positionX * windowSize[0], positionY * windowSize[1] + menuOffset});
    text.setFillColor(textColor);
    text.setString(textString);
    window.draw(text);
}

