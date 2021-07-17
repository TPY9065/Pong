#pragma once

#include "NetClient.h"
#include <iostream>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>

enum class Player
{
    P1 = 0,
    P2,
    P3,
    P4,
    EMPTY
};

class World
{
public:
	World() = delete;
	World(const World& w) = delete;
	void operator=(const World w) = delete;
	static const int WIDTH = 480;
	static const int HEIGHT = 480;
	static const int MAX_PLAYER = 4;
};


struct Vector2D
{
public:
    Vector2D() { m_x = 0; m_y = 0; };
    Vector2D(int x, int y) { m_x = x; m_y = y; };
    Vector2D(const Vector2D& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
    }
    void SetXY(int x, int y) { m_x = x; m_y = y; }
    void MoveXY(int x, int y) { m_x += x; m_y += y; }
    Vector2D operator+(Vector2D v)
    {
        Vector2D newV(m_x + v.m_x, m_y + v.m_y);
        return newV;
    };
    void operator+=(const Vector2D& v)
    {
        m_x += v.m_x;
        m_y += v.m_y;
    }
    bool operator==(const Vector2D& v) const
    {
        if (m_x == v.m_x && m_y == v.m_y)
            return true;
        return false;
    };
    void operator=(const Vector2D& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
    }
    int m_x = 0;
    int m_y = 0;
};

static const Vector2D ZERO(0, 0);