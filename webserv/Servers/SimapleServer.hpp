#ifndef SIMPLE_SERVER_HPP
#define SIMPLE_SERVER_HPP

#include "../hdelibc.hpp"

namespace  WS
{
	class SimapleServer
	{
	private:
		listeningSocket * socket;
		virtual void accepter() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;
	public:
		SimapleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
		{
			socket = new listeningSocket(domain, service, protocol, port, interface, backlog);
		}
		listeningSocket *get_socket()
		{
			return socket;
		}
		virtual void launch() = 0;
	};
	
}
#endif