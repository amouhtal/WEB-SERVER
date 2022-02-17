#include "response.hpp"

Response::Response(dataserver &server,Request &request,int port) : _request(request),data_server(server)
{
	_status = _request.get_status();
	_headers = "";
	_body = "";
	_index_path = "";
	_autoIndex = "";
	_autoIndex_page = "";
	_dir_path = "";
	_autoIndex = false;
	_not_found = false;
	_is_location = false;
	_port = 0;
	_redirected_location = "";
	_cgi_body = "";
	this->_errors[200] = "OK";
	this->_errors[301] = "Moved Permanently";
	this->_errors[307] = "Temporary Redirect";
	this->_errors[400] = "Bad Request";
	this->_errors[403] = "Forbidden";
	this->_errors[404] = "Not Found";
	this->_errors[405] = "Not Allowed";
	this->_errors[411] = "Length Required";
	this->_errors[413] = "Request Entity Too Large";
	this->_errors[414] = "Uri Too Long";
	this->_errors[415] = "Unsupported Media Type";
	this->_errors[500] = "Internal Server Error";
	this->_errors[501] = "Not Implemented";
	this->_errors[502] = "Bad Gateway";
	this->_errors[504] = "Gateway Timeout";
	this->_errors[505] = "HTTP Version Not Supported";
}
void	Response::get_method()
{
	
}
void	Response::generate_response()
{
	if (_request.get_method().compare("GET") == 0)
		get_method();
	else if (_request.get_method().compare("POST") == 0)
		post_method();
	else if (_request.get_method().compare("POST") == 0)
		delete_method();
	if (_status == OK || _status == MOVED_PERMANENTLY)
		build_header();
}
void    Response::init_response()
{
	if(_status == OK)
		build_header();
}