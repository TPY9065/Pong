#include "Ball.h"

Ball::Ball(int x, int y, int r) : m_pos(x, y), m_vec(), m_radius(r)
{
	srand(time(NULL));
}

Ball::Ball() : m_pos(), m_vec(), m_radius(0)
{

}

Ball::~Ball()
{

}

void Ball::Bounce(Player player)
{
	if (player == Player::P1 || player == Player::P2 || m_pos.m_x + m_vec.m_x - m_radius <= 0 || m_pos.m_x + m_vec.m_x + m_radius >= World::WIDTH)
		m_vec.SetXY(-m_vec.m_x, m_vec.m_y);
	else if (player == Player::P3 || player == Player::P4 || m_pos.m_y + m_vec.m_y - m_radius <= 0 || m_pos.m_y + m_vec.m_y + m_radius >= World::HEIGHT)
		m_vec.SetXY(m_vec.m_x, -m_vec.m_y);
}

void Ball::Init()
{
	// randomly generate the velocity of the ball if it has zero velocity
	if (m_vec == ZERO)
	{
		// get random x and y velocity
		m_vec.SetXY(rand() % 5 + 5, rand() % 5 + 5);
	}
}

void Ball::Move()
{
	if (HitWall())  
	{
		if (m_pos.m_x + m_vec.m_x + m_radius >= World::WIDTH)
			m_pos.m_x = World::WIDTH - m_radius;
		else if (m_pos.m_x + m_vec.m_x - m_radius <= 0)
			m_pos.m_x = 0 + m_radius;
		if (m_pos.m_y + m_vec.m_y + m_radius >= World::HEIGHT)
			m_pos.m_y = World::HEIGHT - m_radius;
		else if (m_pos.m_y + m_vec.m_y - m_radius <= 0)
			m_pos.m_y = 0 + m_radius;
	}
	else
		m_pos += m_vec;
}

void Ball::Reset()
{
	// reset velocity
	m_vec.SetXY(0, 0);
	// reset xy-position
	m_pos.SetXY(World::WIDTH / 2, World::HEIGHT / 2);
}

void Ball::SetVec(int x, int y)
{
	m_vec.SetXY(x, y);
}

void Ball::SetPos(int x, int y)
{
	m_pos.SetXY(x, y);
}

bool Ball::HitWall()
{
	Vector2D v = m_pos + m_vec;
	if (v.m_x - m_radius <= 0 || v.m_x + m_radius >= World::WIDTH || v.m_y - m_radius <= 0 || v.m_y + m_radius >= World::HEIGHT)
		return true;
	return false;
}

bool Ball::HitPlayer(int x, int y, int w, int h)
{
	// find the xy difference between the center of the ball and the plate
	int xDiff = m_pos.m_x + m_vec.m_x - (x + w / 2);
	int yDiff = m_pos.m_y + m_vec.m_y - (y + h / 2);
	int xLimit = m_radius + w / 2;
	int yLimit = m_radius + h / 2;

	if (abs(xDiff) < xLimit && abs(yDiff) < yLimit)
		return true;
	return false;
}

void Ball::operator=(const Ball& ball)
{
	m_pos = ball.m_pos;
	m_vec = ball.m_vec;
	m_radius = ball.m_radius;
}