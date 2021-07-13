#ifndef _MESSAGEQUEUE_CPP_
#define _MESSAGEQUEUE_CPP_
#include "MessageQueue.h"

template<typename T>
MessageQueue<T>::MessageQueue() :m_messageQueue({})
{
}

template<typename T>
MessageQueue<T>::~MessageQueue()
{
}

template<typename T>
void MessageQueue<T>::push_back(NetMessage<T> msg)
{
	m_mutex.lock();
	m_messageQueue.push_back(msg);
	m_mutex.unlock();
}

template<typename T>
void MessageQueue<T>::push_front(NetMessage<T> msg)
{
	m_mutex.lock();
	m_messageQueue.push_front(msg);
	m_mutex.unlock();
}

template<typename T>
NetMessage<T> MessageQueue<T>::pop_back()
{
	m_mutex.lock();
	NetMessage<T> msg;
	if (!m_messageQueue.empty())
	{
		msg = m_messageQueue.back();
		m_messageQueue.pop_back();
	}
	m_mutex.unlock();
	return msg;
}

template<typename T>
NetMessage<T> MessageQueue<T>::pop_front()
{
	m_mutex.lock();
	NetMessage<T> msg;
	if (!m_messageQueue.empty())
	{
		msg = m_messageQueue.front();
		m_messageQueue.pop_front();
	}
	m_mutex.unlock();
	return msg;
}

template<typename T>
NetMessage<T>& MessageQueue<T>::back()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_messageQueue.back();
}

template<typename T>
NetMessage<T>& MessageQueue<T>::front()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_messageQueue.front();
}

template<typename T>
bool MessageQueue<T>::empty()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_messageQueue.empty();
}

template<typename T>
uint32_t MessageQueue<T>::size()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_messageQueue.size();
}

#endif