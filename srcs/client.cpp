#include "../headers/Client.hpp"

namespace SERVER
{
	Client::Client(int sockFd, std::string request, std::string ip)
	{
		_sockFd = sockFd;
		_request = request;
		_ip = ip;
		_endOfReq = false;
		_lenReq = 0;
		_SendRetSnd = 0;
		_RecRetSnd = 0;
		_received = false;
	}

	void Client::setRecRetSnd(size_t SendRetSnd)
	{
		_RecRetSnd = SendRetSnd;
	}

	int Client::GetRecRetSnd()
	{
		return (_RecRetSnd);
	}

	void Client::SendRetSnd(size_t SendRetSnd)
	{
		_SendRetSnd = SendRetSnd;
	}

	int Client::GetRetSnd()
	{
		return (_SendRetSnd);
	}

	void Client::setLenReq(size_t lenReq)
	{
		_lenReq = lenReq;
	}

	size_t Client::getLenReq()
	{
		return (_lenReq);
	}

	bool Client::getEndofReq()
	{
		return _endOfReq;
	}

	void Client::setgetEndofReq(bool getEndofReq)
	{
		_endOfReq = getEndofReq;
	}
	void Client::appendReq(char *request, int lenght)
	{

		try
		{
			/* code */
			_request.append(request, lenght);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}

	std::string &Client::getRequest()
	{
		return (_request);
	}

	void Client::setRequest(std::string request)
	{
		_request = request;
	}

	Client &Client::operator=(Client &rhs)
	{
		this->_sockFd = rhs._sockFd;
		this->_ip = rhs._ip;
		this->_request = rhs._request;
		this->_endOfReq = rhs._endOfReq;
		this->_received = rhs._received;
		return *this;
	}

	int Client::getSockFd()
	{
		return (_sockFd);
	}

	void Client::setReceived(bool received)
	{
		_received = received;
	}

	bool Client::getReceived()
	{
		return (_received);
	}

	bool Client::operator!=(Client &rhs)
	{
		return (this->_sockFd != rhs._sockFd);
	}

	bool Client::operator==(Client const &rhs)
	{
		return (*this == rhs);
	}

	Client::Client()
	{
		this->_sockFd = -1;
		this->_ip = "";
		this->_request = "";
		this->_endOfReq = 0;
		this->_received = false;
	}

	Client::~Client()
	{
	}
}