#ifndef __ALL_IMPORTS_H__
#define __ALL_IMPORTS_H__ 1

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

#elif __linux__

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif // __ALL_IMPORTS_H__
