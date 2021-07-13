#include "PongClient.h"

// default constructor
PongClient::PongClient(): m_uid(0), m_score(0), m_x(0), m_y(0), m_width(0), m_height(0), m_player(Player::EMPTY)
{
	ConnectToServer();
}

// default destructor
PongClient::~PongClient()
{

}

// add score when opponent fails to hit the ball back 
void PongClient::AddScore()
{
	m_score += 1;
}

void PongClient::Move(int x, int y)
{
	m_x = ClampX(m_x + x);
	m_y = ClampY(m_y + y);
}

void PongClient::SetXYWH()
{
	switch (m_player)
	{
	case Player::P1:
		m_x = World::WIDTH / 30;
		m_y = World::HEIGHT * 2 / 5;
		m_width = World::WIDTH / 30;
		m_height = World::HEIGHT / 5;
		break;
	case Player::P2:
		m_x = World::WIDTH * 28 / 30;
		m_y = World::HEIGHT * 2 / 5;
		m_width = World::WIDTH / 30;
		m_height = World::HEIGHT / 5;
		break;
	case Player::P3:
		m_x = World::WIDTH * 2 / 5;
		m_y = World::HEIGHT / 30;
		m_width = World::WIDTH / 5;
		m_height = World::HEIGHT / 30;
		break;
	case Player::P4:
		m_x = World::WIDTH * 2 / 5;
		m_y = World::HEIGHT * 28 / 30;
		m_width = World::WIDTH / 5;
		m_height = World::HEIGHT / 30;
		break;
	default:
		break;
	}
}

void PongClient::SetID(uint32_t id)
{
	m_uid = id;
	m_player = static_cast<Player>(m_uid % 10000);
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

uint32_t PongClient::GetID()
{
	return m_uid;
}

uint32_t PongClient::GetScore()
{
	return m_score;
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

int PongClient::GetX()
{
	return m_x;
}

int PongClient::GetY()
{
	return m_y;
}

int PongClient::GetWidth()
{
	return m_width;
}	

int PongClient::GetHeight()
{
	return m_height;
}

Player PongClient::GetPlayer()
{
	return m_player;
}

uint32_t PongClient::GetUX()
{
	return m_x;
}

uint32_t PongClient::GetUY()
{
	return m_y;
}

uint32_t PongClient::GetUWidth()
{
	return m_width;
}

uint32_t PongClient::GetUHeight()
{
	return m_height;
}