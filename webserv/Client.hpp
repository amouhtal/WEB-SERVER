#include "server.hpp"

namespace SERVER
{
	class Client
	{
	private:
		int _sockFd;
		std::string _ip;
		std::string _request;
		size_t _endOfReq;
		bool _received;

	public:
		Client(int sockFd, std::string request, std::string ip)
		{
			_sockFd = sockFd;
			_request = request;
			_ip = ip;
		}

		void appendReq(char *request)
		{
			_request += request;
		}

		std::string getRequest()
		{
			return (_request);
		}

		void setRequest(std::string request)
		{
			_request = request;
			
		}

		Client &operator=(Client &rhs)
		{
			this->_sockFd = rhs._sockFd;
			this->_ip = rhs._ip;
			this->_request = rhs._request;
			this->_endOfReq = rhs._endOfReq;
			this->_received = rhs._received;
		}

		int getSockFd()
		{
			return (_sockFd);
		}

		void setReceived(bool received)
		{
			_received = received;
		}

		bool getReceived()
		{
			return (_received);
		}

		bool operator!=(Client &rhs)
		{
			return (this->_sockFd != rhs._sockFd);
		}

		Client()
		{
			this->_sockFd = -1;
			this->_ip = "";
			this->_request = "";
			this->_endOfReq = 0;
			this->_received = false;
		}

		~Client()
		{
			// puts("---->here des");
			// delete this;
		}
	};
}