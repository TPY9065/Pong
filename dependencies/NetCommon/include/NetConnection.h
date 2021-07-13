#pragma once

#include "NetCommon.h"
#include "NetMessage.h"
#include "MessageQueue.h"

template<typename T>
class NetConnection
{
public:
	enum class Owner
	{
		Server,
		Client
	};
public:
	NetConnection(Owner a_owner, asio::ip::tcp::socket a_socket, asio::io_context& a_context, uint32_t id, MessageQueue<T>& messageIn, MessageQueue<T>& messageOut);
	~NetConnection();
	void ConnectToServer(asio::ip::tcp::endpoint& endpoint);
	void ConnectToClient();
	void ReadMessage();
	void ReadMessageHeader();
	void ReadMessageBody();
	void WriteMessage();
	void WriteMessageHeader();
	void WriteMessageBody();
	void Disconnect();
	uint32_t Hash(uint32_t plaintext);
	bool IsAlive();
public:
	// ack to check if the connection is established successfully
	bool m_ack = false;
	// id used for identification
	uint32_t m_uid;
	// owner of the connetion
	Owner m_owner;
	// socket for connection
	asio::ip::tcp::socket m_socket;
	// I/O context for asio
	asio::io_context& m_context;
	// buffer for message
	NetMessage<T> m_message;
	// temperory message buffer
	NetMessage<T> m_tempMsgBuf;
	// incoming/outgoing message queue
	MessageQueue<T>& m_messageIn;
	MessageQueue<T>& m_messageOut;
};

#include "NetConnection.cpp"