#pragma once
#include "PongClient.h"
#include "Ball.h"

struct Opponent
{
public:
	Opponent() {};
	Opponent(uint32_t id) : m_uid(id), m_player(static_cast<Player>(id % 10000)), m_x(0), m_y(0), m_width(0), m_height(0) {};
	void SetID(int id)
	{
		m_uid = id;
		m_player = static_cast<Player>(id % 10000);
	}
	void SetPlayer(Player p)
	{
		m_player = p;
	}
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
	void Reset()
	{
		switch (m_player)
		{
		case Player::P1:
			m_x = World::WIDTH / 30;
			m_y = World::HEIGHT * 2 / 5;
			m_width = World::WIDTH / 40;
			m_height = World::HEIGHT / 5;
			break;
		case Player::P2:
			m_x = World::WIDTH * 28 / 30;
			m_y = World::HEIGHT * 2 / 5;
			m_width = World::WIDTH / 40;
			m_height = World::HEIGHT / 5;
			break;
		case Player::P3:
			m_x = World::WIDTH * 2 / 5;
			m_y = World::HEIGHT / 30;
			m_width = World::WIDTH / 5;
			m_height = World::HEIGHT / 40;
			break;
		case Player::P4:
			m_x = World::WIDTH * 2 / 5;
			m_y = World::HEIGHT * 28 / 30;
			m_width = World::WIDTH / 5;
			m_height = World::HEIGHT / 40;
			break;
		case Player::EMPTY:
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;
		default:
			break;
		}
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

enum class GameState
{
	IDLE = 0,
	PLAY,
	FINISH
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
	void SetSelfID(int id);
	void AddPlayer(int id);
	void RemovePlayer(int id);
	void PlayerUpdate(int id, int x, int y, int w, int h);
	void SetPlayerLose(int id, Player p);
	void Reset();
	bool IsInit();
	bool GameFinished();
private:
	std::unordered_map<uint32_t, Opponent> m_opponents;
	// use to check if current key is pressed of released: U D L R
	std::unordered_map<char, bool> m_bKeyDown;
	std::unordered_map<Player, bool> m_bPlayerExist;
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	SDL_Event m_userInput;
	Ball m_ball;
	GameState m_state;
	bool m_bKeyPressed;
	bool m_running;
	bool m_bLose;
	Player m_winner;
	int m_place;
};

