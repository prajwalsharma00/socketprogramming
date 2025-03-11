#ifndef TCP_H
#define TCP_H
#if defined(_WIN32)
#ifndef(_WIN32_INTT)
#define _WIN32_INTT 0x0600
#endif
#include <winsock2.h>
#include <w2stcpip.h>
#else
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#endif

#if defined(_WIN32)
#define ISSOCKETVALID(s) ((s) != INVALID_SOCKET)
#define GETSOCKETERROR() (WSAGetLastError)
#define CLOSESOCKET(S) close_socket(s)

#else
#define ISSOCKETVALID(s) (s >= 0)
#define GETSOCKETERRNO() (errno)
#define SOCKET int
#define CLOSESOCKET(s) close(s)
#endif

#include <string.h>
#endif