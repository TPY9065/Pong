#pragma once

#include "NetConnection.h"
#include "MessageQueue.h"

template<typename CustomMessage>
class NetServer
{
public:
	NetServer();
	~NetServer();
	void Start();
	void WaitForConnection();
	void ReadMessageFromClient(uint32_t id);
	void MessageToClient(NetMessage<CustomMessage> msg, uint32_t id);
	void MessageToAllClient(NetMessage<CustomMessage> msg, uint32_t from);
	void Disconnect(uint32_t id);
	void Update();
public:
	// context for I/O services
	asio::io_context m_context;
	// acceptor for accepting connection
	asio::ip::tcp::acceptor m_acceptor;
	// thread for I/O
	std::thread m_thread;
	// container/manageer for keeping the newcoming connection alive
	std::unordered_map<uint32_t, std::shared_ptr<NetConnection<CustomMessage>>> m_connections;
	// socket for sending message to client
	asio::ip::tcp::socket m_socket;
	// incoming message queue
	MessageQueue<CustomMessage> m_messageIn;
	MessageQueue<CustomMessage> m_messageOut;
	// no of connections alived, also used for identification
	uint32_t m_uid = 10000;
};

#include "NetServer.cpp"