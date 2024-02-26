#include "Game.h"

const int thickness = 15;
const float paddleHeight = 100.0f;

Game::Game():mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDirection(0)
{
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)", 100, 100, 1024, 768, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mPaddlePosition.x = 10.0f;
    mPaddlePosition.y = 768.0f / 2.0f;
    mBallPosition.x = 1024.0f / 2.0f;
    mBallPosition.y = 768.0f / 2.0f;
    mBallVelocity.x = -200.0f;
    mBallVelocity.y = 235.0f;
    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    mPaddleDirection = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDirection -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDirection += 1;
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();

    if (mPaddleDirection != 0)
    {
        mPaddlePosition.y += mPaddleDirection * 300.0f * deltaTime;
        if (mPaddlePosition.y < (paddleHeight / 2.0f + thickness))
        {
            mPaddlePosition.y = paddleHeight / 2.0f + thickness;
        }
        else if (mPaddlePosition.y > (768.0f - paddleHeight / 2.0f - thickness))
        {
            mPaddlePosition.y = 768.0f - paddleHeight / 2.0f - thickness;
        }
    }

    mBallPosition.x += mBallVelocity.x * deltaTime;
    mBallPosition.y += mBallVelocity.y * deltaTime;

    float ballPaddleDifference = mPaddlePosition.y - mBallPosition.y;
    ballPaddleDifference = ballPaddleDifference > 0.0f ? ballPaddleDifference : -ballPaddleDifference;
    if (ballPaddleDifference <= paddleHeight / 2.0f && mBallPosition.x <= 25.0f && mBallPosition.x >= 20.0f && mBallVelocity.x < 0.0f)
    {
        mBallVelocity.x *= -1.0f;
    }
    else if (mBallPosition.x <= 0.0f)
    {
        mIsRunning = false;
    }
    else if (mBallPosition.x >= (1024.0f - thickness) && mBallVelocity.x > 0.0f)
    {
        mBallVelocity.x *= -1.0f;
    }

    if (mBallPosition.y <= thickness && mBallVelocity.y < 0.0f)
    {
        mBallVelocity.y *= -1;
    }
    else if (mBallPosition.y >= (768 - thickness) && mBallPosition.y > 0.0f)
    {
        mBallVelocity.y *= -1;
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect wall{ 
        0, 
        0, 
        1024, 
        thickness 
    };
    SDL_RenderFillRect(mRenderer, &wall);
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);

    SDL_Rect paddle{
        static_cast<int>(mPaddlePosition.x),
        static_cast<int>(mPaddlePosition.y - paddleHeight / 2),
        thickness,
        static_cast<int>(paddleHeight)
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_Rect ball{
        static_cast<int>(mBallPosition.x - thickness / 2),
        static_cast<int>(mBallPosition.y - thickness / 2),
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderPresent(mRenderer);
}
