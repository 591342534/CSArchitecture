/*
Copyright 2015. All rights reserved.

Authors: Du Ruoqi
Date:2015.4.10

SocketServer for C/S
*/
#ifndef __BASEFUNC_H__
#define __BASEFUNC_H__
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

//������ɫ����
#define SetTextRed()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);//�����ı���Ϊ��ɫ
#define SetTextGreen()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);//�����ı���Ϊ��ɫ
#define SetTextWhite()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);//�����ı���Ϊ��ɫ

string ToGetLocalTime();


#endif