#pragma once

#include "NetCommon.h"
#include "NetMessage.h"

template<typename T>
class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();
	void push_back(NetMessage<T> msg);
	void push_front(NetMessage<T> msg);
	NetMessage<T> pop_back();
	NetMessage<T> pop_front();
	NetMessage<T>& back();
	NetMessage<T>& front();
	bool empty();
	uint32_t size();
public:
	// message to that socket
	std::deque<NetMessage<T>> m_messageQueue;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};

#include "MessageQueue.cpp"