#include "ListeningSocket/ListeningSocket.hpp"


int main()
{
	std::cout << "Starting ..." << std::endl;

	std::cout << "Binding Socket ..." << std::endl;

	WS::BindingSocket bs = WS::BindingSocket(AF_INET, SOCK_STREAM, 0, 800, INADDR_ANY);
	WS::listeningSocket ls = WS::listeningSocket(AF_INET, SOCK_STREAM, 0, 800, INADDR_ANY, 10);

	std::cout << "Success!" << std::endl;
}