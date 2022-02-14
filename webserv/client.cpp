#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <map>







int main()
{
	char *message;
	struct sockaddr_in adress;
	int sockfd ;
	
	if (sockfd = socket(AF_INET, SOCK_STREAM, 0) == -1)
	{
		perror("socket");
	}

	
}