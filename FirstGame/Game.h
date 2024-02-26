#pragma once
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	Uint32 mTicksCount;
	int mPaddleDirection;
	Vector2 mPaddlePosition;
	int mSecondPaddleDirection;
	Vector2 mSecondPaddlePosition;
	Vector2 mBallPosition;
	Vector2 mBallVelocity;
};

