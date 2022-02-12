#include "server.hpp"

namespace SERVER
{
	class Client
	{
	private:
		int			_sockFd;
		std::string _ip;
		std::string _request;
		size_t 		_endOfReq;
		bool		_received;

	public:
		Client(int sockFd, std::string request, std::string ip)
		{
			_sockFd = sockFd;
			_request = request;
			_ip = ip;
		}

		void appendReq(char 	*request)
		{
			_request += request;
		}

		std::string getRequest()
		{
			return (_request);
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

		bool operator !=(Client &rhs)
		{
			return (this->_sockFd != rhs._sockFd);
		}

		Client()
		{
		}
	};
}