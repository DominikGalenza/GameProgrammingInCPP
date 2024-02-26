#pragma once
#include "SDL.h"
#include "vector"

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 position;
	Vector2 velocity;
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
	std::vector<Ball> mBalls;
};

