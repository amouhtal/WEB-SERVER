
#ifndef TestServer_hpp
#define TestServer_hpp

#include "SimapleServer.hpp"
#include "unistd.h"

namespace WS
{
	class TestServer : public SimapleServer
	{
		private :
			char buffer[30000] = {0};
			int new_socket;
			void accepter()
			{
				struct sockaddr_in adress = get_socket()->get_adress();
				int addrlen = sizeof(adress);
				new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&adress, (socklen_t *)&addrlen);
				read(new_socket, buffer, 30000);
			}
			void handler()
			{
				std::cout << buffer << std::endl;
			}

			void responder()
			{
				char *hello = "Hello from server";

				write(new_socket, hello, strlen(hello));
				close(new_socket);
			}

		public :
			void launch()
			{
				while (true)
				{
					std::cout << "=== WAITING ===" << std::endl;

					accepter();
					handler();
					responder();
					std::cout << "=== DONE ===" << std::endl;
				}
			}

			TestServer() : SimapleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
			{
				launch();
			}
	};
}



#endif