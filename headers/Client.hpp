#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
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
		int _RecRetSnd;
	public:
		Client(int sockFd, std::string request, std::string ip);
		void		setRecRetSnd(size_t SendRetSnd);
		int			GetRecRetSnd();
		void		SendRetSnd(size_t SendRetSnd);
		int			GetRetSnd();
		void		setLenReq(size_t lenReq);
		size_t		getLenReq();
		bool		getEndofReq();
		void		setgetEndofReq(bool getEndofReq);
		void		appendReq(char *request, int lenght);
		std::string &getRequest();
		void 		setRequest(std::string request);
		Client		&operator=(Client &rhs);
		int			getSockFd();
		void		setReceived(bool received);
		bool		getReceived();
		bool		operator!=(Client &rhs);
		bool		operator==(Client const &rhs);
		Client();
		~Client();
	};
}

#endif