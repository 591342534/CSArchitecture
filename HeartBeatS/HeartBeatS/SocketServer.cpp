/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#include "SocketServer.h"


CSocketServer::CSocketServer()
{
	if (InitSocketServer())
	{
		_beginthreadex(NULL, 0, AcceptThread, (void*)this, 0, 0);
	}
	else
	{
		SetTextRed();
		cout << "InitSocketServer Failed!" << endl;
	}
}


CSocketServer::~CSocketServer()
{

	closesocket(m_ser);
	WSACleanup();
}

bool CSocketServer::InitSocketServer()
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
		HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return false;
	}

	//TCP套接字初始化
	m_ser = socket(AF_INET, SOCK_STREAM, 0);
	int TimeOut = 5000;//设置接收超时6秒 
	if (::setsockopt(m_ser, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut)) == SOCKET_ERROR)
	{
		return false;
	}

	SOCKADDR_IN seraddr;
	seraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(12345);
	if (bind(m_ser, (SOCKADDR*)&seraddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		closesocket(m_ser);
		return false;
	}

	if (listen(m_ser, 100) == SOCKET_ERROR)
	{
		closesocket(m_ser);
		return false;
	}
	return true;
}

unsigned int WINAPI CSocketServer::AcceptThread(void* lpParam)
{
	CSocketServer* p = (CSocketServer*)lpParam;
	int len = sizeof(SOCKADDR);
	long count = 0;
	while (1)
	{	
		struct ClientSocket pClient;
		pClient.m_client = accept(p->m_ser, (SOCKADDR*)&pClient.m_clientaddr, &len);
		string clientIP = inet_ntoa(pClient.m_clientaddr.sin_addr);
		//cout << clientIP.c_str() << endl;
		long tick = GetTickCount() + count++;
		SetTextGreen();
		cout << "客户端" << ends << tick << ":" << clientIP << ends << "已连接" << ends << ToGetLocalTime() << endl;
		p->m_ClientList.insert(pair<long, struct ClientSocket>(tick, pClient));
		struct CParam temp;
		temp.ClientName = tick;
		temp.pThis = lpParam;
		_beginthreadex(NULL, 0, TcpRevThread, (void*)&temp, 0, 0);
	}
	return 0;
}

unsigned int WINAPI CSocketServer::TcpRevThread(void* lpParam)
{
	struct CParam temp = *(struct CParam*)lpParam; //不能定义为指针，因为需要每个子线程都创建一个临时变量，指针公用了
	CSocketServer* p = (CSocketServer*)temp.pThis;
	DWORD lngPassTime = 0;
	string ClientIP = inet_ntoa(p->m_ClientList[temp.ClientName].m_clientaddr.sin_addr);
	while (lngPassTime <= 2500)
	{
		//cout << GetCurrentThreadId() << endl;
		DWORD lngStartTime = GetTickCount();
		char buff[1024] = { 0 };
		if (recv(p->m_ClientList[temp.ClientName].m_client, buff, 1024, 0) == SOCKET_ERROR)
		{
			SetTextRed();
			cout << "客户端" << ends  << temp.ClientName << ":" << ClientIP << ends << "已断开，" << "Socket错误码：" << WSAGetLastError() << ends << ToGetLocalTime() << endl;
			break;
		}
		else
		{
			//cout << buff << endl;
			string message = (string)buff;
			if (message != "心跳测试")
			{
				struct CMessagePached mPachedtemp;
				mPachedtemp.nClientName = temp.ClientName;
				mPachedtemp.nMessage = message;
				p->MessageList.push(mPachedtemp);
				SetTextGreen();
				cout << "消息：" << message << " 来自 " << temp.ClientName << ":" << ClientIP << ends << ToGetLocalTime() << endl;
			}
		}
		lngPassTime = GetTickCount() - lngStartTime;
		send(p->m_ClientList[temp.ClientName].m_client, "收到", strlen("收到") + 1, 0);
	}
	closesocket(p->m_ClientList[temp.ClientName].m_client);
	p->m_ClientList.erase(temp.ClientName);
	return 0;
}

int CSocketServer::SendMessage(const char* message, size_t length, long &ClientName)
{
	send(m_ClientList[ClientName].m_client, message, length, 0);
	return 0;
}

void CSocketServer::SendClientList(long &ClientName)
{
	map<long, struct ClientSocket>::iterator it;
	string message;
	for (it = m_ClientList.begin(); it != m_ClientList.end(); ++it)
	{
		char str[100] = { 0 };
		sprintf_s(str, "%d:%s", it->first, inet_ntoa(it->second.m_clientaddr.sin_addr));
		message = message + (string)str + " ";
	}
	SendMessage(message.c_str(), message.length(), ClientName);
}

void CSocketServer::SendClientListSize(long &ClientName)
{
	stringstream ss;
	string message;
	ss << m_ClientList.size();
	ss >> message;
	SendMessage(message.c_str(), message.length(), ClientName);
}