#include "server.hpp"
#include "socket.hpp"
namespace SERVER
{

	void ASERVER::newClient(int &sockFD)
	{
		int accptSockFD = accept(sockFD, (struct sockaddr *)&_Adrress, &_addrLen);
		if (accptSockFD == -1)
			perror("[ERROR] Socket");
		std::cout << "New connection: Master socket " << std::to_string(sockFD) << ". Accept socket " + std::to_string(accptSockFD) << ", address " << inet_ntoa(_Adrress.sin_addr) << ":" << std::to_string(ntohs(_Adrress.sin_port)) << std::endl;
		if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
			perror("ERROR] fcntl");
		FD_SET(accptSockFD, &_socket._masterFDs);
		FD_SET(accptSockFD, &_socket._writefds);
		// _socket._masterSockFDs.push_back(accptSockFD);
		if (accptSockFD > _maxSockFD)
			_maxSockFD = accptSockFD;
		_clientList.insert(std::pair<int, std::string>(accptSockFD, ""));
		std::map<int, int>::iterator it = _accptMaster.find(accptSockFD);
		if (it != _accptMaster.end())
			it->second = sockFD;
		else
			_accptMaster.insert(std::pair<int, int>(accptSockFD, sockFD));
	}

	void ASERVER::waitClients()
	{
		char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\nhello\n";

		std::cout << "\n"
				  << "+++++++ Waiting for new connection +++++++"
				  << "\n";

		while (running)
		{

			// FD_ZERO(&_readFDs);
			// _readFDs = _socket._masterFDs;
			FD_ZERO(&_socket._workingFDs);

			memcpy(&_socket._workingFDs, &_socket._masterFDs, sizeof(_socket._masterFDs));
			// poll kqueue
			_activity = select(_maxSockFD + 1, &_socket._workingFDs, &_socket._writefds, NULL, NULL);

			if (_activity == -1)
				perror("[ERROR] SELECT");
			if (_activity > 0)
			{
				bool bool_treat = false;
				for (int sockFD = 0; sockFD < _maxSockFD + 1; sockFD++)
				{
					bool _treat = false;
					if (FD_ISSET(sockFD , &_socket._workingFDs))
					{
						for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
						{
							if (*it == sockFD)
							{
								printf("it : %d \n", sockFD);
								this->newClient(*it);
								_treat = true;
								break;
							}
						}

						if (_treat)
						{
							break;
						}

						puts("ready for read");
						char _buffRes[BUFFER_SIZE + 1] = {0};
						bzero(_buffRes, sizeof(_buffRes));

						int valRead = recv(sockFD, _buffRes, BUFFER_SIZE, 0);
						std::cout << "valread :" << valRead << std::endl;
						std::cout << "Activity in socket " << std::to_string(sockFD) << ", address: " << inet_ntoa(_Adrress.sin_addr) << ':' << std::to_string(ntohs(_Adrress.sin_port)) << std::endl;

						if (valRead == 0)
						{
							std::cout << "Disconnected socket: " << std::to_string(sockFD) << std::endl;
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterFDs);
							FD_CLR(sockFD, &_socket._writefds);
							_clientList.erase(sockFD);
						}
						else if (valRead > 0)
						{
							_buffRes[valRead] = '\0';
							std::map<int, std::string>::iterator it = _clientList.find(sockFD);
							if (it != _clientList.end())
								it->second += _buffRes;
							bool_treat = true;
							printf("%s\n", _buffRes);
							// exit(1);
							// if (FD_ISSET(sockFD, &_readFDs))
							//{
							// sockFD = accept(sockFD, (struct sockaddr *)&_Adrress, &_addrLen);
							//  write(sockFD , message , strlen(message));
							// close(sockFD);
							//  this->responseHandling(accptSockFD);
							//}
							// int valread = read( sockFD , buffer, 30000);
							// close(accptSockFD);
						}
					}

					if (FD_ISSET(sockFD, &_socket._writefds) )
					{
						puts("writeee");
						int rtn_send;
						rtn_send = send(sockFD, message, strlen(message), 0);
						close(sockFD);
						FD_CLR(sockFD, &_socket._masterFDs);
						FD_CLR(sockFD, &_socket._writefds);
						_clientList.erase(sockFD);
						bool_treat = false;
					}
				}
			}
			usleep(2000);
		}
	}

}
