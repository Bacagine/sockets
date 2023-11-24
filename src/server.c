/**
 * server.c
 *
 * Copyright (C) 2023
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <https://www.gnu.org/licenses/>
 *
 * Developed by geek4geeks <https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/>
 * Modified by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 * 
 * Example of server using sockets in C
 * 
 * Compiling with: gcc -o server server.c -Wall -Wextra -O3 -ansi
 *
 * Date: 2023-10-31
 */

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/
#include <stdio.h> 
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h> 
#include <strings.h> /* bzero()                  */
#include <unistd.h>  /* read(), write(), close() */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

/**
 * Used to ignore warnings in compilation
 * about unused variables
 */
#define UNUSED(X) (void) X

/**
 * Comunication definitions
 */
#define MAX 80 
#define PORT 8080 

/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/
typedef struct sockaddr STRUCT_SOCKADDR;
typedef struct sockaddr * PSTRUCT_SOCKADDR;

typedef struct sockaddr_in STRUCT_SOCKADDR_IN;
typedef STRUCT_SOCKADDR_IN * PSTRUCT_SOCKADDR_IN;

/******************************************************************************
 *                                                                            *
 *                                 Functions                                  *
 *                                                                            *
 ******************************************************************************/

/**
 * Function designed for chat between stClient and server.
 */
void vChat(int iConnFd) 
{ 
	char szBuf[MAX]; 
	int ii; 

	/**
   * infinite loop for chat
   */
	while(true)
  { 
		bzero(szBuf, MAX); 

		/**
     * read the message from stClient and copy it in szBufer 
     */
    read(iConnFd, szBuf, sizeof(szBuf)); 
		
    /**
     * print szBufer which contains the stClient contents
     */
    printf("From Clientent: %s"
           "Type a string to send to the client: ", szBuf
    ); 
		bzero(szBuf, MAX); 
		ii = 0; 
		
    /**
     * copy server message in the szBufer
     */
		while((szBuf[ii++] = getchar()) != '\n'); 

		/**
     * and send that szBufer to stClient
     */
		write(iConnFd, szBuf, sizeof(szBuf));

		/**
     * if msg contains "Exit" then server exit and chat ended. 
		 */
    if(strncmp("exit", szBuf, 4) == 0)
    { 
			printf("Server Exit...\n"); 
      
			break; 
		} 
	} 
} 

/******************************************************************************
 *                                                                            *
 *                                   main                                     *
 *                                                                            *
 ******************************************************************************/
int main(int argc, char **argv) 
{ 
	int iSockFd;
  int iConnFd;
  int iLength; 
	STRUCT_SOCKADDR_IN stServAdd;
  STRUCT_SOCKADDR_IN stClient; 
  
  UNUSED(argc);
  UNUSED(argv);
  
  memset(&stServAdd, 0, sizeof(stServAdd));
  memset(&stClient , 0, sizeof(stClient ));

	/**
   * socket create and verification
   */
	iSockFd = socket(AF_INET, SOCK_STREAM, 0); 
	
  if(iSockFd == -1)
  { 
		fprintf(stderr, "E: socket creation failed...\n"); 
    
		exit(EXIT_FAILURE);
	} 
	else
  {
		printf("Socket successfully created..\n"); 
  }
  
  bzero(&stServAdd, sizeof(stServAdd)); 

	/**
   * assign IP, PORT
   */
	stServAdd.sin_family = AF_INET; 
	stServAdd.sin_addr.s_addr = htonl(INADDR_ANY); 
	stServAdd.sin_port = htons(PORT); 

	/**
   * Binding newly created socket to given IP and verification 
   */
	if((bind(iSockFd,(PSTRUCT_SOCKADDR) &stServAdd, sizeof(stServAdd))) != 0)
  { 
		fprintf(stderr, "E: socket bind failed...\n"); 
    
		exit(EXIT_FAILURE); 
	} 

  printf("Socket successfully binded..\n"); 

	/**
   * Now server is ready to listen and verification 
   */
	if((listen(iSockFd, 5)) != 0)
  { 
		fprintf(stderr, "E: Listen failed...\n"); 
  
    exit(EXIT_FAILURE); 
	} 

  printf("Server listening..\n"); 

  iLength = sizeof(stClient); 

	/**
   * Accept the data packet from stClient and verification
   */
	iConnFd = accept(iSockFd, (PSTRUCT_SOCKADDR) &stClient, (socklen_t *) &iLength); 
	if(iConnFd < 0)
  { 
		fprintf(stderr, "E: server accept failed...\n"); 
    
		exit(EXIT_FAILURE); 
	} 

  printf("server accept the stClient...\n"); 

	/**
   * Function for chatting between stClient and server 
	 */
  vChat(iConnFd); 

	/**
   * After chatting close the socket 
	 */
  close(iSockFd); 

  return 0;
}

