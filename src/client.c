/**
 * client.c
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
#include <strings.h>    /* bzero()                  */
#include <arpa/inet.h>  /* inet_addr()              */
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>     /* read(), write(), close() */

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
#define MAX            80
#define IP   "127.0.00.1"
#define PORT         8080

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
 * Function designed for chat between stClientent and server.
 */
void vChat(int iSockFd)
{
  char szBuf[MAX];
  int ii;
  
  while(true)
  {
    bzero(szBuf, sizeof(szBuf));
    
    printf("Type a string to send to the server: ");
    ii = 0;
    
    while((szBuf[ii++] = getchar()) != '\n');
    
    write(iSockFd, szBuf, sizeof(szBuf));
    
    bzero(szBuf, sizeof(szBuf));
    
    read(iSockFd, szBuf, sizeof(szBuf));
    
    printf("From Server : %s", szBuf);
    
    if((strncmp(szBuf, "exit", 4)) == 0)
    {
      printf("Client Exit...\n");
      
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

  printf("Socket successfully created..\n");

  bzero(&stServAdd, sizeof(stServAdd));

  /**
   * assign IP, PORT
   */
  stServAdd.sin_family = AF_INET;
  stServAdd.sin_addr.s_addr = inet_addr(IP);
  stServAdd.sin_port = htons(PORT);

  /*+
   * connect the stClientent socket to server socket
   */
  if(connect(iSockFd, (PSTRUCT_SOCKADDR) &stServAdd, sizeof(stServAdd)) != 0)
  {
    fprintf(stderr, "E: connection with the server failed...\n");
    
    exit(EXIT_FAILURE);
  }
  
  printf("connected to the server..\n");

  /**
   * vChattion for chat
   */
  vChat(iSockFd);

  /**
   * close the socket
   */
  close(iSockFd);

  return 0;
}

