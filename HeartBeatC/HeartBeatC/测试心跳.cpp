/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketClient for C/S
*/
#include "SocketClient.h"
#include<string>

void main()
{
	CSocketClient *n_Socket = new CSocketClient("172.16.200.184"); 

	while (1)
	{
		string message;
		cin >> message;
		n_Socket->SendMessage(message.c_str(), message.length());
		/*if (n_Socket->MessageListSize() > 0)
		{
			cout << n_Socket->GetMessage() << endl;
		}*/
	}
}
