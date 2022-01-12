
#include "../BindingSocket/BindinSocket.hpp"
#ifndef LISTENING_SOCKET
#define LISTENING_SOCKET

namespace WS
{
	class listeningSocket : public BindingSocket
	{
		private : 
			int backlog;
			int listening;
	public:
		listeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
		{
			this->backlog = backlog;
			start_listening();
			test_connection(listening);
			
		}

		void start_listening()
		{
			listening = listen(get_sock(), backlog);
		}
	};
}

#endif