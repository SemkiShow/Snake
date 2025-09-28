#include "Game.hpp"
#include "Settings.hpp"
#include "UI.hpp"
#include <cmath>

Vector2 windowSize{16 * 50 * 2, 9 * 50 * 2};
unsigned int horizontalCellsNumber = 50;
#define SNAKE_BORDER_SIZE 5

Music bgMusic;
Music funMusic;
Music pauseMusic;
Texture2D funTexture;
Texture2D funGameOverTexture;
Sound pickupSound;
Sound gameOverSound;

bool isGameOver = false;
bool isHighscore = false;
std::string mode = "normal";

std::vector<size_t> scores;
std::vector<Apple> apples;

double delayClock;
char lastKeyPress = 'R';
int lastScale = scale;
int lastApplesNumber = applesNumber;
bool lastAutoMode = autoMode;
bool lastVsync = vsync;
Color lastAppleColor;
float lastAudioVolume = audioVolume;

void GenerateApple()
{
    size_t newApplePosition = 0;
    bool loop = snake.body.size() + apples.size() < windowSize.x / scale * windowSize.y / scale;
    if (!loop) return;
    while (loop)
    {
        loop = false;
        newApplePosition = rand() % int(windowSize.x / scale * windowSize.y / scale);
        for (size_t k = 0; k < snake.body.size(); k++)
        {
            if (snake.body[k] == newApplePosition)
            {
                loop = true;
                break;
            }
        }
        if (loop) continue;
        for (size_t k = 0; k < apples.size(); k++)
        {
            if (apples[k].position == newApplePosition)
            {
                loop = true;
                break;
            }
        }
    }
    apples.push_back(Apple());
    apples[apples.size() - 1].position = newApplePosition;
    apples[apples.size() - 1].color = appleColor;
}

void Restart()
{
    snake.body.clear();
    snake.body.push_back(0);
    snake.body.push_back(1);
    snake.body.push_back(2);
    snake.direction = 'R';
    snake.keyBuffer.clear();
    snake.lastTailPosition = 0;

    lastKeyPress = 'R';

    apples.clear();
    for (int i = 0; i < applesNumber; i++)
        GenerateApple();

    isGameOver = false;
    isPaused = false;
    isHighscore = false;

    StopMusicStream(pauseMusic);
    StopMusicStream(funMusic);
    PlayMusicStream(bgMusic);
}

void ProcessPlayerInput()
{
    if (IsKeyPressed(KEY_UP) && lastKeyPress != 'D')
    {
        lastKeyPress = 'U';
        snake.keyBuffer.push_back('U');
    }
    if (IsKeyPressed(KEY_DOWN) && lastKeyPress != 'U')
    {
        lastKeyPress = 'D';
        snake.keyBuffer.push_back('D');
    }
    if (IsKeyPressed(KEY_LEFT) && lastKeyPress != 'R')
    {
        lastKeyPress = 'L';
        snake.keyBuffer.push_back('L');
    }
    if (IsKeyPressed(KEY_RIGHT) && lastKeyPress != 'L')
    {
        lastKeyPress = 'R';
        snake.keyBuffer.push_back('R');
    }
}

std::string GenerateAutoModeKeypresses()
{
    std::string output = "";
    for (size_t i = 0; i < horizontalCellsNumber - 3; i++)
        output += 'R';
    for (size_t i = 0; i < windowSize.y / scale - 1; i++)
        output += 'D';
    for (size_t i = 0; i < horizontalCellsNumber / 2 - 1; i++)
    {
        output += 'L';
        for (size_t j = 0; j < windowSize.y / scale - 2; j++)
            output += 'U';
        output += 'L';
        for (size_t j = 0; j < windowSize.y / scale - 2; j++)
            output += 'D';
    }
    output += 'L';
    for (size_t i = 0; i < windowSize.y / scale - 1; i++)
        output += 'U';
    for (size_t i = 0; i < 2; i++)
        output += 'R';
    return output;
}

void UpdateSettings()
{
    if (lastApplesNumber != applesNumber)
    {
        lastApplesNumber = applesNumber;
        apples.clear();
        for (int i = 0; i < applesNumber; i++)
            GenerateApple();
    }
    if (lastScale != scale || lastApplesNumber != applesNumber || lastAutoMode != autoMode)
    {
        lastScale = scale;
        horizontalCellsNumber = floor(windowSize.x / scale);
        lastApplesNumber = applesNumber;
        lastAutoMode = autoMode;
        Restart();
    }
    if (lastVsync != vsync)
    {
        lastVsync = vsync;
        if (!vsync)
            ClearWindowState(FLAG_VSYNC_HINT);
        else
            SetWindowState(FLAG_VSYNC_HINT);
    }
    if (lastAppleColor.r != appleColor.r || lastAppleColor.g != appleColor.g ||
        lastAppleColor.b != appleColor.b)
    {
        lastAppleColor = appleColor;
        for (size_t i = 0; i < apples.size(); i++)
            apples[i].color = appleColor;
    }
    if (lastAudioVolume != audioVolume)
    {
        lastAudioVolume = audioVolume;
        SetMasterVolume(audioVolume);
    }
}

void DrawSprites()
{
    // Print the background if it's fun mode
    if (mode == "fun")
        DrawTextureEx(isGameOver ? funGameOverTexture : funTexture, Vector2{0, 0}, 0,
                      (float)GetScreenHeight() / funTexture.height, WHITE);

    // Draw the snake
    for (size_t i = 0; i < snake.body.size(); i++)
    {
        float x = snake.body[i] % horizontalCellsNumber * scale *
                      (GetScreenWidth() * 1.f / windowSize.x) +
                  SNAKE_BORDER_SIZE;
        float y = snake.body[i] * 1.f / horizontalCellsNumber * scale *
                      (GetScreenHeight() * 1.f / windowSize.y) +
                  SNAKE_BORDER_SIZE;
        DrawRectangle(x, y, scale * (GetScreenWidth() * 1.f / windowSize.x) - SNAKE_BORDER_SIZE,
                      scale * (GetScreenHeight() * 1.f / windowSize.y) - SNAKE_BORDER_SIZE,
                      snake.color);
    }

    // Draw the apples
    for (size_t i = 0; i < apples.size(); i++)
    {
        float x = apples[i].position % horizontalCellsNumber * scale *
                      (GetScreenWidth() * 1.f / windowSize.x) +
                  SNAKE_BORDER_SIZE;
        float y = apples[i].position * 1.f / horizontalCellsNumber * scale *
                      (GetScreenHeight() * 1.f / windowSize.y) +
                  SNAKE_BORDER_SIZE;
        DrawRectangle(x, y, scale * (GetScreenWidth() * 1.f / windowSize.x) - SNAKE_BORDER_SIZE,
                      scale * (GetScreenHeight() * 1.f / windowSize.y) - SNAKE_BORDER_SIZE,
                      apples[i].color);
    }
}

void DrawFrame()
{
    BeginDrawing();

    ClearBackground(BLACK);

    if (IsKeyPressed(KEY_ESCAPE))
    {
        isPaused = !isPaused;
        if (isPaused)
        {
            PauseMusicStream(bgMusic);
            ResumeMusicStream(pauseMusic);
        }
        else
        {
            PauseMusicStream(pauseMusic);
            ResumeMusicStream(bgMusic);
        }
    }

    // Detect player input
    ProcessPlayerInput();

    // Snake physics stuff
    if ((GetTime() - delayClock >=
             std::max(1 / (snake.speed * sqrt(snake.body.size())), 1.0 / snake.maxSpeed) ||
         noSpeedLimit) &&
        !isGameOver && !isPaused)
    {
        delayClock = GetTime();

        // Add keypresses to a keybuffer if it's empty and autoMode is true
        if (snake.keyBuffer.size() == 0 && autoMode == true)
        {
            std::string buf = GenerateAutoModeKeypresses();
            for (size_t i = 0; i < buf.size(); i++)
                snake.keyBuffer.push_back(buf[i]);
        }

        // Change the snake's direction to the first key pressed in between the movement frames
        if (snake.keyBuffer.size() != 0)
        {
            snake.direction = snake.keyBuffer[0];
            snake.keyBuffer.erase(snake.keyBuffer.begin());
        }

        // Calculate the new head position
        int snakeDirectionInteger = 2;
        if (snake.direction == 'U') snakeDirectionInteger = -horizontalCellsNumber;
        if (snake.direction == 'D') snakeDirectionInteger = horizontalCellsNumber;
        if (snake.direction == 'L') snakeDirectionInteger = -1;
        if (snake.direction == 'R') snakeDirectionInteger = 1;
        snake.lastTailPosition = snake.body[0];
        // Remove the last tail piece
        snake.body.erase(snake.body.begin());
        // Move the head
        snake.body.push_back(snake.body[snake.body.size() - 1] + snakeDirectionInteger);

        // Check apple collision
        snake.CheckAppleCollision();

        // Check collision with the walls
        snake.CheckWallCollision();

        // Check self-collision
        snake.CheckSelfCollision();

        // fun mode check
        if (snake.body.size() >= (size_t)funModeLevel && mode != "fun" && funMode == true)
        {
            mode = "fun";
            PauseMusicStream(bgMusic);
            ResumeMusicStream(funMusic);
        }
        if (!(snake.body.size() >= (size_t)funModeLevel && funMode == true) && mode != "normal")
        {
            mode = "normal";
            PauseMusicStream(funMusic);
            ResumeMusicStream(bgMusic);
        }
    }

    // Update data fom settings
    UpdateSettings();

    DrawSprites();
    DrawUI();

    if (mode == "normal" && !isPaused) UpdateMusicStream(bgMusic);
    if (mode == "fun" && !isPaused) UpdateMusicStream(funMusic);
    if (isPaused) UpdateMusicStream(pauseMusic);
    if (!IsMusicStreamPlaying(bgMusic)) PlayMusicStream(bgMusic);
    if (!IsMusicStreamPlaying(funMusic)) PlayMusicStream(funMusic);
    if (!IsMusicStreamPlaying(pauseMusic)) PlayMusicStream(pauseMusic);

    EndDrawing();
}

void CleanUp()
{
    UnloadMusicStream(bgMusic);
    UnloadMusicStream(funMusic);
    UnloadMusicStream(pauseMusic);
    UnloadSound(pickupSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    UnloadTexture(funTexture);
    UnloadTexture(funGameOverTexture);
}
