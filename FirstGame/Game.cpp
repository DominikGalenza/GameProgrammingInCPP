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
    mSecondPaddlePosition.x = 1000.0f;
    mSecondPaddlePosition.y = 768.0f / 2.0f;
    Ball firstBall;
    firstBall.position.x = 1024.0f / 2.0f;
    firstBall.position.y = 768.0f / 2.0f;
    firstBall.velocity.x = -200.0f;
    firstBall.velocity.y = 235.0f;
    Ball secondBall;
    secondBall.position.x = 1024.0f / 3.0f;
    secondBall.position.y = 768.0f / 2.0f;
    secondBall.velocity.x = 200.0f;
    secondBall.velocity.y = 235.0f;
    mBalls.push_back(firstBall);
    mBalls.push_back(secondBall);
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

    mSecondPaddleDirection = 0;
    if (state[SDL_SCANCODE_I])
    {
        mSecondPaddleDirection -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        mSecondPaddleDirection += 1;
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

    if (mSecondPaddleDirection != 0)
    {
        mSecondPaddlePosition.y += mSecondPaddleDirection * 300.0f * deltaTime;
        if (mSecondPaddlePosition.y < (paddleHeight / 2.0f + thickness))
        {
            mSecondPaddlePosition.y = paddleHeight / 2.0f + thickness;
        }
        else if (mSecondPaddlePosition.y > (768.0f - paddleHeight / 2.0f - thickness))
        {
            mSecondPaddlePosition.y = 768.0f - paddleHeight / 2.0f - thickness;
        }
    }

    for (Ball &ball : mBalls)
    {
        ball.position.x += ball.velocity.x * deltaTime;
        ball.position.y += ball.velocity.y * deltaTime;

        float ballPaddleDifference = mPaddlePosition.y - ball.position.y;
        float ballSecondPaddleDifference = mSecondPaddlePosition.y - ball.position.y;
        ballPaddleDifference = ballPaddleDifference > 0.0f ? ballPaddleDifference : -ballPaddleDifference;
        ballSecondPaddleDifference = ballSecondPaddleDifference > 0.0f ? ballSecondPaddleDifference : -ballSecondPaddleDifference;
        if (ballPaddleDifference <= paddleHeight / 2.0f && ball.position.x <= 25.0f && ball.position.x >= 20.0f && ball.velocity.x < 0.0f
            || ballSecondPaddleDifference <= paddleHeight / 2.0f && ball.position.x >= 1000.0f && ball.position.x <= 1005.0f && ball.velocity.x > 0.0f)
        {
            ball.velocity.x *= -1.0f;
        }
        else if (ball.position.x <= 0.0f || ball.position.x >= 1024.0f)
        {
            mIsRunning = false;
        }

        if (ball.position.y <= thickness && ball.velocity.y < 0.0f)
        {
            ball.velocity.y *= -1;
        }
        else if (ball.position.y >= (768 - thickness) && ball.position.y > 0.0f)
        {
            ball.velocity.y *= -1;
        }
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

    SDL_Rect paddle{
        static_cast<int>(mPaddlePosition.x),
        static_cast<int>(mPaddlePosition.y - paddleHeight / 2),
        thickness,
        static_cast<int>(paddleHeight)
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_Rect secondPaddle{
        static_cast<int>(mSecondPaddlePosition.x),
        static_cast<int>(mSecondPaddlePosition.y - paddleHeight / 2),
        thickness,
        static_cast<int>(paddleHeight)
    };
    SDL_RenderFillRect(mRenderer, &secondPaddle);

    for (Ball &_ball : mBalls)
    {
        SDL_Rect ball{
            static_cast<int>(_ball.position.x - thickness / 2),
            static_cast<int>(_ball.position.y - thickness / 2),
            thickness,
            thickness
        };
        SDL_RenderFillRect(mRenderer, &ball);
        SDL_RenderPresent(mRenderer);
    }
}
