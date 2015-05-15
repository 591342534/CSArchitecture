/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketClient for C/S
*/
#include "SocketClient.h"


CSocketClient::CSocketClient(string ServerIP):
	m_ServerIP(ServerIP)
{
	if (InitSocketClient())
	{
		ServerisAlive = false;
		ServerisAliveBefore = ServerisAlive;
		_beginthreadex(NULL, 0, TcpRevThread, (void*)this, 0, 0);
		_beginthreadex(NULL, 0, TimerThread, (void*)this, 0, 0);
	}
	else
	{
		SetTextRed();
		cout << "InitSocketClient Failed!" << endl;
	}
}


CSocketClient::~CSocketClient()
{
	closesocket(m_sockClient);
	WSACleanup();
}

bool CSocketClient::InitSocketClient()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return false;
	}


	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);
	char *pIP = (char*)malloc(sizeof(char)* 10);
	char *strHostName = (char*)malloc(sizeof(char)* 10);
	DWORD dwIP = 0;
	strcpy_s(strHostName, sizeof(m_ServerIP), m_ServerIP.c_str());
	HOSTENT *pHS = gethostbyname(strHostName);
	if (pHS == NULL)
	{
		return false;
	}
	pIP = inet_ntoa(*((in_addr*)pHS->h_addr_list[0]));
	m_clientSrv.sin_addr.S_un.S_addr = inet_addr(pIP);
	m_clientSrv.sin_family = AF_INET;
	m_clientSrv.sin_port = htons(12345);
	return true;
}

unsigned int WINAPI CSocketClient::TcpRevThread(void* lpParam)
{
	CSocketClient* p = (CSocketClient*)lpParam;
	while (1)
	{
		if (p->ServerisAlive != p->ServerisAliveBefore && p->ServerisAlive)
		{
			SetTextGreen();
			cout << "服务器已连接" << ends << ToGetLocalTime() << endl;
		}
		p->ServerisAliveBefore = p->ServerisAlive;
		if (connect(p->m_sockClient, (SOCKADDR*)&p->m_clientSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			int i = ::WSAGetLastError();
			if (i == 10056)//已连接
			{
				char recvBuf[100] = { 0 };
				if (recv(p->m_sockClient, recvBuf, 100, 0) == SOCKET_ERROR)
				{
					p->ServerisAlive = false;
					SetTextRed();
					cout << "服务器已断开，" << "Socket错误码：" << WSAGetLastError() << ends << ToGetLocalTime() << endl;
					Sleep(3000);
					closesocket(p->m_sockClient);
					p->m_sockClient = socket(AF_INET, SOCK_STREAM, 0);
					continue;
				}
				string message = (string)recvBuf;
				if (message != "收到")
				{
					p->MessageList.push(message);
					SetTextWhite();
					cout << "应答信息：" << message << ends << ToGetLocalTime() << endl;
				}
				p->ServerisAlive = true;
				continue;
			}
			p->ServerisAlive = false;
			SetTextRed();
			cout << "服务器连接失败，" << "Socket错误码：" << i << ends << ToGetLocalTime() << endl;
			Sleep(3000);
			closesocket(p->m_sockClient);
			p->m_sockClient = socket(AF_INET, SOCK_STREAM, 0);
			continue;
		}
		else
		{
			p->ServerisAlive = true;
		}
	}
	return 0;
}

unsigned int WINAPI CSocketClient::TimerThread(void* lpParam)
{
	CSocketClient* p = (CSocketClient*)lpParam;
	while (1)
	{
		DWORD lngPassTime = 0;
		DWORD lngStartTime = GetTickCount();
		while (lngPassTime <= 2000)
		{
			lngPassTime = GetTickCount() - lngStartTime;
		}
		send(p->m_sockClient, "心跳测试", strlen("心跳测试") + 1, 0);
	}
}

int CSocketClient::SendMessage(const char* message, size_t length)
{
	send(m_sockClient, message, length, 0);
	return 0;
}
