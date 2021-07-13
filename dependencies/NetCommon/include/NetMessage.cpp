#ifndef _NETMESSAGE_CPP_
#define _NETMESSAGE_CPP_
#include "NetMessage.h"

template <typename T>
NetMessageHeader<T>::NetMessageHeader()
{
	m_source_id = 0;
	m_flag = (T)0;
	m_size = 0;
}

template <typename T>
NetMessageHeader<T>::NetMessageHeader(uint32_t source_id, T flag, uint32_t size)
{
	m_source_id = source_id;
	m_flag = flag;
	m_size = size;
}

template <typename T>
NetMessage<T>::NetMessage() : m_header(), m_body({})
{

}

// default constructor
template <typename T>
NetMessage<T>::NetMessage(uint32_t source_id, T flag, std::vector<uint32_t> body): m_header(source_id, flag, body.size()), m_body(body)
{

}

template <typename T>
NetMessage<T>::~NetMessage()
{

}

template <typename T>
void NetMessage<T>::Print()
{
	std::cout << "source id: " << m_header.m_source_id << std::endl;
	std::cout << "flag: " << (uint32_t)m_header.m_flag << std::endl;
	std::cout << "body: ";
	for (auto it = m_body.begin(); it != m_body.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << std::endl;
}

template <typename T>
void NetMessage<T>::operator=(std::vector<uint32_t> body)
{
	m_body = body;
	m_header.m_size = body.size();
}

template <typename T>
void NetMessage<T>::operator=(NetMessage<T> msg)
{
	m_header.m_source_id = msg.m_header.m_source_id;
	m_header.m_size = msg.m_header.m_size;
	m_header.m_flag = msg.m_header.m_flag;
	m_body = msg.m_body;
}

template <typename T>
NetMessage<T>& NetMessage<T>::operator<<(uint32_t data)
{
	m_body.push_back(data);
	m_header.m_size = m_body.size();
	return *this;
}

template <typename T>
NetMessage<T>& NetMessage<T>::operator>>(uint32_t& data)
{
	assert(!m_body.empty());
	data = m_body.front();
	m_body.erase(m_body.begin());
	m_header.m_size = m_body.size();
	return *this;
}

#endif // !_NETMESSAGE_CPP_