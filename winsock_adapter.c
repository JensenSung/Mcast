/* ex: set shiftwidth=4 tabstop=4 expandtab: */

/**
 * @file winsock_adapter.c
 * @author T.Ostaszewski 
 * @date 04-Jan-2012
 * @brief
 * @details
 */

#include "pcc.h"

#if !defined WIN32

int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	return 0;
}

int	WSACleanup(void)
{
}

#endif
