#ifndef BINDING_SOCKET
#define BINDING_SOCKET
#include "../SimpleSocket.hpp"

namespace WS
{
	class BindingSocket : public SimpleSocket
	{
	public:
		BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
		{
			set_connection(connect_to_network(get_sock(), get_adress()));
			test_connection(get_connection());
		}

		int connect_to_network(int sock, struct sockaddr_in adress)
		{
			return bind(sock, (struct sockaddr *)&adress, sizeof(adress));
		}
	};
}


#endif