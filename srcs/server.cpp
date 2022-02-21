#include "../headers/server.hpp"
// #include "socket.hpp"

// #include <winsock2.h>
// #include <Ws2tcpip.h>
#include <stdio.h>

namespace SERVER
{

	size_t getContentLen(std::string request)
	{
		size_t pos = 0;
		size_t find_pos = 0;
		size_t end_pos = request.find("\n");

		while (end_pos != npos)
		{
			std::string l = request.substr(pos, end_pos - pos);

			if ((find_pos = l.find("Content-Length:")) != npos && find_pos == 0)
				return (atoi(l.substr(l.find(":") + 2, l.length()).c_str()));
			else if ((find_pos = l.find("\r\n\r\n")) != npos && find_pos == 0)
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
		while (end != npos)
		{
			std::string header = request.substr(begin, end - begin);

			if ((find_pos = header.find("Transfer-Encoding: chunked")) != npos && find_pos == 0)
				return (2);
			else if ((find_pos = header.find("Content-Length")) != npos && find_pos == 0)
				return (1);
			else if ((find_pos = header.find("\r\n\r\n")) != npos && find_pos == 0)
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
		if (status == 0 && pos != npos)
		{
			// std::cout << "1\n";
			// exit(1);
			return (true);
		}
		if (status == 1)
		{
			// std::cout << "2\n";

			body = request.substr(pos + 4, request.length() - (pos + 4));
			std::cout << "lenght " << body.length() << " -> " << getContentLen(request) << std::endl;
			// exit(1);
			printf("pos:/ %d content-lenght : %d\n", client.GetRecRetSnd() - (pos + 4), getContentLen(request));
			if (client.GetRecRetSnd() - (pos + 4) == getContentLen(request))
			{
				std::ifstream file;
				std::ostringstream streambuff;
				std::cout << client.getRequest() << std::endl;
				std::ofstream supp_info_output("/Users/amouhtal/Desktop/web-serv/webserv/new3.ico",  std::ios::out | std::ios::binary);

				supp_info_output.write(body.c_str(), getContentLen(request));
				supp_info_output.close();
				file.open("/Users/amouhtal/Desktop/web-serv/webserv/new3.ico",  std::ios::in | std::ios::out | std::ios::binary);
				if (file.is_open())
				{
					// body << file.get();
					std::istringstream gfg1(body);
					//file << body.c_str();
					// bodyMessage = streambuff.str();
					file.close();
				}
				return (true);
			}
			// if (body.length() == getContentLen(request))
			// 	return (true);
			// if (body.find("\r\n\r\n") != npos)
			// 	return (true);
		}
		if (status == CHUNKED)
		{
			// std::cout << "1\n";
			// exit(1);

			body = request.substr(pos + 4, request.length() - (pos + 4));
			pos = body.find("0\r\n\r\n");
			if (pos != npos)
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
		if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
			perror("ERROR] fcntl");
		FD_SET(accptSockFD, &_socket._masterRFDs);
		FD_SET(accptSockFD, &_socket._masterWFDS);
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
		_clients.push_back(newClient);
		// _clientList.insert(std::pair<int, std::string>(accptSockFD, ""));
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
			// _readFDs = _socket._masterRFDs;
			// FD_ZERO(&_socket._readFDs);
			// FD_ZERO(&_socket._readFDs);
			_socket._readFDs = _socket._masterRFDs;
			_socket._writeFDs = _socket._masterWFDS;
			// memcpy(&_socket._readFDs, &_socket._masterRFDs, sizeof(_socket._masterRFDs));
			// poll kqueue
			_activity = select(_maxSockFD + 1, &_socket._readFDs, &_socket._writeFDs, NULL, NULL);
			if (_activity == -1)
				perror("[ERROR] SELECT");
			if (_activity > 0)
			{
				bool _treat = false;
				std::vector<int>::iterator it;
				for (it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
				{
					if (FD_ISSET(*it, &_socket._readFDs))
					{
						this->newClient(*it);
						break;
					}
				}

				for (size_t CurrentCli = 0; CurrentCli < _clients.size(); CurrentCli++)
				{
					Client &client = *_clients[CurrentCli];
					bool bool_treat = false;
					int sockFD = client.getSockFd();
					if (FD_ISSET(sockFD, &_socket._readFDs) && client.getEndofReq() == false)
					{
						std::cout << "Remplir reqqq getEndofReq " << client.getEndofReq() << std::endl;

						char _buffRes[BUFFER_SIZE + 1];
						bzero(_buffRes, sizeof(_buffRes));
						int valRead = recv(sockFD, _buffRes, BUFFER_SIZE, 0);
						client.setRecRetSnd(client.GetRecRetSnd() + valRead);
						// std::cout << "valread :" << valRead << " " << client.GetRecRetSnd() << std::endl;
						// std::cout << "Activity in socket " << std::to_string(sockFD) << ", address: " << inet_ntoa(_Adrress.sin_addr) << ':' << std::to_string(ntohs(_Adrress.sin_port)) << std::endl;

						if (valRead < 1)
						{
							std::cout << "Disconnected socket: " << std::to_string(sockFD) << std::endl;
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._readFDs) == false)
									_maxSockFD--;
							// _clientList.erase(sockFD);
							_clients.erase(_clients.begin() + CurrentCli);
							CurrentCli--;
							if (CurrentCli < 0)
								break;
						}
						else if (valRead > 0)
						{
							client.appendReq(_buffRes);
							// std::cout << std::endl
							// 		  << "request string : " << client.getRequest() << std::endl;
							client.setReceived(checkReq(client));
							if(client.getReceived())
							{
								Request r(client.getRequest(),30000,1);
								r.parseRequest();
							}
							// std::cout << "valread :" << valRead << std::endl;
						}
						// if (client.getEndofReq() == false)
						// {
						// 	std::string statusLine;
						// 	std::string bodyMessage;
						// 	std::map<std::string, std::string> _responseHeaders;
						// 	std::string respStr;

						// 	statusLine = "HTTP/1.1 200 OK";
						// 	std::ifstream file;
						// 	std::ostringstream streambuff;
						// 	file.open("/goinfre/amouhtal/tom.mp4", std::ios::binary);
						// 	if (file.is_open())
						// 	{
						// 		streambuff << file.rdbuf();
						// 		bodyMessage = streambuff.str();
						// 		file.close();
						// 	}
						// 	_responseHeaders["Content-Length"] = std::to_string(bodyMessage.length());
						// 	// Content-Type: image
						// 	_responseHeaders["Content-Type"] = "video/mp4";
						// 	respStr += statusLine;
						// 	respStr += "\r\n";
						// 	std::map<std::string, std::string>::iterator it = _responseHeaders.begin();
						// 	while (it != _responseHeaders.end())
						// 	{
						// 		respStr += it->first;
						// 		respStr += ": ";
						// 		respStr += it->second;
						// 		respStr += "\n";
						// 		it++;
						// 	}
						// 	respStr += "\r\n";
						// 	respStr += bodyMessage;
						// 	respStr += "\r\n\r\n";
						// 	client.setReceived(checkReq(client));
						// 	// std::cout << respStr << std::endl;
						// 	client.setRequest(respStr);
						// 	client.setLenReq(respStr.length());
						// 	client.setgetEndofReq(true);
						// }
					}

					if (FD_ISSET(sockFD, &_socket._writeFDs) && client.getReceived())
					{
						// puts("writing step");
						// exit(0);
						
						Response resp(_data_server,_requset,80);
						resp.init_response();
						int SendRet = 0;
						// std::string respStr = client.getRequest();
						std::string respStr = resp.getHeader();
						// std::cout <<"===============================" <<std::endl;
						// std::cout <<respStr <<std::endl;
						// std::cout <<"===============================" <<std::endl;

						client.setLenReq(respStr.length());
						int leng = client.getLenReq() > 294000 ? 294000 : client.getLenReq();
						if (leng > 0)
						{
							SendRet = send(sockFD, respStr.c_str(), leng, 0);

							client.SendRetSnd(client.GetRetSnd() + SendRet);
							try
							{
								client.setRequest(client.getRequest().substr(SendRet, client.getLenReq()));
							}
							catch (std::exception &e)
							{
								std::cout << e.what() << std::endl;
							}
						}
						// std::cout << "sockFD : " << sockFD << " SendRet : " << SendRet << " send lenght : " << client.GetRetSnd() << "req lenght " << client.getLenReq() << std::endl;
						if (SendRet < 1)
						{
							puts("yes im in SendRet < 1");
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							// _clientList.erase(sockFD);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._masterRFDs) == false)
									_maxSockFD--;
							_clients.erase(_clients.begin() + CurrentCli);
							CurrentCli--;
							// client.setReceived(false);
							if (CurrentCli < 0)
								break;
							/*client.getRequest().clear();
							bool_treat = true;
							client.setLenReq(0);
							client.SendRetSnd(0);
							client.setgetEndofReq(false);*/
						}
						else if (client.getRequest().length() == 0)
						{
							puts("yes im in lenght == 0");
							close(sockFD);
							FD_CLR(sockFD, &_socket._masterRFDs);
							FD_CLR(sockFD, &_socket._masterWFDS);
							// _clientList.erase(sockFD);
							if (sockFD == _maxSockFD)
								while (FD_ISSET(_maxSockFD, &_socket._masterRFDs) == false)
									_maxSockFD--;
							_clients.erase(_clients.begin() + CurrentCli);

							CurrentCli--;
							// client.setReceived(false);
							if (CurrentCli < 0)
								break;
							/*client.getRequest().clear();
							bool_treat = true;
							client.setLenReq(0);
							client.SendRetSnd(0);
							client.setgetEndofReq(false);*/
						}
						else
						{
							// client.setRequest(client.getRequest().c_str() +  SendRet);
							// std::cout << "sendRet " << SendRet << " req lenght : " << (client.getRequest()).length()  << std::endl;
						}
						// std::cout << "req = " << client.getRequest() << std::endl;
						// std::cout << "req2 = " << client.getRequest() << std::endl;
						// puts("yes im in");
					}
				}
				// puts("here");
			}

			// usleep(2000);
		}
	}
	Request	ASERVER::getRequest()
	{
		return this->_requset;
	}

	dataserver	ASERVER::getDataServer()
	{
		return this->_data_server;
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
