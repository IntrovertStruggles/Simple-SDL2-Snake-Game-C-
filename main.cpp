#include "game.h"

Game game;

int main(int argc, char* argv[])
{
    int screenFps = 60;
    const int frameDelay = 1000/screenFps;
    
    game.Init(800, 800);
    game.GameLoop(screenFps, frameDelay);
    game.CloseApp();
    
    return 0;
}