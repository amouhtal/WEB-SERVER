#ifndef RESPONSE_HPP
#define RESPONSE_HPP

// #include <sys/socket.h>
#include"request.hpp"
#include "../ParssingConfig/DataServer.hpp"
#include "../ParssingConfig/location.hpp"
#include <fstream>
#include <sstream>
#define npos std::string::npos

class Response
{
	private:
		int _status;
		Request &_request;
		dataserver &data_server;
		location _location;
		std::string _headers;
		std::string _body;
		std::string _index_path;
		std::string _autoIndex_page;
		std::string _dir_path;
		bool _autoIndex;
		bool _not_found;
		bool _is_location;
		int _port;
		std::map<int, std::string> _errors;
		std::vector<std::string> _dir_content;
		std::string _redirected_location;
		std::string _cgi_body;
	public:
	Response(dataserver &server,Request &request,int port);
	~Response();
	void	init_response();
	void	generate_response();
	void	get_method();
	void	post_method();
	void	delete_method();
	void	build_header();
	void	read_file(std::string file_path);
	std::string getContentType();

};
#endif