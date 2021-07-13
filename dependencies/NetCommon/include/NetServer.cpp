#ifndef _NETSERVER_CPP_
#define _NETSERVER_CPP_
#include "NetServer.h"

// initialize an acceptor and open a tcp socket at port
template<typename T>
NetServer<T>::NetServer() : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 60000)), m_socket(m_context), m_messageIn()
{
	m_uid = 10000;
}

template<typename T>
NetServer<T>::~NetServer()
{
	// disconnect to all client
	for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++)
		Disconnect(connection->first);
	// stop the context
	m_context.stop();
}

// start the server
template<typename T>
void NetServer<T>::Start()
{
	std::cout << "[SERVER] Started!\n";

	// start waiting connection
	WaitForConnection();

	// start thread for io
	m_thread = std::thread([this]() { m_context.run(); });
}

template<typename T>
void NetServer<T>::WaitForConnection()
{
	// creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	m_acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
				// keep the new connection alive
				std::shared_ptr<NetConnection<T>> newConnecion = std::make_shared<NetConnection<T>>(NetConnection<T>::Owner::Server, std::move(socket), m_context, m_uid, m_messageIn, m_messageOut);
				m_connections[m_uid] = std::move(newConnecion);

				// start reading message from clients
				ReadMessageFromClient(m_uid);

				// id for next connection + 1
				m_uid += 1;
			}
			else
				std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";

			// wait for another connection
			WaitForConnection();
		}
	);
}

template<typename T>
void NetServer<T>::ReadMessageFromClient(uint32_t id)
{
	// safe guard, prevent client disconnected instantly after connecting to server
	if (m_connections[id]->IsAlive())
		m_connections[id]->ReadMessageHeader();
	else
		Disconnect(id);
}

template<typename T>
void NetServer<T>::MessageToClient(NetMessage<T> msg, uint32_t id)
{
	if (m_connections[id]->IsAlive())
	{
		m_connections[id]->m_messageOut.push_back(msg);
		m_connections[id]->WriteMessage();
	}
	else
		Disconnect(id);
}

template<typename T>
void NetServer<T>::MessageToAllClient(NetMessage<T> msg, uint32_t from)
{
	// write message to all client, except the one who send message
	std::deque<uint32_t> disconnectID;
	for (auto connection = m_connections.begin(); connection != m_connections.end(); connection++)
		if (connection->second->IsAlive())
		{
			if (connection->first != from)
			{
				connection->second->m_messageOut.push_back(msg);
				connection->second->WriteMessage();
			}
		}
		else
		{
			disconnectID.push_back(connection->first);
		}

	for (auto id : disconnectID)
	{
		Disconnect(id);
	}
}

template<typename T>
void NetServer<T>::Disconnect(uint32_t id)
{
	// disconnect the connection
	m_connections[id]->Disconnect();
	// remove the pointer from the container/manager first, otherwise remove will not work after reset
	m_connections[id].reset();
	// erase the nullptr
	m_connections.erase(id);
}

template<typename T>
void NetServer<T>::Update()
{
	// check if there is any message sent from the client, if yes, pop out from the message queue and print it on screen
	if (!m_messageIn.empty())
	{
		m_messageIn.pop_front().Print();
	}
}

#endif // !_NETSERVER_CPP_
