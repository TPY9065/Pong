#pragma once
#include "Pong.h"

enum class Protocal
{
    ACK = 0,
    POS_UPDATE,
    BALL_POS,
    PLAYER_CONNECT,
    PLAYER_DISCONNECT
};

enum class Player
{
    P1 = 0,
    P2,
    P3,
    P4,
    EMPTY
};

class PongClient: public NetClient<Protocal>
{
public:
    // default constructor
    PongClient();
    // default destructor
    ~PongClient();
    // add score when other player fails to hit the ball back 
    void AddScore();
    void Move(int x, int y);
    void SetXYWH();
    void SetID(uint32_t id);
    void Run();
    void MessageToServer(NetMessage<Protocal> msg);
    uint32_t GetID();
    uint32_t GetScore();
    int ClampX(int x);
    int ClampY(int y);
    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    uint32_t GetUX();
    uint32_t GetUY();
    uint32_t GetUWidth();
    uint32_t GetUHeight();
    Player GetPlayer();
private:
    // client id
    uint32_t m_uid;
    // player's score
    uint32_t m_score;
    // player ID
    Player m_player;
    // paddle props
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};
