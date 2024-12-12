#include "game.h"

Game::Game()
{

}

Game::~Game()
{

}

//SDL_initialization along with window and renderer
void Game::Init(int width, int height)
{
    //init
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::cout << "Failed To Initialize SDL \n";
        isRunning = false;
    }

    window_width = width;
    window_height = height;
    
    window = SDL_CreateWindow("Snek Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    render = SDL_CreateRenderer(window, -1, /*SDL_RENDERER_PRESENTVSYNC |*/ SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    //generate apples at random
    for (int i = 0; i < 100; i++)
    {
        apples.emplace_back(rand()%100*10, rand()%100*10, 10, 10);
    }

    //start the loop
    isRunning = true;
}

void Game::GameLoop(int screenFps, int frameDelay)
{
    //loop
    while (isRunning == true)
    {
        frameStart = SDL_GetTicks64();

        rectQueue.push_front(snakeHead);

        while (rectQueue.size() > size)
        {
            rectQueue.pop_back();
        }

        GameRender();
        GameUpdate();
        GameEvents();

        int frameTime = SDL_GetTicks64() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::GameUpdate()
{   
    //movement
    Movement();
    //collision
    Collision();
}

void Game::GameRender()
{
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    //draw snake
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    std::for_each(rectQueue.begin(), rectQueue.end(), [&](auto& snake_body)
    {
        SDL_RenderFillRect(render, &snake_body);
    });

    //draw apple
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    std::for_each(apples.begin(), apples.end(), [&](auto& apple)
    {
        SDL_RenderFillRect(render, &apple);
    });

    SDL_RenderPresent(render);
}

void Game::GameEvents()
{
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            KeyPress();
        }

        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
    }

}

void Game::KeyPress()
{
    switch (event.key.keysym.sym)
    {
        case (SDLK_UP):
            if (moveState != 2)
            {
                moveState = 1;
            }
            break;
        case (SDLK_DOWN):
            if (moveState != 1)
            {
                moveState = 2;
            }
            break;
        case (SDLK_RIGHT):
            if (moveState != 4)
            {
                moveState = 3;
            }
            break;
        case (SDLK_LEFT):
            if (moveState != 3)
            {
                moveState = 4;
            }
            break;
        default:
            break;
    }
}

void Game::Movement()
{
    switch (moveState)
    {
        case 1: //up
            snakeHead.y -= moveSpeed;
            break;

        case 2: //down
            snakeHead.y += moveSpeed;
            break;

        case 3: //right
            snakeHead.x += moveSpeed;
            break;

        case 4: //left
            snakeHead.x -= moveSpeed;
            break;

        default:
            break;
    }
}

void Game::Collision()
{
    //window border collision
    if (snakeHead.y + 10 < 0)
    {
        snakeHead.y = window_height + moveSpeed;
    }

    if (snakeHead.y - 10 > window_height)
    {
        snakeHead.y = 0 - moveSpeed;
    }

    if (snakeHead.x - 10 > window_width)
    {
        snakeHead.x = 0 - moveSpeed;
    }

    if (snakeHead.x + 10 < 0)
    {
        snakeHead.x = window_width + moveSpeed;
    }

    //apples collision
    std::for_each(apples.begin(), apples.end(), [&](auto& apple) 
    {
        SDL_bool appleCollision = SDL_HasIntersection(&snakeHead, &apple);
        if (appleCollision)
        {
            size +=1;
            apple.x = -20;
            apple.y = -20;
        }
    });

    //snake collision
    std::for_each(rectQueue.begin(), rectQueue.end(), [&](auto& snake_body)
    {
        if (snakeHead.x == snake_body.x && snakeHead.y == snake_body.y)
        {
            size = 1;
        }
    });
}

void Game::CloseApp()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}