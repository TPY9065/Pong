#pragma once
#include "Pong.h"

enum class Protocal
{
    ACK = 0,
    PLAYER_UPDATE,
    BALL_UPDATE,
    PLAYER_LOSE,
    PLAYER_CONNECT,
    PLAYER_DISCONNECT
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
    // move towards x and y steps
    void Move(int stepX, int stepY);
    // set the plate xy-pos and width height
    void SetXYWH();
    // set the client's ID
    void SetID(uint32_t id);
    // main function of the client
    void Run();
    // send message to server
    void MessageToServer(NetMessage<Protocal> msg);
    uint32_t GetID() { return m_uid; };
    uint32_t GetScore() { return m_score; };
    int ClampX(int x);
    int ClampY(int y);
    int GetX() { return m_x; };
    int GetY() { return m_y; };
    int GetWidth() { return m_width; };
    int GetHeight() { return m_height; };
    uint32_t GetUX() { return m_x; };
    uint32_t GetUY() { return m_y; };
    uint32_t GetUWidth() { return m_width; };
    uint32_t GetUHeight() { return m_height; };
    Player GetPlayer() { return m_player; };
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
