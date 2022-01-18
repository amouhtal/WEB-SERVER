#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#define TRUE 1
#define FALSE 0
#define PORT 80
#define PORT_NUMBERS 3
#define MAX_CLIENTS 30
#define running 1
class Sockets
{
private:
	// Sockets *_socket;
	// struct sockaddr_in address[PORT_NUMBERS];
	int PORTS[PORT_NUMBERS];
	int max_clients = 30;
	int client_socket[30];
	char buffer[1025];
    int opt = TRUE;  

public:
	int master_sockets[PORT_NUMBERS];

public:
	Sockets()
	{
		PORTS[0] = 80;
		PORTS[1] = 800;
		PORTS[2] = 801;

		for (int i = 0; i < max_clients; i++)
		{
			client_socket[i] = 0;
		}
	}

	// initialise all client_socket[]
	void initialise_socket()
	{
		for (int i = 0; i < max_clients; i++)
		{
			client_socket[i] = 0;
		}
	}

	// create a master socket
	void create_sockets()
	{
		for (size_t i = 0; i < PORT_NUMBERS; i++)
		{
			if ((master_sockets[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0)
			{
				perror("socket failed");
				exit(EXIT_FAILURE);
			}
			if (setsockopt(master_sockets[i], SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0)
			{
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}
		}
	}

	// create_sockets_types

	void initialise_adress(sockaddr_in address[3])
	{
		for (size_t i = 0; i < PORT_NUMBERS; i++)
		{
			address[i].sin_family = AF_INET;
			address[i].sin_addr.s_addr = INADDR_ANY;
			address[i].sin_port = htons(PORTS[i]);
		}
	}

	// bind sockets
	void bind_sockets(sockaddr_in address[3])
	{
		int bd;
		for (size_t i = 0; i < PORT_NUMBERS; i++)
		{

			if ((bd = bind(master_sockets[i], (struct sockaddr *)&address[i], sizeof(address[i]))) < 0)
			{
				perror("bind failed");
				exit(EXIT_FAILURE);
			}
		}
	}

	// listening
	void socket_listenining()
	{
		for (size_t i = 0; i < PORT_NUMBERS; i++)
		{
			if (listen(master_sockets[i], 5) < 0)
			{
				perror("listen");
				exit(EXIT_FAILURE);
			}
		}
	}

	// Child sockets listening
	int add_child_sockets(fd_set &preadfds)
	{
		int sd;
		int max_sd;

		for (int i = 0; i < max_clients; i++)
		{
			// socket descriptor
			sd = client_socket[i];

			// if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &preadfds);

			// highest file descriptor number, need it for the select function
			if (sd > max_sd)
				max_sd = sd;
		}
		return max_sd;
	}

	// ADD NEW SOCKETS

	void add_new_sockets(int new_socket)
	{
		for (int i = 0; i < 30; i++)
		{
			// if position is empty
			if (client_socket[i] == 0)
			{
				client_socket[i] = new_socket;
				printf("Adding to list of sockets as %d\n", i);

				break;
			}
		}
	}

	// its some IO operation on some other socket

	void m_operation(fd_set &readfds, sockaddr_in address[3])
	{
		int sd;
		int valread;
		int addrlen;
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = client_socket[i];
			addrlen = sizeof(address[i]);
			if (FD_ISSET(sd, &readfds))
			{
				// Check if it was for closing , and also read the
				// incoming message
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					// Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address,
								(socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						   inet_ntoa(address[i].sin_addr), ntohs(address[i].sin_port));

					// Close the socket and mark as 0 in list for reuse
					close(sd);
					client_socket[i] = 0;
				}

				// Echo back the message that came in
				else
				{
					// set the string terminating NULL byte on the end
					// of the data read
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	// fd_set get_fds()
	// {
	// 	return readfds;
	// }

	int *get_master_sockets()
	{
		return master_sockets;
	}

	// sockaddr_in *get_adress()
	// {
	// 	return address;
	// }
};