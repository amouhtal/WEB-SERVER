#include "Socket.hpp"

int main(int argc, char *argv[])
{
	Sockets c_socket;
	struct sockaddr_in address[3];
	int max_sd;
	int activity;
	int new_socket[PORT_NUMBERS];
	int sd;

	new_socket[0] = 0;
	// new_socket[1] = 0;
	// new_socket[2] = 0;
	char *message = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8\r\nContent-Length:5\r\n\r\nhello\n";

	c_socket.initialise_socket();
	c_socket.create_sockets();
	c_socket.initialise_adress(address);
	c_socket.bind_sockets(address);
	c_socket.socket_listenining();
	fd_set readfds;
	// address = c_socket.get_adress();

	// int *master_sockets;
	int addrlen;
	int valread;
	while (running)
	{
		FD_ZERO(&readfds);
		FD_SET(c_socket.master_sockets[0], &readfds);
		
		max_sd = c_socket.add_child_sockets(readfds);
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		printf("New connection , socket \n");
		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		if (FD_ISSET(c_socket.master_sockets[0], &readfds))
		{
			for (size_t i = 0; i < PORT_NUMBERS; i++)
			{
				addrlen = sizeof(address[i]);
				if ((new_socket[i] = accept(c_socket.master_sockets[i],
											(struct sockaddr *)&address[i], (socklen_t *)&addrlen)) < 0)
				{
					perror("accept");
					exit(EXIT_FAILURE);
				}
			}
		}

		// for (size_t i = 0; i < PORT_NUMBERS; i++)
		// {
		// 	if (new_socket[i] != 0)
		// 		if (send(new_socket[i], message, strlen(message), 0) != strlen(message))
		// 		{
		// 			perror("send");
		// 		}
		// }

		puts("Welcome message sent successfully");
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			// if position is empty
			if (new_socket[i] != 0)
				c_socket.add_new_sockets(new_socket[i]);
		}

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = c_socket.client_socket[i];
			addrlen = sizeof(address[0]);
			std::cout << "sd : " << sd  << std::endl;
			if (FD_ISSET(sd, &readfds))
			{
				puts("->1");
				// Check if it was for closing , and also read the
				// incoming message
				if ((valread = read(sd, c_socket.buffer, 1024)) == 0)
				{
					// Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address[0],
								(socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						   inet_ntoa(address[0].sin_addr), ntohs(address[0].sin_port));

					// Close the socket and mark as 0 in list for reuse
					close(sd);
					c_socket.client_socket[i] = 0;
				}

				// Echo back the message that came in
				else
				{
					puts("->2");
					// set the string terminating NULL byte on the end
					// of the data read
					c_socket.buffer[valread] = '\0';
					send(sd, c_socket.buffer, strlen(c_socket.buffer), 0);
				}
				puts("->3");
			}
			puts("->4");

		}
	}
}
