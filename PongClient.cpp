#include "PongClient.h"

// default constructor
PongClient::PongClient(): m_uid(0), m_score(5), m_x(0), m_y(0), m_width(0), m_height(0), m_player(Player::EMPTY)
{
	ConnectToServer();
}

// default destructor
PongClient::~PongClient()
{

}

// add score when opponent fails to hit the ball back 
void PongClient::LosePoint()
{
	m_score -= 1;
	switch (m_player)
	{
	case Player::P1:
	case Player::P2:
		m_height = World::HEIGHT * (5 - m_score + 1) / 15;
		break;
	case Player::P3:
	case Player::P4:
		m_width = World::WIDTH * (5 - m_score + 1) / 15;
		break;
	case Player::EMPTY:
		break;
	default:
		break;
	}
}

void PongClient::Move(int stepX, int stepY)
{
	m_x = ClampX(m_x + stepX);
	m_y = ClampY(m_y + stepY);
}

void PongClient::SetXYWH()
{
	switch (m_player)
	{
	case Player::P1:
		m_x = World::WIDTH / 30;
		m_y = World::HEIGHT * 2 / 5;
		m_width = World::WIDTH / 40;
		m_height = World::HEIGHT / 8;
		break;
	case Player::P2:
		m_x = World::WIDTH * 28 / 30;
		m_y = World::HEIGHT * 2 / 5;
		m_width = World::WIDTH / 40;
		m_height = World::HEIGHT / 8;
		break;
	case Player::P3:
		m_x = World::WIDTH * 2 / 5;
		m_y = World::HEIGHT / 30;
		m_width = World::WIDTH / 8;
		m_height = World::HEIGHT / 40;
		break;
	case Player::P4:
		m_x = World::WIDTH * 2 / 5;
		m_y = World::HEIGHT * 28 / 30;
		m_width = World::WIDTH / 8;
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

void PongClient::SetID(uint32_t id)
{
	m_uid = id;
	m_player = static_cast<Player>(m_uid % 10000);
}

void PongClient::SetPlayer(Player p)
{
	m_player = p;
}

void PongClient::Run()
{
	while (true)
		Update();
}

void PongClient::MessageToServer(NetMessage<Protocal> msg)
{
	WriteMessage(msg);
}

void PongClient::SetScore(int score)
{
	m_score = std::max(1, score);
}

int PongClient::ClampX(int x)
{
	if (x + m_width > World::WIDTH)
		return World::WIDTH - m_width;
	else if (x < 0)
		return 0;
	else
		return x;
}

int PongClient::ClampY(int y)
{
	if (y + m_height > World::HEIGHT)
		return World::HEIGHT - m_height;
	else if (y < 0)
		return 0;
	else
		return y;
}