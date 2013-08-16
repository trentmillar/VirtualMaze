#pragma once

#include <cwchar>
#include <cassert>

#include <windows.h>
#include <ddraw.h>
#include <stdio.h>

class Logger
{
public:
	static void Log(char *message);
	static void Die(char *message, char *file, int line);

	static bool IsLoggingOn;

};

