#ifndef CONNECTION_SOCKET
#define CONNECTION_SOCKET
#include "../SimpleSocket.hpp"

namespace WS
{
	class ConnectingSocket : public SimpleSocket
	{
	private:
	public:
		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
		{
			set_connection(connect_to_network(get_sock(), get_adress()));
			test_connection(get_connection());
		}

		int	connect_to_network(int sock, struct sockaddr_in adress)
		{
			return connect(sock, (struct sockaddr *)&adress, sizeof(adress));
		}
	};
	
} // namespace WS

#endif