/* ex: set shiftwidth=4 tabstop=4 expandtab: */

/**
 * @file platform-sockets.h
 * @brief Platform agnostic sockets header.
 * @details 
 * @author T.Ostaszewski
 * @par License
 * @code Copyright 2012 Tomasz Ostaszewski. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
 * 	and/or other materials provided with the distribution.
  * THIS SOFTWARE IS PROVIDED BY Tomasz Ostaszewski AS IS AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL Tomasz Ostaszewski OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.
  * The views and conclusions contained in the software and documentation are those of the 
 * authors and should not be interpreted as representing official policies, 
 * either expressed or implied, of Tomasz Ostaszewski.
 * @endcode
 * @date 04-Jan-2012
 */
#if !defined PLATFORM_SOCKETS_H_3AFE234D_384E_4575_8194_4C7D83326B95
#define PLATFORM_SOCKETS_H_3AFE234D_384E_4575_8194_4C7D83326B95

#if defined WIN32
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <windows.h>
typedef struct SOCKADDR_IN * P_SOCKADDR_IN;
typedef struct SOCKADDR_IN6 * P_SOCKADDR_IN6;
#else
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netdb.h>
typedef int SOCKET;
typedef struct sockaddr_in * P_SOCKADDR_IN;
typedef struct sockaddr_in6 * P_SOCKADDR_IN6;
#   define SOCKET_ERROR (-1)
#   define INVALID_SOCKET (-1)
#   define NO_ERROR (0)
#endif

#endif /* PLATFORM_SOCKETS_H_3AFE234D_384E_4575_8194_4C7D83326B95 */

