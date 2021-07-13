#include "Game.h"

Game::Game()
{	
	// init SDL
	Init();
	// init player
	m_running = true;
}

Game::~Game()
{
	// free the associated memory
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	m_Window = NULL;
	m_Renderer = NULL;
	// shut down SDL
	SDL_Quit();
}

void Game::Init()
{
	// SDL init
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Init SDL failed! Error: " << SDL_GetError() << std::endl;
		return;
	}
	InitWindow();
	InitRenderer();
}

void Game::InitWindow()
{
	m_Window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, World::WIDTH, World::HEIGHT, SDL_WINDOW_RESIZABLE);
	if (m_Window == NULL)
		std::cout << "Init window failed! Error: " << SDL_GetError() << std::endl;
}

void Game::InitRenderer()
{
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (m_Renderer == NULL)
	{
		std::cout << "Init renderer failed! Error: " << SDL_GetError() << std::endl;
		return;
	}
}

void Game::GetInput()
{
	while (SDL_PollEvent(&m_userInput) != 0)
	{
		switch (m_userInput.type)
		{
		// quit
		case SDL_QUIT:
		{
			NetMessage<Protocal> msg(GetID(), Protocal::PLAYER_DISCONNECT, { GetID() });
			MessageToServer(msg);
			m_running = false;
			break;
		}
		// arrow key
		case SDL_KEYDOWN:
		{
			switch (m_userInput.key.keysym.sym)
			{
			case SDLK_LEFT:
			{
				if (GetPlayer() == Player::P3 || GetPlayer() == Player::P4)
				{
					Move(-10, 0);
					NetMessage<Protocal> pos(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
					MessageToServer(pos);
				}
				break;
			}
			case SDLK_RIGHT:
			{
				if (GetPlayer() == Player::P3 || GetPlayer() == Player::P4)
				{
					Move(10, 0);
					NetMessage<Protocal> pos(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
					MessageToServer(pos);
				}
				break;
			}
			case SDLK_UP:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2)
				{
					Move(0, -10);
					NetMessage<Protocal> pos(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
					MessageToServer(pos);
				}
				break;
			}
			case SDLK_DOWN:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2)
				{
					Move(0, 10);
					NetMessage<Protocal> pos(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
					MessageToServer(pos);
				}
				break;
			}
			case SDLK_KP_ENTER:
				std::cout << "ID: " << GetID() << std::endl;
			}
		}
		default:
			break;
		}
	}
}

void Game::Draw()
{
	// clear renderer
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_Renderer);

	// set player and opponents color
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);

	// draw player
	SDL_Rect playerRect{ GetX(), GetY(), GetWidth(), GetHeight() };
	SDL_RenderDrawRect(m_Renderer, &playerRect);
	SDL_RenderFillRect(m_Renderer, &playerRect);

	// draw opponents if exist
	for (auto opponent = m_opponents.begin(); opponent != m_opponents.end(); opponent++)
	{
		SDL_Rect opponentRect{ opponent->second.GetX(), opponent->second.GetY(), opponent->second.GetWidth(), opponent->second.GetHeight() };
		SDL_RenderDrawRect(m_Renderer, &opponentRect);
		SDL_RenderFillRect(m_Renderer, &opponentRect);
	}

	// update on screen
	SDL_RenderPresent(m_Renderer);
}

void Game::Update()
{
	if (!m_messageIn.empty())
	{
		NetMessage<Protocal> msg = m_messageIn.pop_front();
		switch (msg.m_header.m_flag)
		{
		case Protocal::ACK:
		{
			// std::cout << "Ack message: " << std::endl;
			SetID(msg.m_body[0]);
			SetXYWH();
			// std::cout << "ID: " << GetID() << std::endl;
			NetMessage<Protocal> self_data(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
			WriteMessage(self_data);
			break;
		}
		case Protocal::POS_UPDATE:
		{
			// update opponents position
			if (m_opponents.find(msg.m_header.m_source_id) == m_opponents.end())
			{
				m_opponents.emplace(msg.m_header.m_source_id, Opponent(msg.m_header.m_source_id));
			}
			// std::cout << "Player ID [" << msg.m_header.m_source_id << "] pos update" << std::endl;
			m_opponents[msg.m_header.m_source_id].SetXYWH(msg.m_body[0], msg.m_body[1], msg.m_body[2], msg.m_body[3]);
			break;
		}
		case Protocal::BALL_POS:
			std::cout << "Ball pos" << std::endl;
			break;
		case Protocal::PLAYER_CONNECT:
		{
			// create an opponent object if no exist
			if (m_opponents.find(msg.m_body[0]) == m_opponents.end())
			{
				// std::cout << "Player ID[" << msg.m_body[0] << "] connect" << std::endl;
				m_opponents.emplace(msg.m_header.m_source_id, Opponent(msg.m_body[0]));
				// send back self data
				NetMessage<Protocal> self_data(GetID(), Protocal::POS_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
				MessageToServer(self_data);
			}
			break;
		}
		case Protocal::PLAYER_DISCONNECT:
		{
			std::cout << "Player ID[" << msg.m_body[0] << "] is disconnected" << std::endl;
			// remove the opponent
			m_opponents.erase(msg.m_header.m_source_id);
			break;
		}
		default:
			break;
		}
	}
}

void Game::Run()
{
	while (m_running)
	{
		// get user input
		GetInput();
		// get data from server
		Update();
		// draw player and opponents
		Draw();
	}
}

void Game::AddPlayer()
{

}

bool Game::IsInit()
{
	return (m_Window && m_Renderer) ? true : false;
}