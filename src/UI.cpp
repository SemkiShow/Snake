#define RAYGUI_IMPLEMENTATION
#include "UI.hpp"
#include "Settings.hpp"
#include "Game.hpp"

bool isPaused = false;
bool isSettings = false;

void DrawSettings(bool* isOpen)
{
    if (!*isOpen) return;
    DrawRectangleRounded(Rectangle{30, 30, (float)GetScreenWidth() - 60, (float)GetScreenHeight() - 60}, 0.1f, 1, Color{128, 128, 128, 128});
    GuiCheckBox(Rectangle{60, 60, 30, 30}, "fun-mode", &funMode);
    // GuiSlider(Rectangle{60, 100, (float)GetScreenWidth() - 250, 30}, "", "fun-mode-level", &funModeLevel, 0, 100);
    GuiSlider(Rectangle{60, 140, (float)GetScreenWidth() - 250, 30}, "", "speed", &snake.speed, 0.1f, 100);
    GuiSlider(Rectangle{60, 180, (float)GetScreenWidth() - 250, 30}, "", "max-speed", &snake.maxSpeed, 1, 100);
    // GuiSliderInt(Rectangle{60, 220, 30, 30}, "", "scale", &scale, 5, 100);
    GuiColorPicker(Rectangle{60, 260, 100, 100}, "snake-color", &snake.color);
    GuiColorPicker(Rectangle{60, 370, 100, 100}, "apple-color", &appleColor);
    // GuiSliderInt(Rectangle{60, 480, 30, 30}, "", "apples-number", &applesNumber, 1, 50);
    GuiCheckBox(Rectangle{60, 520, 30, 30}, "no-speed-limit", &noSpeedLimit);
    GuiCheckBox(Rectangle{60, 560, 30, 30}, "auto-mode", &autoMode);
    GuiCheckBox(Rectangle{60, 600, 30, 30}, "vsync", &vsync);
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
