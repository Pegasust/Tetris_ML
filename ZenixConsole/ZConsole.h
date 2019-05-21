#pragma once
#include "../Zenix_ML/Zenxis.h"
#include "mkdir.h"
class ZConsole
{
public:
	static void do_nothing();
protected:
	//Print to console
	int (*print_txt)(const char str[]);
	//Save on hard drive
	int (*serialize_txt)(const char str[]);


};