#pragma once
#include "NetServer.h"
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

enum class Protocal
{
    ACK = 0,
    POS_UPDATE,
    BALL_POS,
    PLAYER_CONNECT,
    PLAYER_DISCONNECT
};

class PongServer: public NetServer<Protocal>
{
public:
    void Start();
    void WaitForConnection();
    void Update();
    void Run();
private:
    int m_player = 0;
};

