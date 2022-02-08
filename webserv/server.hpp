#ifndef _SERVER_HPP_
#define _SERVER_HPP_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <map>
#include "socket.hpp"

#define BUFFER_SIZE 1024
#define running 1

namespace SERVER
{
	class ASOCKET
	{
	public:
		std::vector<short> _ports;
		short _port;
		std::string _host;
		std::vector<int> _masterSockFDs;
		int _masterSockFD;
		struct sockaddr_in _Adrress;
		socklen_t _addrLen;
		fd_set _masterFDs;
		fd_set _workingFDs;

		fd_set _writefds;
		int _maxSockFD;

	public:
		ASOCKET(){};
		void SetupSocket();
		void CreatSocket();
		void BindSocket();
		void ListenSocket();

		// Getters
		fd_set Getmasterfd()
		{
			return _masterFDs;
		}
	};

	class ASERVER
	{
	private:
		fd_set _masterFDs;
		fd_set _writeFDs;
		int _maxSockFD;
		std::vector<int> _masterSockFDs;
		std::vector<short> _ports;
		short _port;
		struct sockaddr_in _Adrress;
		socklen_t _addrLen;

		fd_set _readFDs;
		int _activity;
		std::map<int, std::string> _clientList;
		std::map<int, int> _accptMaster;
		ASOCKET _socket;

	public:
		void launch()
		{
			_socket.SetupSocket();
			this->_masterFDs = _socket._masterFDs;
			this->_writeFDs = _socket._writefds;
			this->_maxSockFD = _socket._maxSockFD;
			this->_masterSockFDs = _socket._masterSockFDs;
			this->_ports = _socket._ports;
			this->_port = _socket._port;
			this->_Adrress = _socket._Adrress;
			this->_addrLen = _socket._addrLen;
			waitClients();
		}
		void waitClients();
		void newClient(int &sockFD);
	};
}

#endif