/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#include "SocketServer.h"
#include "MessageHandle.h"

void main()
{
	CSocketServer *n_Socket = new CSocketServer();
	CMessageHandle MHandle(n_Socket);
	while (1)
	{
		/*string message;
		cin >> message;
		n_Socket->SendString(message);*/
		if (n_Socket->MessageListSize() > 0)
		{
			struct CMessagePached message = n_Socket->GetMessage();
			
			MHandle.MessageHandleFunc(message);
		}
	}
}