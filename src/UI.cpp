#define RAYGUI_IMPLEMENTATION
#include "UI.hpp"
#include "Settings.hpp"
#include "Game.hpp"

#define UI_SPACING 30
#define ELEMENT_SIZE 30
#define SLIDER_WIDTH (float)GetScreenWidth() - 270
#define COLOR_PICKER_SIZE 100
#define ELEMENT_SPACING 10

bool isPaused = false;
bool isSettings = false;

int nextElementPositionY = UI_SPACING * 2;

void DrawCheckBox(char* text, bool* value)
{
    GuiCheckBox(Rectangle{UI_SPACING * 2, nextElementPositionY, ELEMENT_SIZE, ELEMENT_SIZE}, text, value);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawSlider(char* leftText, char* rightText, float* value, float minValue, float maxValue)
{
    GuiSlider(Rectangle{UI_SPACING * 2, nextElementPositionY, SLIDER_WIDTH, ELEMENT_SIZE}, leftText, rightText, value, minValue, maxValue);
    DrawText(std::to_string(*value).c_str(), (SLIDER_WIDTH + UI_SPACING*2) / 2.f, nextElementPositionY + 5, 24, WHITE);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawSliderInt(char* leftText, char* rightText, int* value, float minValue, float maxValue)
{
    float valueFloat = *value;
    GuiSlider(Rectangle{UI_SPACING * 2, nextElementPositionY, SLIDER_WIDTH, ELEMENT_SIZE}, leftText, rightText, &valueFloat, minValue, maxValue);
    *value = valueFloat;
    DrawText(std::to_string(*value).c_str(), (SLIDER_WIDTH + UI_SPACING*2) / 2.f, nextElementPositionY + 5, 24, WHITE);
    nextElementPositionY += ELEMENT_SIZE + ELEMENT_SPACING;
}

void DrawColorPicker(char* text, Color* color)
{
    GuiColorPanel(Rectangle{UI_SPACING * 2, nextElementPositionY, COLOR_PICKER_SIZE, COLOR_PICKER_SIZE}, text, color);
    nextElementPositionY += COLOR_PICKER_SIZE + ELEMENT_SPACING;
}

void DrawSettings(bool* isOpen)
{
    if (!*isOpen) return;
    DrawRectangleRounded(
        Rectangle{UI_SPACING, UI_SPACING, (float)GetScreenWidth() - UI_SPACING*2, (float)GetScreenHeight() - UI_SPACING*2}, 
        0.1f, 1, Color{128, 128, 128, 128});
    nextElementPositionY = UI_SPACING * 2;
    DrawCheckBox("fun-mode", &funMode);
    DrawSliderInt("", "fun-mode-level", &funModeLevel, 0, 100);
    DrawSlider("", "speed", &snake.speed, 0.1f, 100);
    DrawSlider("", "max-speed", &snake.maxSpeed, 1, 100);
    DrawSliderInt("", "scale", &scale, 5, 100);
    DrawColorPicker("snake-color", &snake.color);
    DrawColorPicker("apple-color", &appleColor);
    DrawSliderInt("", "apples-number", &applesNumber, 1, 50);
    DrawCheckBox("no-speed-limit", &noSpeedLimit);
    DrawCheckBox("auto-mode", &autoMode);
    DrawCheckBox("vsync", &vsync);
    DrawSlider("", "audio-volume", &audioVolume, 0, 2);
}

void DrawUI()
{

    if (isGameOver && mode == "china") StopMusicStream(funMusic); 
    if (isGameOver && mode == "normal" && snake.body.size() < horizontalCellsNumber * windowSize[1] / scale)
    {
        StopMusicStream(bgMusic);
        DrawText("Game Over!", .13f * GetScreenWidth(), .3f * GetScreenHeight(), 0.135f * GetScreenWidth(), RED);
    }
    // Print the highscore text is necessary
    if (isHighscore && isGameOver && snake.body.size() < horizontalCellsNumber * windowSize[1] / scale)
    {
        DrawText("Highscore!", .35f * GetScreenWidth(), .6f * GetScreenHeight(), 0.05f * GetScreenWidth(), YELLOW);
        DrawText(("Last best score: " + std::to_string(scores[scores.size()-2])).c_str(), 
            .345f * GetScreenWidth(), .7f * GetScreenHeight(), .03f * GetScreenWidth(), YELLOW);
    }

    // Print score text
    DrawText(((mode == "normal" ? "Score: " : "Social  credit: ") + std::to_string(snake.body.size())).c_str(), 
        0, 0, .03f * GetScreenWidth(), WHITE);

    // Print You Won! text
    if (snake.body.size() >= horizontalCellsNumber * windowSize[1] / scale)
        DrawText("You won!", .25f * GetScreenWidth(), .3f * GetScreenHeight(), .135f * GetScreenWidth(), YELLOW);

    // Draw buttons
    if (GuiButton(Rectangle{(float)GetScreenWidth() - 30, 0, 30, 30}, "#142#")) isSettings = !isSettings;
    if (GuiButton(Rectangle{(float)GetScreenWidth() - 60, 0, 30, 30}, isPaused ? "#131#" : "#132#")) isPaused = !isPaused;
    if (GuiButton(Rectangle{(float)GetScreenWidth() - 90, 0, 30, 30}, "#77#")) Restart();
    
    DrawSettings(&isSettings);
}
