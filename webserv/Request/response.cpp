#include "response.hpp"

Response::Response(dataserver &server,Request &request,int port) : _request(request),data_server(server)
{
	_status = _request.get_status();
	_headers = "HTTP/1,1 ";
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

void	Response::read_file(std::string file_path)
{
	std::ifstream file;
	std::ostringstream streambuff;
	file.open("/Users/amouhtal/Desktop/web-serv/webserv/among.mp4", std::ios::binary);
	if (file.is_open())
	 {
		streambuff << file.rdbuf();
		_body = streambuff.str();
		file.close();
     }
}
std::string Response::getContentType()
{
	if (_request.get_header_value("Content-Type").size())
		return _request.get_header_value("Content-Type");
	else if (_request.get_method().substr(_request.get_method().find(".") + 1).compare("html") == 0 || _request.get_method().substr(_request.get_method().find(".") + 1).compare("php") == 0)
		return "text/html; charset=UTF-8";
	else if (_request.get_method().substr(_request.get_method().find(".") + 1).compare("json") == 0)
		return "application/json";
	else
		return "text/plain";
}
void	Response::build_header()
{
	time_t rawTime;
	std::string tm;

	time(&rawTime);
	tm = ctime(&rawTime);
	tm.pop_back();
	this->_headers.append(std::to_string(_status));
	this->_headers.append(" ");
	this->_headers.append(this->_errors[_status]);
	this->_headers.append("\r\n");
	this->_headers.append("Server: webServ\r\n");
	this->_headers.append("Date: " + tm.append(" GMT"));
	this->_headers.append("\r\n");
	this->_headers.append("Connection: " + _request.get_header_value("Connection"));
	this->_headers.append("\r\n");
	this->_headers.append("Content-Type: " + getContentType());
	if (_request.get_header_value("Transfer-Encoding").size())
	{
		this->_headers.append("\r\n");
		this->_headers.append("Transfer-Encoding: " + _request.get_header_value("Transfer-Encoding"));
	}
	else
	{
		this->_headers.append("\r\n");
		this->_headers.append("Content-Length: " + std::to_string(_body.length()));
	}
	this->_headers.append("\r\n\r\n");
	this->_headers.append(_body);
}
void	Response::get_method()
{
	read_file(_request.get_method());
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
		generate_response();
}