#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <deque>
#include <algorithm>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

class Game {
public:
    Game();
    ~Game();
    void Init(int window_width, int window_height);
    void GameLoop(int screenFps, int frameDelay);
    void GameUpdate();
    void GameRender();
    void GameEvents();
    void Movement();
    void KeyPress();
    void Collision();
    void CloseApp();
private:
    //init
    Uint64 frameStart;
    Uint64 frameEnd;
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Rect snakeHead { 395, 395, 10, 10 };
    SDL_Rect gameBoundary;
    SDL_Rect srcRect, dstRect;
    SDL_Event event;

    //variables
    bool isRunning;
    int score {};
    int partsCounter;
    int moveState { 0 };
    int size = 1;
    int window_width {};
    int window_height {};
    const int moveSpeed = 5; 

    //container
    std::deque<SDL_Rect> rectQueue;

    std::vector<SDL_Rect> apples;
};

#endif