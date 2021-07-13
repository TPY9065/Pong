#pragma once

#include "NetCommon.h"

enum class HandShake
{
	SYN = -1,
	SYN_ACK = -2,
	ACK = -3
};

template<typename T>
class NetMessageHeader
{
public:
	NetMessageHeader();
	NetMessageHeader(uint32_t id, T type, uint32_t size);
public:
	// sender id
	uint32_t m_source_id;
	// message type
	T m_flag;
	// body size in byte
	uint32_t m_size;
};

template<typename T>
class NetMessage
{
public:
	NetMessage();
	NetMessage(uint32_t id, T type, std::vector<uint32_t> body);
	~NetMessage();
	void Print();
	// only change the body of the message
	void operator=(std::vector<uint32_t> body);
	// copy the whole message into *this
	void operator=(NetMessage<T> msg);
	NetMessage<T>& operator<<(uint32_t data);
	NetMessage<T>& operator>>(uint32_t& data);
public:
	// header for message
	NetMessageHeader<T> m_header;
	// data to be sent
	std::vector<uint32_t> m_body;
};

#include "NetMessage.cpp"