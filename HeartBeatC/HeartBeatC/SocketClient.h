/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketClient for C/S
*/
#pragma once
#include <process.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include "BaseFunc.h"
using namespace std;
#pragma comment(lib,"ws2_32.lib")  

class CSocketClient
{
public:
	CSocketClient(string ServerIP);
	~CSocketClient();
private:
	SOCKET m_sockClient;
	SOCKADDR_IN m_clientSrv;
	queue<string> MessageList;
	string m_ServerIP;
	bool ServerisAlive;
	bool ServerisAliveBefore;
private:
	static unsigned int WINAPI TcpRevThread(void* lpParam);
	static unsigned int WINAPI TimerThread(void* lpParam);

	bool InitSocketClient();

public:
	int SendMessage(const char* message, size_t lengt);
	size_t MessageListSize(void){ return MessageList.size(); }
	string GetMessage(void)
	{
		string message = MessageList.front();
		MessageList.pop();
		return message;
	}
};

