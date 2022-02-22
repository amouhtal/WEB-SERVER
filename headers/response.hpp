#ifndef RESPONSE_HPP
#define RESPONSE_HPP

// #include <sys/socket.h>
#include"request.hpp"
#include "DataServer.hpp"
#include "location.hpp"
#include <fstream>
#include <sstream>
#define CONTINUE 100
#define SWITCHINGPROTOCOL 101

// Successful 2xx
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NON AUTHORITATIVE 203
#define NO_CONTENT 204
#define RESET_CONTENT 205
#define PARTIAL_CONTENT 206

// Redirection 3xx
#define MULTIPLE_CHOICES 300
#define MOVED_PERMANENTLY 301
#define FOUND 302
#define SEE_OTHER 303
#define NOT_MODIFIED 304
#define USE_PROXY 305

// Client Error 4xx
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define PAYMENT_REQUIRED 402
#define FORBIDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define PROXY_AUTHENTCATION_REQUIRED 407
#define REQUEST_TIMEOUT 408
#define CONFLICT 409
#define GONE 410
#define LENGTH_REQUIRED 411
#define PRECONDITION_FAILED 412
#define REQUEST_ENTITY_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define UNSUPPORTED_MEDIA_TYPE 415
#define REQUEST_RANGE_NOT_SATISFIABLE 416
#define EXPECTATION_FAILED 417

// Server Error 5xx
#define INTERNAL_SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GETEWAY 502
#define SERVICE_UNABAILABLE 503
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSOIN_NOT_SUPPORTED 505
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
	// Response(){}
	~Response();
	void	init_response();
	void	generate_response();
	void	get_method();
	void	post_method();
	void	delete_method();
	void	build_header();
	void	read_file(std::string file_path);
	std::string getContentType();
	std::string getHeader();
	std::string find_file_name(std::string dispo);
	void	set_error_page(int code);
	void	read_error_file(std::string error_path);
	void	read_default_error_file(int status);


};
#endif