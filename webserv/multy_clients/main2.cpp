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
	new_socket[1] = 0;
	new_socket[2] = 0;
	char *message = "hello\n";

	// c_socket.initialise_socket();
	c_socket.create_sockets();
	c_socket.initialise_adress(address);
	c_socket.bind_sockets(address);
	c_socket.socket_listenining();
	fd_set readfds ;	
	// address = c_socket.get_adress();

	// int *master_sockets;
	int addrlen;
	while (running)
	{
		FD_ZERO(&readfds);
		FD_SET(c_socket.master_sockets[0], &readfds);
		max_sd = c_socket.add_child_sockets(readfds);
		printf("-->%d\n", c_socket.master_sockets[0]);
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		puts("here");
		// exit(1);
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
				/* code */
			}
		}

		printf("New connection , socket \n");
		for (size_t i = 0; i < PORT_NUMBERS; i++)
		{
			if (new_socket[i] != 0)
				if (send(new_socket[i], message, strlen(message), 0) != strlen(message))
				{
					perror("send");
				}
		}

		puts("Welcome message sent successfully");
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			// if position is empty
			if (new_socket[i] != 0)
				c_socket.add_new_sockets(new_socket[i]);
		}
		c_socket.m_operation(readfds, address);
	}
}
