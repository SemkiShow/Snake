#include <random>
#include <ctime>
#include "Settings.hpp"
#include "UI.hpp"
#include "Game.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int main()
{
    srand(time(0));
    Load("settings.txt");

    int flags = 0;
    if (vsync) flags |= FLAG_VSYNC_HINT;
    #if !defined(PLATFORM_WEB)
    flags |= FLAG_WINDOW_HIGHDPI;
    flags |= FLAG_WINDOW_RESIZABLE;
	#endif
    SetConfigFlags(flags);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

    InitWindow(windowSize[0], windowSize[1], "Snake");
    SetExitKey(-1);
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(8192);
    SetMasterVolume(audioVolume);

    GuiSetFont(GetFontDefault());
    
    bgMusic = LoadMusicStream("resources/snake-bg.ogg");
    PlayMusicStream(bgMusic);
    funMusic = LoadMusicStream("resources/fun-bg.ogg");
    SetMusicVolume(funMusic, 2);
    pauseMusic = LoadMusicStream("resources/pause-music.ogg");
    pickupSound = LoadSound("resources/pickup.ogg");
    gameOverSound = LoadSound("resources/game-over.ogg");
    Image image = LoadImage("resources/fun-bg.jpg");
    funTexture = LoadTextureFromImage(image);
    image = LoadImage("resources/fun-bg-gameover.jpg");
    funGameOverTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    Restart();

    while (!WindowShouldClose())
    {
        DrawFrame();
    }

    CleanUp();

    Save("settings.txt");
    CloseWindow();

    return 0;
}
