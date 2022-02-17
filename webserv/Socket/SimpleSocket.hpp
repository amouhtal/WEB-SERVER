#ifndef SIMPLE_SOCKET
#define SIMPLE_SOCKET

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
/*
NOTE
	TCP Sockets
		A connection between two computers uses a socket.
		A socket is the combination of IP address plus port
*/
namespace WS
{
	class SimpleSocket
	{
	private:
		int sock;
		int connection;
		struct sockaddr_in address;

	public:
		virtual int connect_to_network(int sock, struct sockaddr_in adress) = 0;
		SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
		{
			address.sin_family = domain;
			address.sin_port = htons(port);
			address.sin_addr.s_addr = htonl(interface);
			// ESTABLISH SOCKET
			this->sock = socket(domain, service, protocol);
			// if (fcntl(this->sock, F_SETFL, O_NONBLOCK) == -1)
            // perror("[ERROR] in fcntl !");
			test_connection(sock);
		}

		void test_connection(int item_to_test)
		{
			if (item_to_test < 0)
			{
				perror("Failed to connect ...");
				exit(EXIT_FAILURE);
			}
		}

		// GETTER
		struct sockaddr_in get_adress()
		{
			return address;
		}

		int get_sock()
		{
			return sock;
		}

		int get_connection()
		{
			return connection;
		}

		// SETTER
		void set_connection(int con)
		{
			connection = con;
		}
	};
}

#endif