#include "PongServer.h"

void PongServer::Start()
{
	std::cout << "[Server] Started!\n";

	std::cout << "Waiting for connection..." << std::endl;

	WaitForConnection();

	// start thread for io
	m_thread = std::thread([this]() { m_context.run(); });
}

void PongServer::WaitForConnection()
{
	// creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	m_acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec && m_player < World::MAX_PLAYER)
			{
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
				// keep the new connection alive
				std::shared_ptr<NetConnection<Protocal>> newConnecion = std::make_shared<NetConnection<Protocal>>(NetConnection<Protocal>::Owner::Server, std::move(socket), m_context, m_uid, m_messageIn, m_messageOut);
				m_connections[m_uid] = std::move(newConnecion);

				// start reading message from clients
				ReadMessageFromClient(m_uid);

				// assign id to client
				NetMessage<Protocal> ack(0, Protocal::ACK, { m_uid });
				MessageToClient(ack, m_uid);

				// messag to other player that a new player join
				NetMessage<Protocal> player_join(0, Protocal::PLAYER_CONNECT, { m_uid });
				MessageToAllClient(player_join, m_uid);

				// id for next connection
				while (m_connections.find(m_uid) != m_connections.end())
					m_uid += 1;
				m_player += 1;
			}
			else
				std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";

			// wait for another connection
			WaitForConnection();
		}
	);
}

void PongServer::Update()
{
	if (!m_messageIn.empty())
	{
		NetMessage<Protocal> msg = m_messageIn.pop_front();
		switch (msg.m_header.m_flag)
		{
		case Protocal::ACK:
		{
			std::cout << "ACK" << std::endl;
			break;
		}
		case Protocal::PLAYER_UPDATE:
		{
			MessageToAllClient(msg, msg.m_header.m_source_id);
			break;
		}
		case Protocal::BALL_UPDATE:
		{
			MessageToAllClient(msg, msg.m_header.m_source_id);
			break;
		}
		case Protocal::PLAYER_CONNECT:
		{
			MessageToAllClient(msg, msg.m_header.m_source_id);
			break;
		}
		case Protocal::PLAYER_LOSE:
			MessageToAllClient(msg, msg.m_header.m_source_id);
			break;
		case Protocal::PLAYER_DISCONNECT:
		{
			Disconnect(msg.m_header.m_source_id);
			MessageToAllClient(msg, msg.m_header.m_source_id);
			m_player -= 1;
			if (msg.m_header.m_source_id < m_uid)
				m_uid = msg.m_header.m_source_id;
			break;
		}
		default:
			break;
		}
	}

}

void PongServer::Run()
{
	Start();
	while (true)
		Update();
}