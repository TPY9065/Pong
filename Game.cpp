#include "Game.h"

Game::Game() : m_ball(World::WIDTH / 2, World::HEIGHT / 2, World::WIDTH / 50), 
m_bKeyDown({ {'U', false}, {'D', false}, {'L', false}, {'R', false} }),
m_bPlayerExist({ {Player::P1, false}, {Player::P2, false}, {Player::P3, false}, {Player::P4, false}, })
{	
	// init SDL
	Init();
	// init player
	m_running = true;
	m_bKeyPressed = false;
	m_state = GameState::IDLE;
	m_winner = Player::EMPTY;
	m_bLose = false;
	m_place = 0;
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
	while (SDL_PollEvent(&m_userInput) != 0 && !m_bKeyPressed)
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
					m_bKeyDown['L'] = true;
				}
				break;
			}
			case SDLK_RIGHT:
			{
				if (GetPlayer() == Player::P3 || GetPlayer() == Player::P4)
				{
					m_bKeyDown['R'] = true;
				}
				break;
			}
			case SDLK_UP:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2)
				{
					m_bKeyDown['U'] = true;
				}
				break;
			}
			case SDLK_DOWN:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2)
				{
					m_bKeyDown['D'] = true;
				}
				break;
			}
			case SDLK_KP_ENTER:
				if (GetPlayer() == Player::P1)
				{
					if (m_ball.Init())
					{
						NetMessage<Protocal> ball_data(GetID(), Protocal::BALL_UPDATE, { static_cast<uint32_t>(m_ball.GetVX()), static_cast<uint32_t>(m_ball.GetVY()) });
						MessageToServer(ball_data);
					}
				}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (m_userInput.key.keysym.sym)
			{
			case SDLK_LEFT:
			{
				if (GetPlayer() == Player::P3 || GetPlayer() == Player::P4 && m_bKeyDown['L'])
				{
					m_bKeyDown['L'] = false;
				}
				break;
			}
			case SDLK_RIGHT:
			{
				if (GetPlayer() == Player::P3 || GetPlayer() == Player::P4 && m_bKeyDown['R'])
				{
					m_bKeyDown['R'] = false;
				}
				break;
			}
			case SDLK_UP:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2 && m_bKeyDown['U'])
				{
					m_bKeyDown['U'] = false;
				}
				break;
			}
			case SDLK_DOWN:
			{
				if (GetPlayer() == Player::P1 || GetPlayer() == Player::P2 && m_bKeyDown['D'])
				{
					m_bKeyDown['D'] = false;
				}
				break;
			}
			}
			break;
		}
		default:
			break;
		}
	}

	if (m_bKeyDown['U'] || m_bKeyDown['D'] || m_bKeyDown['L'] || m_bKeyDown['R'] && !m_bLose && m_winner == Player::EMPTY)
	{
		if (m_bKeyDown['U'])
			Move(0, -10);
		else if (m_bKeyDown['D'])
			Move(0, 10);
		else if (m_bKeyDown['L'])
			Move(-10, 0);
		else if (m_bKeyDown['R'])
			Move(10 , 0);
		NetMessage<Protocal> pos(GetID(), Protocal::PLAYER_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
		MessageToServer(pos);
	}
}

void Game::DrawBall()
{
	// start from (0, r)
	int x = 0;
	int y = m_ball.GetRadius();
	int d = 3 - 2 * m_ball.GetRadius();

	// draw the starting points
	SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + x, m_ball.GetY() - y);
	SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + y, m_ball.GetY() - x);
	SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + x, m_ball.GetY() + y);
	SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() - y, m_ball.GetY() + x);

	while (x <= y)
	{
		// the next x-pos will at least 1 pixel larger than the previous point
		x += 1;
		// if the point is inside circle
		if (d < 0)
		{
			d = d + 4 * x + 6;
		}
		else
		{
			d = d + 4 * (x - y) + 10;
			y -= 1;
		}

		// draw the corresponding point in each octant
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + x, m_ball.GetY() - y);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + y, m_ball.GetY() - x);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + y, m_ball.GetY() + x);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() + x, m_ball.GetY() + y);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() - x, m_ball.GetY() + y);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() - y, m_ball.GetY() + x);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() - y, m_ball.GetY() - x);
		SDL_RenderDrawPoint(m_Renderer, m_ball.GetX() - x, m_ball.GetY() - y);
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

	// draw ball
	DrawBall();

	// update on screen
	SDL_RenderPresent(m_Renderer);
}

void Game::MoveBall()
{
	bool bounce = false;
	// move the ball
	m_ball.Move();
	// if it hits wall or player, set its position back to the boundary
	if (m_ball.HitWall())
	{
		Vector2D arrivedPos = m_ball.GetPos() + m_ball.GetVec();
		// check which wall the ball hit
		if (arrivedPos.m_x - m_ball.GetRadius() < 0)
		{
			if (m_bPlayerExist[Player::P1])
			{
				if (GetPlayer() == Player::P1)
				{
					LosePoint();
				}
				m_ball.Reset();
			}
			else if (!m_bPlayerExist[Player::P1])
			{
				m_ball.SetPos(0 + m_ball.GetRadius(), m_ball.GetY());
				m_ball.SetVec(-m_ball.GetVX(), m_ball.GetVY());
			}
		}
		else if (arrivedPos.m_x + m_ball.GetRadius() > World::WIDTH)
		{
			if (m_bPlayerExist[Player::P2])
			{
				if (GetPlayer() == Player::P2)
					LosePoint();
				m_ball.Reset();
			}
			else if (!m_bPlayerExist[Player::P2])
			{
				m_ball.SetPos(World::WIDTH - m_ball.GetRadius(), m_ball.GetY());
				m_ball.SetVec(-m_ball.GetVX(), m_ball.GetVY());
			}
		}
		else if (arrivedPos.m_y - m_ball.GetRadius() < 0)
		{
			if (m_bPlayerExist[Player::P3])
			{
				if (GetPlayer() == Player::P3)
					LosePoint();
				m_ball.Reset();
			}
			else if (!m_bPlayerExist[Player::P3])
			{
				m_ball.SetPos(m_ball.GetX(), 0 + m_ball.GetRadius());
				m_ball.SetVec(m_ball.GetVX(), -m_ball.GetVY());
			}
		}
		else if (arrivedPos.m_y + m_ball.GetRadius() > World::HEIGHT)
		{
			if (m_bPlayerExist[Player::P4])
			{
				if (GetPlayer() != Player::P4)
					LosePoint();
				m_ball.Reset();
			}
			else if (!m_bPlayerExist[Player::P4])
			{
				m_ball.SetPos(m_ball.GetX(), World::HEIGHT - m_ball.GetRadius());
				m_ball.SetVec(m_ball.GetVX(), -m_ball.GetVY());
			}
		}
		return;
	}
	
	Player playerHit = Player::EMPTY;
	Vector2D arrivedPos = m_ball.GetPos() + m_ball.GetVec();
	int xDiff = 0;
	int yDiff = 0;
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	// if it hit self, else loop through all opponents
	if (m_ball.HitPlayer(GetX(), GetY(), GetWidth(), GetHeight()))
	{
		playerHit = GetPlayer();
		xDiff = arrivedPos.m_x - (GetX() + GetWidth() / 2);
		yDiff = arrivedPos.m_y - (GetY() + GetHeight() / 2);
		x = GetX();
		y = GetY();
		w = GetWidth();
		h = GetHeight();
		bounce = true;
	}
	else
	{
		for (auto opponent = m_opponents.begin(); opponent != m_opponents.end(); opponent++)
			if (m_ball.HitPlayer(opponent->second.GetX(), opponent->second.GetY(), opponent->second.GetWidth(), opponent->second.GetHeight()))
			{
				playerHit = opponent->second.GetPlayer();
				xDiff = arrivedPos.m_x - (opponent->second.GetX() + opponent->second.GetWidth() / 2);
				yDiff = arrivedPos.m_y - (opponent->second.GetY() + opponent->second.GetHeight() / 2);
				x = opponent->second.GetX();
				y = opponent->second.GetY();
				w = opponent->second.GetWidth();
				h = opponent->second.GetHeight();
				bounce = true;
				break;
			}
	}

	switch (playerHit)
	{
	case Player::P1:
	case Player::P2:
	{
		if (xDiff >= 0)
		{
			m_ball.SetPos(x + w + m_ball.GetRadius(), m_ball.GetY());
		}
		else if (xDiff < 0)
		{
			m_ball.SetPos(x - m_ball.GetRadius(), m_ball.GetY());
		}
		break;
	}
	case Player::P3:
	case Player::P4:
	{
		if (yDiff >= 0)
		{
			m_ball.SetPos(m_ball.GetX(), y + h + m_ball.GetRadius());
		}
		else
		{
			m_ball.SetPos(m_ball.GetX(), y - m_ball.GetRadius());
		}
		break;
	}
	case Player::EMPTY:
		return;
	default:
		break;
	}

	if (bounce)
		m_ball.Bounce(playerHit);
}

void Game::Update()
{
	while (!m_messageIn.empty())
	{
		NetMessage<Protocal> msg = m_messageIn.pop_front();
		switch (msg.m_header.m_flag)
		{
		case Protocal::ACK:
		{
			SetSelfID(msg.m_body[0]);
			break;
		}
		case Protocal::PLAYER_UPDATE:
		{
			PlayerUpdate(msg.m_header.m_source_id, msg.m_body[0], msg.m_body[1], msg.m_body[2], msg.m_body[3]);
			break;
		}
		case Protocal::BALL_UPDATE:
			m_ball.SetVec(msg.m_body[0], msg.m_body[1]);
			break;
		case Protocal::PLAYER_LOSE:
		{
			SetPlayerLose(msg.m_header.m_source_id, static_cast<Player>(msg.m_body[0]));
			break;
		}
		case Protocal::PLAYER_CONNECT:
		{
			AddPlayer(msg.m_body[0]);
			break;
		}
		case Protocal::PLAYER_DISCONNECT:
		{
			RemovePlayer(msg.m_header.m_source_id);
			break;
		}
		case Protocal::RESTART:
		{
			Reset();
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
		switch (m_state)
		{
		case GameState::IDLE:
		case GameState::PLAY:
		{
			// get user input
			GetInput();
			// get data from server
			Update();
			// move the ball
			MoveBall();
			// check if game finished
			GameFinished();
			// draw player and opponents
			Draw();
			break;
		}
		case GameState::FINISH:
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

				case SDL_KEYDOWN:
				{
					switch (m_userInput.key.keysym.sym)
					{
					case SDLK_KP_ENTER:
					{
						Reset();
						break;
					}
					default:
						break;
					}
				default:
					break;
				}
				}
				break;
			}
		default:
			break;
		}
		}
	}
}

void Game::SetSelfID(int id)
{
	if (!m_bLose)
	{
		SetID(id);
		SetXYWH();
		std::cout << "Your ID: " << GetID() << std::endl;
		m_bPlayerExist[GetPlayer()] = true;
		NetMessage<Protocal> self_data(GetID(), Protocal::PLAYER_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
		MessageToServer(self_data);
		m_place = m_opponents.size() + 1;
	}
}

void Game::AddPlayer(int id)
{
	// create an opponent object if no exist
	if (m_opponents.find(id) == m_opponents.end() && !m_bLose && m_winner == Player::EMPTY)
	{
		std::cout << "Player ID: " << id << " is connected" << std::endl;
		m_opponents.emplace(id, Opponent(id));
		m_bPlayerExist[m_opponents[id].GetPlayer()] = true;
		// send back self data
		NetMessage<Protocal> self_data(GetID(), Protocal::PLAYER_UPDATE, { GetUX(), GetUY(), GetUWidth(), GetUHeight() });
		MessageToServer(self_data);
		m_place = m_opponents.size() + 1;
	}
}

void Game::RemovePlayer(int id)
{
	std::cout << "Player ID[" << id << "] is disconnected" << std::endl;
	// remove the opponent
	m_bPlayerExist[m_opponents[id].GetPlayer()] = false;
	if (m_opponents.find(id) != m_opponents.end())
	{
		m_opponents.erase(id);
		m_place = m_opponents.size() + 1;
	}
}

void Game::PlayerUpdate(int id, int x, int y, int w, int h)
{
	// update opponents position
	if (m_opponents.find(id) == m_opponents.end() && !m_bLose && m_winner == Player::EMPTY)
	{
		std::cout << "Player ID: " << id << " is in the room" << std::endl;
		m_opponents.emplace(id, Opponent(id));
		m_bPlayerExist[m_opponents[id].GetPlayer()] = true;
		m_place = m_opponents.size() + 1;
	}
	m_opponents[id].SetXYWH(x, y, w, h);
}

void Game::SetPlayerLose(int id, Player p)
{
	std::cout << "Player " << static_cast<uint32_t>(p) + 1 << " lose." << std::endl;
	m_bPlayerExist[p] = false;
	m_opponents[id].SetXYWH(0, 0, 0, 0);
	m_place -= 1;

	if (m_place == 1)
	{
		std::cout << "You Win!" << std::endl
			<< "Your score is " << GetScore() << std::endl
			<< "You are place " << m_place << std::endl;
		m_state = GameState::FINISH;
		m_winner = GetPlayer();
		m_ball.Reset();
	}
}

void Game::Reset()
{
	SetID(GetID());
	SetPlayer(GetPlayer());
	SetXYWH();
	SetScore(1);
	m_ball.Reset();
	for (auto opponent = m_opponents.begin(); opponent != m_opponents.end(); opponent++)
	{
		opponent->second.Reset();
		m_bPlayerExist[opponent->second.GetPlayer()] = true;
	}

	NetMessage<Protocal> restart(GetID(), Protocal::RESTART, { GetID() });
	MessageToServer(restart);
	m_state = GameState::PLAY;
}

bool Game::IsInit()
{
	return (m_Window && m_Renderer);
}

bool Game::GameFinished()
{
	if (GetScore() == 0 && !m_bLose) {
		std::cout << "You Lose!" << std::endl
				  << "Your score is " << GetScore() << std::endl
			      << "You are place " << m_place << std::endl;
		NetMessage<Protocal> loose(GetID(), Protocal::PLAYER_LOSE, { static_cast<uint32_t>(GetPlayer()) });
		MessageToServer(loose);
		m_bPlayerExist[GetPlayer()] = false;
		SetPlayer(Player::EMPTY);
		SetXYWH();
		m_bLose = true;
		return true;
	}
	return false;
}