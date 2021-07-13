#pragma once
#include "NetConnection.h"
#include "MessageQueue.h"

template<typename CustomMessage>
class NetClient
{
public:
	NetClient();
	~NetClient();
	void Disconnect();
	void ConnectToServer();
	void ReadMessage();
	void WriteMessage(NetMessage<CustomMessage> msg);
	void Update();
	virtual uint32_t Hash(uint32_t plaintext);
	bool IsConnected();
public:
	// context to write message by using asio
	asio::io_context m_context;
	// thread for io
	std::thread m_thread;
	// socket for connection
	asio::ip::tcp::socket m_socket;
	// id
	uint32_t m_uid;
	// connection
	std::unique_ptr<NetConnection<CustomMessage>> m_connection;
	// incoming message queue
	MessageQueue<CustomMessage> m_messageIn;
	// outgoing message queue
	MessageQueue<CustomMessage> m_messageOut;
};

#include "NetClient.cpp"