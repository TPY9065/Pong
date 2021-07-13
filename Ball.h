#pragma once

class Ball
{
public:
    // default constructor
    Ball();
    // default destructor
    ~Ball();
    void Move();
    void Reset();
    bool HitWall();
    void SetXY(float x, float y);
    float GetX();
    float GetY();
private:
    // x position
    float m_x;
    // y position
    float m_y;
    // velocity
    float m_v;
};

