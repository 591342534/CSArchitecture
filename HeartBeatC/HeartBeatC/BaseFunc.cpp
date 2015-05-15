#include "BaseFunc.h"

string ToGetLocalTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X %A", localtime(&t));
	string currenttime = (string)tmp;
	return currenttime;
}