#include "server.hpp"

namespace SERVER
{
	class Client
	{
	private:
		int _sockFd;
		std::string _ip;
		std::string _request;
		bool _endOfReq;
		bool _received;
		size_t _lenReq;
		int _SendRetSnd;

	public:
		Client(int sockFd, std::string request, std::string ip)
		{
			_sockFd = sockFd;
			_request = request;
			_ip = ip;
			_endOfReq = false;
			_lenReq = 0;
			_SendRetSnd = 0;
		}

		void SendRetSnd(size_t SendRetSnd)
		{
			_SendRetSnd = SendRetSnd;
		}

		int GetRetSnd()
		{
			return (_SendRetSnd);
		}

		void setLenReq(size_t lenReq)
		{
			_lenReq = lenReq;
		}

		size_t getLenReq()
		{
			return (_lenReq);
		}

		bool getEndofReq()
		{
			return _endOfReq;
		}

		void setgetEndofReq(bool getEndofReq)
		{
			_endOfReq = getEndofReq;
		}
		void appendReq(char *request)
		{
			_request += request;
		}

		std::string &getRequest()
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
		
		bool operator==(Client const &rhs)
		{
			return (*this == rhs);
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