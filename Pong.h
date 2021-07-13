#pragma once

#include "NetClient.h"
#include <iostream>
#include <SDL.h>

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