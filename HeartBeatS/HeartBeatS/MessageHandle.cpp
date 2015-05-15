/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#include "MessageHandle.h"


CMessageHandle::CMessageHandle(CSocketServer *nSocket):
	m_nSocket(nSocket)
{
}


CMessageHandle::~CMessageHandle()
{
}

void CMessageHandle::MessageHandleFunc(struct CMessagePached &message)
{
	size_t position = message.nMessage.find("?");
	if (position == message.nMessage.npos || position <= 6)
	{
		string str = "The message's format is error!";
		m_nSocket->SendMessage(str.c_str(), str.length(), message.nClientName);
		return;
	}
	string SType = message.nMessage.substr(6, position - 6);
	if (is_digit(SType.c_str()) != 0)
	{
		string str = "The message's type is not digit!";
		m_nSocket->SendMessage(str.c_str(), str.length(), message.nClientName);
		return;
	}
	int MType = atoi(SType.c_str());
	switch (MType)
	{
	case GetClientList:
		m_nSocket->SendClientList(message.nClientName);
		break;
	case GetClientListSize:
		m_nSocket->SendClientListSize(message.nClientName);
		break;
	default:
		break;
	}
}

int CMessageHandle::is_digit(const char *str)
{
	size_t len = strlen(str);
	while (len > 0) {
		if (*str < '0' || *str > '9') {
			return -1;
		}
		str++;
		len--;
	}
	return 0;
}
