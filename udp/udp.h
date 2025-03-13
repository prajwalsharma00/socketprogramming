#pragma once
#if defined(_WIN32)
#ifndef(_WIN32_INTT)
#define _WIN32_INTTT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#endif
#if defined(_WIN32)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERROR() (WSAGETLastError)
#define ISSOCKETVALID(s) (s != INVALIDSOCKET)

#else
#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERROR() (errno)
#define ISSOCKETVLAID(s) (s >= 0)
#endif
#include <stdio.h>
#include <string.h>
