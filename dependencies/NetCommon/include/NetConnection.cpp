#ifndef _NETCONNECTION_CPP_
#define _NETCONNECTION_CPP_
#include "NetConnection.h"

template<typename T>
NetConnection<T>::NetConnection(NetConnection::Owner a_owner, asio::ip::tcp::socket a_socket, asio::io_context& a_context, uint32_t id, MessageQueue<T>& messageIn, MessageQueue<T>& messageOut)
	: m_uid(id), m_socket(std::move(a_socket)), m_context(a_context), m_messageIn(messageIn), m_messageOut(messageOut), m_tempMsgBuf(), m_message()
{
	m_owner = a_owner;
	m_ack = false;
}

template<typename T>
NetConnection<T>::~NetConnection()
{
	if (m_socket.is_open())
	{
		Disconnect();
	}
}

template<typename T>
void NetConnection<T>::ConnectToServer(asio::ip::tcp::endpoint& endpoint)
{
}

template<typename T>
void NetConnection<T>::ConnectToClient()
{
}

template<typename T>
void NetConnection<T>::ReadMessage()
{

}

template<typename T>
void NetConnection<T>::ReadMessageHeader()
{
	// read the message header, which contains the size of the body and id for identification, asynchronously
	m_socket.async_receive(asio::buffer(&m_tempMsgBuf.m_header, sizeof(NetMessageHeader<T>)),
		[this](asio::error_code ec, size_t len)
		{
			if (!ec)
			{
				// if there is body in the message, resize the buffer size and ready to read body
				if (m_tempMsgBuf.m_header.m_size > 0)
				{
					m_tempMsgBuf.m_body.resize(m_tempMsgBuf.m_header.m_size);
					ReadMessageBody();
				}
				else
				{
					// otherwise, back to read next message header
					ReadMessageHeader();
				}
			}
			else
			{
				Disconnect();
			}
		});
}

template<typename T>
void NetConnection<T>::ReadMessageBody()
{
	// read the message body, which contains the main data, asynchronously
	m_socket.async_receive(asio::buffer(m_tempMsgBuf.m_body.data(), m_tempMsgBuf.m_header.m_size * sizeof(uint32_t)),
		[this](asio::error_code ec, size_t len)
		{
			if (!ec)
			{
				if (m_owner == Owner::Client && m_tempMsgBuf.m_header.m_flag == static_cast<T>(HandShake::ACK))
					m_uid = m_tempMsgBuf.m_body[0];

				// push the newly received message into incoming message queue
				m_messageIn.push_back(m_tempMsgBuf);

				// back to read next message header
				ReadMessageHeader();
			}
			else
			{
				Disconnect();
			}
		});
}

template<typename T>
void NetConnection<T>::WriteMessage()
{
	// wait until the current asyn read or write finish
	asio::post(m_context, [this]()
		{
			if (!m_messageOut.empty())
			{
				m_message = m_messageOut.pop_front();
				WriteMessageHeader();
			}
		}
	);
}

template<typename T>
void NetConnection<T>::WriteMessageHeader()
{
	// write the message header, which contains the size of the body and id for identification, asynchronously
	m_socket.async_send(asio::buffer(&m_message.m_header, sizeof(NetMessageHeader<T>)),
		[this](asio::error_code ec, size_t len)
		{
			// if no error occured during writing message header, ready for writing message body, otherwise, diconnect the connection
			if (!ec)
			{
				WriteMessageBody();
			}
			else
			{
				Disconnect();
			}
		}
	);
}

template<typename T>
void NetConnection<T>::WriteMessageBody()
{
	// write the message header, which contains the main data, asynchronously
	m_socket.async_send(asio::buffer(m_message.m_body.data(), m_message.m_header.m_size * sizeof(uint32_t)),
		[this](asio::error_code ec, size_t len)
		{
			// if no error occured during writing message body, back to write next message header, otherwise, diconnect the connection
			if (!ec)
			{
				WriteMessage();
			}
			else
			{
				Disconnect();
			}
		}
	);
}

template<typename T>
void NetConnection<T>::Disconnect()
{
	// wait the io_context the finish the current job first
	if (IsAlive())
	{
		asio::post(m_context, [this]() { m_socket.close(); });
		if (m_owner == Owner::Server)
			std::cout << "[SERVER] Client ID[" << m_uid << "] is disconnected." << std::endl;
		else if (m_owner == Owner::Client)
			std::cout << "Disconnected from the server." << std::endl;
	}
}

template<typename CustomMessage>
uint32_t NetConnection<CustomMessage>::Hash(uint32_t plaintext)
{
	return (11 * plaintext + 4) % 26;
}

template<typename T>
bool NetConnection<T>::IsAlive()
{
	return m_socket.is_open();
}

#endif // !_NETCONNECTION_CPP_