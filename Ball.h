#pragma once
#include "Pong.h"

class Ball
{
public:
    // default constructor
    Ball(int x, int y, int r);
    Ball();
    // default destructor
    ~Ball();
    void Bounce(Player player);
    void Init();
    void Move();
    void Reset();
    void SetVec(int x, int y);
    void SetPos(int x, int y);
    // check if the xy-pos will hit the wall or not
    bool HitWall(const Vector2D& v) const;
    bool HitWall();
    bool HitPlayer(int x, int y, int w, int h, Player p);
    int GetX() { return m_pos.m_x; };
    int GetY() { return m_pos.m_y; };
    int GetRadius() { return m_radius; };
    uint32_t GetUX() { return m_pos.m_x; };
    uint32_t GetUY() { return m_pos.m_y; };
    uint32_t GetVX() { return m_vec.m_x; };
    uint32_t GetVY() { return m_vec.m_y; };
    uint32_t GetURadius() { return m_radius; };
    void operator=(const Ball& ball);
private:
    // radius
    int m_radius;
    // velocity
    Vector2D m_vec;
    // position
    Vector2D m_pos;
};

