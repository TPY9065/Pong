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

void Ball::Bounce()
{
	if (HitWall())
	{
		if (m_pos.m_x + m_vec.m_x - m_radius <= 0 || m_pos.m_x + m_vec.m_x + m_radius >= World::WIDTH)
			if (m_vec.m_x < 0)
				m_vec.SetXY(rand() % 5 + 5, m_vec.m_y);
			else
				m_vec.SetXY(-(rand() % 5 + 5), m_vec.m_y);
		else if (m_pos.m_y + m_vec.m_y - m_radius <= 0 || m_pos.m_y + m_vec.m_y + m_radius >= World::HEIGHT)
			if (m_vec.m_y < 0)
				m_vec.SetXY(m_vec.m_x, rand() % 5 + 5);
			else
				m_vec.SetXY(m_vec.m_x, -(rand() % 5 + 5));
	}
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

bool Ball::HitWall(const Vector2D& v) const
{
	// return true if the coordinate of the ball is going to hit the boundary
	if (v.m_x - m_radius <= 0 || v.m_x + m_radius >= World::WIDTH || v.m_y - m_radius <= 0 || v.m_y + m_radius >= World::HEIGHT)
		return true;
	return false;
}

bool Ball::HitWall()
{
	return HitWall(m_pos + m_vec);
}

void Ball::operator=(const Ball& ball)
{
	m_pos = ball.m_pos;
	m_vec = ball.m_vec;
	m_radius = ball.m_radius;
}