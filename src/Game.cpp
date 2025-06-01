#include "Game.hpp"
#include "UI.hpp"
#include "Settings.hpp"

unsigned int windowSize[2]{16*50*2, 9*50*2};
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

std::vector<int> scores;
std::vector<Apple> apples;

double delayClock;
char lastKeyPress = 'R';
float lastFunModeLevel = funModeLevelFloat;
float lastScale = scaleFloat;
float lastApplesNumber = applesNumberFloat;
bool lastAutoMode = autoMode;
bool lastVsync = vsync;
Color lastAppleColor;
float lastAudioVolume = audioVolume;

void GenerateApple()
{
    int newApplePosition = 0;
    bool loop = snake.body.size() + apples.size() < windowSize[0] / scale * windowSize[1] / scale;
    if (!loop) return;
    while (loop)
    {
        loop = false;
        newApplePosition = rand() % (windowSize[0] / scale * windowSize[1] / scale);
        for (int k = 0; k < snake.body.size(); k++)
        {
            if (snake.body[k] == newApplePosition)
            {
                loop = true;
                break;
            }
        }
        if (loop) continue;
        for (int k = 0; k < apples.size(); k++)
        {
            if (apples[k].position == newApplePosition)
            {
                loop = true;
                break;
            }
        }
    }
    apples.push_back(Apple());
    apples[apples.size()-1].position = newApplePosition;
    apples[apples.size()-1].color = appleColor;
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

    scores.clear();
    #if !defined(PLATFORM_WEB)
    std::fstream scoreListFile;
    scoreListFile.open("score.txt", std::ios::in);
    std::string buf;
    while (std::getline(scoreListFile, buf))
        scores.push_back(stoi(buf));
    scoreListFile.close();
    #else
    scores.push_back(3);
    #endif

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
    for (int i = 0; i < horizontalCellsNumber - 3; i++)
        output += 'R';
    for (int i = 0; i < windowSize[1] / scale - 1; i++)
        output += 'D';
    for (int i = 0; i < horizontalCellsNumber / 2 - 1; i++)
    {
        output += 'L';
        for (int j = 0; j < windowSize[1] / scale - 2; j++)
            output += 'U';
        output += 'L';
        for (int j = 0; j < windowSize[1] / scale - 2; j++)
            output += 'D';
    }
    output += 'L';
    for (int i = 0; i < windowSize[1] / scale - 1; i++)
        output += 'U';
    for (int i = 0; i < 2; i++)
        output += 'R';
    return output;
}

void UpdateSettings()
{
    if (lastFunModeLevel != funModeLevelFloat)
    {
        lastFunModeLevel = funModeLevelFloat;
        funModeLevel = funModeLevelFloat;
    }
    if (lastApplesNumber != applesNumberFloat)
    {
        lastApplesNumber = applesNumberFloat;
        if ((int)applesNumberFloat != applesNumber)
        {
            applesNumber = applesNumberFloat;
            apples.clear();
            for (int i = 0; i < applesNumber; i++)
                GenerateApple();
        }
    }
    if (lastScale != scaleFloat || lastApplesNumber != applesNumberFloat || lastAutoMode != autoMode)
    {
        lastScale = scaleFloat;
        scale = scaleFloat;
        horizontalCellsNumber = floor(windowSize[0] / scale);
        lastApplesNumber = applesNumberFloat;
        applesNumber = applesNumberFloat;
        lastAutoMode = autoMode;
        Restart();
    }
    if (lastVsync != vsync)
    {
        lastVsync = vsync;
        if (!vsync) ClearWindowState(FLAG_VSYNC_HINT);
        else SetWindowState(FLAG_VSYNC_HINT);
    }
    if (lastAppleColor.r != appleColor.r || lastAppleColor.g != appleColor.g || lastAppleColor.b != appleColor.b)
    {
        lastAppleColor = appleColor;
        for (int i = 0; i < apples.size(); i++)
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
    // Print the background if it's china mode
    if (mode == "china")
        DrawTextureEx(isGameOver ? funGameOverTexture : funTexture, Vector2{0, 0}, 0, (float)GetScreenHeight() / funTexture.height, WHITE);

    // Draw the snake
    for (int i = 0; i < snake.body.size(); i++)
        DrawRectangle((snake.body[i] % horizontalCellsNumber) * scale * (GetScreenWidth() * 1.f / windowSize[0]) + SNAKE_BORDER_SIZE, 
            (snake.body[i] / horizontalCellsNumber) * scale * (GetScreenHeight() * 1.f / windowSize[1]) + SNAKE_BORDER_SIZE,
            scale * (GetScreenWidth() * 1.f / windowSize[0]) - SNAKE_BORDER_SIZE, 
            scale * (GetScreenHeight() * 1.f / windowSize[1]) - SNAKE_BORDER_SIZE, snake.color);

    // Draw the apples
    for (int i = 0; i < apples.size(); i++)
        DrawRectangle((apples[i].position % horizontalCellsNumber) * scale * (GetScreenWidth() * 1.f / windowSize[0]) + SNAKE_BORDER_SIZE, 
            (apples[i].position / horizontalCellsNumber) * scale * (GetScreenHeight() * 1.f / windowSize[1]) + SNAKE_BORDER_SIZE,
            scale * (GetScreenWidth() * 1.f / windowSize[0]) - SNAKE_BORDER_SIZE, 
            scale * (GetScreenHeight() * 1.f / windowSize[1]) - SNAKE_BORDER_SIZE, apples[i].color);
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
    if ((GetTime() - delayClock >= std::max(1 / (snake.speed * sqrt(snake.body.size())), 1.0 / snake.maxSpeed) || noSpeedLimit) && !isGameOver && !isPaused)
    {
        delayClock = GetTime();

        // Add keypresses to a keybuffer if it's empty and autoMode is true
        if (snake.keyBuffer.size() == 0 && autoMode == true)
        {
            std::string buf = GenerateAutoModeKeypresses();
            for (int i = 0; i < buf.size(); i++)
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
        snake.body.push_back(snake.body[snake.body.size()-1] + snakeDirectionInteger);

        // Check apple collision
        snake.CheckAppleCollision();

        // Check collision with the walls
        snake.CheckWallCollision();

        // Check self-collision
        snake.CheckSelfCollision();
    
        // China mode check
        if (snake.body.size() >= funModeLevel && mode != "china" && funMode == true)
        {
            mode = "china";
            PauseMusicStream(bgMusic);
            ResumeMusicStream(funMusic);
        }
        if (!(snake.body.size() >= funModeLevel && funMode == true) && mode != "normal")
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
    if (mode == "china" && !isPaused) UpdateMusicStream(funMusic);
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
