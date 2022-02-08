#include "socket.hpp"
#include "server.hpp"

# include <signal.h>

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << "ctrl-c detected, closing application..." << std::endl;
		throw std::exception();
	}
	if (signum == SIGQUIT)
	{
		std::cout << "ctrl-d detected, closing application..." << std::endl;
		throw std::exception();
	}
}

int main()
{
    SERVER::ASERVER runServer;
    signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
    runServer.launch();

    /*
    struct sockaddr_in antelope;
    char *some_addr;

    inet_aton("10.0.0.1", &antelope.sin_addr); // store IP in antelope

    some_addr = inet_ntoa(antelope.sin_addr); // return the IP
    printf("%s\n", some_addr); // prints "10.0.0.1"

    // and this call is the same as the inet_aton() call, above:
    antelope.sin_addr.s_addr = inet_addr("10.0.0.1");
    */
}