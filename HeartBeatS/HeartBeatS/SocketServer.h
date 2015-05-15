/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#pragma once
#include <process.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <queue>
#include "BaseFunc.h"
using namespace std;
#pragma comment(lib,"ws2_32.lib")  

typedef struct CParam
{
	long ClientName;
	void *pThis;
}ParamData;

typedef struct ClientSocket
{
	SOCKET m_client;
	SOCKADDR_IN m_clientaddr;
}pClientSocket;

typedef struct CMessagePached
{
	long nClientName;
	string nMessage;
}pCMessagePached;

class CSocketServer
{
public:
	CSocketServer();
	~CSocketServer();
private:
	SOCKET m_ser;
	map<long, struct ClientSocket> m_ClientList;
	queue<struct CMessagePached> MessageList;
private:
	static unsigned int WINAPI AcceptThread(void* lpParam);
	static unsigned int WINAPI TcpRevThread(void* lpParam);
	bool InitSocketServer();
public:
	size_t MessageListSize(void){ return MessageList.size(); }
	struct CMessagePached GetMessage(void)
	{ 
		struct CMessagePached message = MessageList.front();
		MessageList.pop(); 
		return message; 
	}
	int SendMessage(const char* message, size_t length, long &ClientName);
	void SendClientList(long &ClientName);
	void SendClientListSize(long &ClientName);
};

