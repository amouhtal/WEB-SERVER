#include "server.hpp"
#include "socket.hpp"
namespace SERVER
{

	size_t getContentLen(std::string request)
	{
		size_t pos = 0;
		size_t find_pos = 0;
		size_t end_pos = request.find("\n");

		while (end_pos != std::string::npos)
		{
			std::string l = request.substr(pos, end_pos - pos);

			if ((find_pos = l.find("Content-Length:")) != std::string::npos && find_pos == 0)
				return (atoi(l.substr(l.find(":") + 2, l.length()).c_str()));
			else if ((find_pos = l.find("\r\n\r\n")) != std::string::npos && find_pos == 0)
				return (0);

			pos = end_pos + 1;
			end_pos = request.find("\n", pos);
		}
		return (0);
	}

	int checkContent(std::string request)
	{
		size_t begin = 0;
		size_t find_pos = 0;
		size_t end = request.find("\r\n");
		// std::cout << "request : " << request << std::endl;
		while (end != std::string::npos)
		{
			std::string header = request.substr(begin, end - begin);

			if ((find_pos = header.find("Transfer-Encoding: chunked")) != std::string::npos && find_pos == 0)
				return (2);
			else if ((find_pos = header.find("Content-Length")) != std::string::npos && find_pos == 0)
				return (1);
			else if ((find_pos = header.find("\r\n\r\n")) != std::string::npos && find_pos == 0)
				return (0);
			begin = end + 1;
			end = request.find("\n", begin);
		}
		return (0);
	}

	bool checkReq(Client client)
	{
		int status;
		std::string request;
		size_t pos;
		std::string body;

		request = client.getRequest();
		status = checkContent(request);
		pos = request.find("\r\n\r\n");
		if (status == 0 && pos != std::string::npos)
		{
			return (true);
		}
		if (status == 1)
		{
			body = request.substr(pos + 4, request.length() - (pos + 4));

			// printf("body: %d content-lenght : %d\n", body.length(), getContentLen(request));
			if (body.length() == getContentLen(request) ||
				body.find("\r\n\r\n") != std::string::npos)
				return (true);
		}
		if (status == CHUNKED)
		{
			body = request.substr(pos + 4, request.length() - (pos + 4));
			pos = body.find("0\r\n\r\n");
			if (pos != std::string::npos)
				if (pos == 0 || (body[pos - 2] == '\r' && body[pos - 1] == '\n'))
					return (true);
		}
		return (false);
	}

	void ASERVER::newClient(int &sockFD)
	{
		int accptSockFD = accept(sockFD, (struct sockaddr *)&_Adrress, &_addrLen);
		if (accptSockFD == -1)
			perror("[ERROR] Socket");
		std::cout << "New connection: Master socket " << std::to_string(sockFD) << ". Accept socket " + std::to_string(accptSockFD) << ", address " << inet_ntoa(_Adrress.sin_addr) << ":" << std::to_string(ntohs(_Adrress.sin_port)) << std::endl;
		// if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
		// 	perror("ERROR] fcntl");
		FD_SET(accptSockFD, &_socket._masterFDs);
		FD_SET(accptSockFD, &_socket._writefds);
		// _socket._masterSockFDs.push_back(accptSockFD);
		if (accptSockFD > _maxSockFD)
			_maxSockFD = accptSockFD;

		std::ofstream outfile;

		outfile.open("server.log", std::ios_base::app);
		time_t now = time(0);
		tm *localtm = localtime(&now);

		outfile << "client socket : " << accptSockFD << " "
				<< "ip : " << inet_ntoa(_Adrress.sin_addr) << " "
				<< "Port : " << std::to_string(ntohs(_Adrress.sin_port)) << " "
				<< "Date : " << asctime(localtm);

		outfile.close();
		Client *newClient = new Client(accptSockFD, "", inet_ntoa(_Adrress.sin_addr));

		// _clients.push_back(Client(accptSockFD, "", inet_ntoa(_Adrress.sin_addr)));
		_clients.push_back(*newClient);
		_clientList.insert(std::pair<int, std::string>(accptSockFD, ""));
		std::map<int, int>::iterator it = _accptMaster.find(accptSockFD);
		if (it != _accptMaster.end())
			it->second = sockFD;
		else
			_accptMaster.insert(std::pair<int, int>(accptSockFD, sockFD));
	}

	void ASERVER::waitClients()
	{
		char message[70] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\nhello\n";
		std::cout << "\n"
				  << "+++++++ Waiting for new connection +++++++"
				  << "\n";

		while (running)
		{
			// FD_ZERO(&_readFDs);
			// _readFDs = _socket._masterFDs;
			FD_ZERO(&_socket._workingFDs);
			_socket._workingFDs = _socket._masterFDs;
			memcpy(&_socket._workingFDs, &_socket._masterFDs, sizeof(_socket._masterFDs));
			// poll kqueue
			_activity = select(_maxSockFD + 1, &_socket._workingFDs, &_socket._writefds, NULL, NULL);
			if (_activity == -1)
				perror("[ERROR] SELECT");
			if (_activity > 0)
			{
				bool _treat = false;
				bool bool_treat = false;
				std::vector<int>::iterator it;
				for (it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
				{
					if (FD_ISSET(*it, &_socket._workingFDs))
					{
						this->newClient(*it);
						break;
					}
				}

				for (size_t CurrentCli = 0; CurrentCli < _clients.size(); CurrentCli++)
				{
					Client &client = _clients[CurrentCli];
					int sockFD = client.getSockFd();
					if (FD_ISSET(sockFD, &_socket._workingFDs) && !bool_treat)
					{
						char _buffRes[BUFFER_SIZE + 1];
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
							if (sockFD == _maxSockFD)
								_maxSockFD--;
							_clientList.erase(sockFD);
							_clients.erase(_clients.begin() + CurrentCli);
							CurrentCli--;
						}
						else if (valRead > 0)
						{
							client.appendReq(_buffRes);
							// std::cout << std::endl
							// 		  << "request string : " << client.getRequest() << std::endl;
							client.setReceived(checkReq(client));
						}

						std::string statusLine;
						std::string bodyMessage;
						std::map<std::string, std::string> _responseHeaders;
						std::string respStr;

						statusLine = "HTTP/1.1 200 OK";
						std::ifstream file;
						std::ostringstream streambuff;
						file.open("/Users/amouhtal/Desktop/web-serv/webserv/giphy.gif", std::ios::binary);
						if (file.is_open())
						{
							streambuff << file.rdbuf();
							bodyMessage = streambuff.str();
							file.close();
						}
						_responseHeaders["Content-Length"] = std::to_string(bodyMessage.length());
						//Content-Type: image
						_responseHeaders["Content-Type"] = "image/gif";
						respStr += statusLine;
						respStr += "\r\n";
						std::map<std::string, std::string>::iterator it = _responseHeaders.begin();
						while (it != _responseHeaders.end())
						{
							respStr += it->first;
							respStr += ": ";
							respStr += it->second;
							respStr += "\n";
							it++;
						}
						respStr += "\r\n";
						respStr += bodyMessage;
						respStr += "\r\n\r\n";
						// std::cout << respStr << std::endl;
						client.setRequest(respStr);
					}

					if (FD_ISSET(sockFD, &_socket._writefds) && client.getReceived())
					{
						int SendRet;
						std::string respStr = client.getRequest();
						std::cout << "---->" << respStr << std::endl;
						SendRet = send(sockFD, respStr.c_str(), strlen(respStr.c_str()), 0);
						if (SendRet < 0)
						{
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterFDs);
							FD_CLR(sockFD, &_socket._writefds);
							_clientList.erase(sockFD);
							if (sockFD == _maxSockFD)
								_maxSockFD--;
							CurrentCli--;
						}
						else
						{
							client.setReceived(true);
							bool_treat = true;

						}
					}
				}
			}
			usleep(2000);
		}
	}

	ASERVER::~ASERVER(void)
	{
		// exit(1);
		// for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end() ; it++)
		// {
		// it->~Client();
		// }
	}
}
