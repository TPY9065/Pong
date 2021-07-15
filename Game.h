#pragma once
#include "PongClient.h"
#include "Ball.h"

struct Opponent
{
public:
	Opponent() {};
	Opponent(uint32_t id) : m_uid(id), m_player(static_cast<Player>(id % 10000)), m_x(0), m_y(0), m_width(0), m_height(0) {};
	uint32_t GetID() { return m_uid; };
	int GetX() { return m_x; };
	int GetY() { return m_y; };
	int GetWidth() { return m_width; };
	int GetHeight() { return m_height; };
	Player GetPlayer() { return m_player; };
	void SetXY(int x, int y)
	{
		m_x = x;
		m_y = y;
	}
	void SetXYWH(int x, int y, int w, int h)
	{
		m_x = x;
		m_y = y;
		m_width = w;
		m_height = h;
	}
private:
	// client id
	uint32_t m_uid = 0;
	// player ID
	Player m_player = Player::EMPTY;
	// paddle props
	int m_x = 0;
	int m_y = 0;
	int m_width = 0;
	int m_height = 0;
};

class Game: PongClient
{
public:
	Game();
	~Game();
	void Init();
	void InitWindow();
	void InitRenderer();
	void GetInput();
	// draw a circle using Bresenham¡¦s circle drawing algorithm
	void DrawBall();
	void Draw();
	void MoveBall();
	void Update();
	void Run();
	void AddPlayer();
	bool IsInit();
private:
	std::unordered_map<uint32_t, Opponent> m_opponents;
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	SDL_Event m_userInput;
	Ball m_ball;
	bool m_running;
	bool m_start;
};

