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

//字体颜色设置
#define SetTextRed()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);//设置文本设为红色
#define SetTextGreen()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);//设置文本设为绿色
#define SetTextWhite()  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);//设置文本设为白色

string ToGetLocalTime();


#endif