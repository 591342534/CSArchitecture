/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#pragma once
#include "SocketServer.h"
#include "MessageType.h"

class CMessageHandle
{
public:
	CMessageHandle(CSocketServer *nSocket);
	~CMessageHandle();
private:
	CSocketServer *m_nSocket;
private:
	int is_digit(const char *str);
public:
	void MessageHandleFunc(struct CMessagePached &message);
};

