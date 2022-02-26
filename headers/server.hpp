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
#include "Client.hpp"
#include <fstream>
#include "time.h"
#include <sstream>


#include"parssingfile.hpp"
#include "request.hpp"
#include "response.hpp"
#define BUFFER_SIZE 99
#define CHUNKED 2
#define running 1

namespace SERVER
{
	/*
		* this class is response of create  socket, bind it,
		and listen for any incmonig connection
	*/
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
		fd_set _masterRFDs;
		fd_set _masterWFDS;

		fd_set _readFDs;
		fd_set _writeFDs;

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
			return _masterRFDs;
		}
	};

	class ASERVER
	{
	private:
		fd_set _masterRFDs;
		fd_set _masterWFDS;
		int _maxSockFD;
		std::vector<int> _masterSockFDs;
		std::vector<short> _ports;
		short _port;
		struct sockaddr_in _Adrress;
		socklen_t _addrLen;

		fd_set _readFDs;
		int _activity;
		std::vector<Client *> _clients;
		std::map<int, std::string> _clientList;
		std::map<int, int> _accptMaster;
		ASOCKET _socket;
		/////////////////////////
		dataserver _data_server;
		Request _requset;
	public:
		void launch(dataserver Dserver)
		{
			_socket.SetupSocket();
			this->_masterRFDs = _socket._masterRFDs;
			this->_masterWFDS = _socket._masterWFDS;
			this->_maxSockFD = _socket._maxSockFD;
			this->_masterSockFDs = _socket._masterSockFDs;
			this->_ports = _socket._ports;
			this->_port = _socket._port;
			this->_Adrress = _socket._Adrress;
			this->_addrLen = _socket._addrLen;
			this->_data_server = Dserver;
			waitClients();
		}
		void waitClients();
		void newClient(int &sockFD);
		dataserver	getDataServer();
		Request	getRequest();
		~ASERVER(void);
	};
}

#endif