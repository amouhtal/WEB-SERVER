#include "../headers/response.hpp"

Response::Response(dataserver &server,Request &request,int port) : _request(request),data_server(server)
{
	_status = _request.get_status();
	_headers = "HTTP/1.1 ";
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
void Response::build_error_header(int status)
{
	time_t rawTime;
	std::string tm;

	this->_headers.clear();
	time(&rawTime);
	tm = ctime(&rawTime);
	tm.pop_back();
	this->_headers.append(this->_request.get_protocol());
	this->_headers.append(" ");
	this->_headers.append(std::to_string(_status));
	this->_headers.append(" ");
	this->_headers.append(this->_errors[_status]);
	this->_headers.append("\r\n");
	this->_headers.append("Server: webServ\r\n");
	this->_headers.append("Date: " + tm.append(" GMT"));
	this->_headers.append("\r\n");
	this->_headers.append("Connection: " + _request.get_header_value("Connection"));
	this->_headers.append("\r\n");
	this->_headers.append("Content-Type: text/html; charset=UTF-8");
	this->_headers.append("\r\n");
	this->_headers.append("Content-Length: " + std::to_string(_body.length()));
	this->_headers.append("\r\n\r\n");
	this->_headers.append(_body);
}
void	Response::read_error_file(std::string error_path)
{
	std::ifstream file(error_path);
	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		_body = ss.str();
	}
	else
		read_default_error_file(_status);

}
void	Response::read_default_error_file(int status)
{
	std::ifstream file("/Users/amouhtal/Desktop/web-server/default_error/default_error.html");
	std::ostringstream ss;
	ss << file.rdbuf();
	_body = ss.str();
	// std::cout << _body << "|" << std::endl;
	_body.replace(_body.find("$1"), 2, std::to_string(status));
	_body.replace(_body.find("$2"), 2, _errors[status]);
	// 	// puts("here");
}
void	Response::set_error_page(int code)
{
	_status = code;
	if (data_server.getError_page().find(code) != data_server.getError_page().end())
	{
		read_error_file(data_server.getError_page()[_status]);
	}
	else
	{
	read_default_error_file(_status);
	}
	build_error_header(_status);
}
void	Response::read_file(std::string file_path)
{
	std::ostringstream streambuff;
	std::string file_to_open = data_server.getRoot() + file_path;
	// std::cout << "=>" <<file_to_open << std::endl;
	if (access(file_to_open.c_str(), F_OK) != 0)
	{
		puts("before here");
		set_error_page(NOT_FOUND);
	}
	else
	{
		if (access(file_to_open.c_str(), R_OK) == 0)
		{
			std::ifstream file(file_to_open);
			if (file)
			{
				std::ostringstream ss;
				ss << file.rdbuf();
				_body = ss.str();
			}
			else
				set_error_page(INTERNAL_SERVER_ERROR);
			file.close();
		}
		else
			set_error_page(FORBIDEN);
	}
	// if (file.good())
	// {
	// 	file.open(file_to_open, std::ios::binary);
	// 	if(file.is_open())
	// 	{
	// 		streambuff << file.rdbuf();
	// 		_body = streambuff.str();
	// 		file.close();
	// 	}
	// }
	// else
	// 	set_error_body(NOT_FOUND);
}
std::string Response::getContentType()
{
	std::string extension = _request.get_url().substr(_request.get_url().find(".") + 1);
	if (_request.get_header_value("Content-Type:").size())
		return _request.get_header_value("Content-Type");
	else if (extension.compare("html") == 0 || extension.compare("php") == 0)
		return "text/html; charset=UTF-8";
	else if (extension.compare("json") == 0)
		return "application/json";
	else if (extension.compare("ico") == 0)
		return "image/x-icon";
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
	read_file(_request.get_url());
}
std::string	Response::find_file_name(std::string dispo)
{
	std::string tmp = dispo.substr(dispo.find("filename=") + 10);
	tmp = tmp.substr(0,tmp.find("\""));
	return tmp;
}
void	Response::post_method()
{
	std::string file_dir;
	std::string buffer;
	for (size_t i = 0; i < _request.getBodys().size(); i++)
		{
			// dispoFilename = getDispContentFilename(_request.getBody()[i].contentDesp);
			// if (!isDirectory(getUploadDir()))
			// 	setErrorPage(NOT_FOUND_STATUS);
			// else
			// {
				// fileDir = getUploadDir().append(dispoFilename);
				file_dir = "/Users/amouhtal/Desktop/web-server/" + find_file_name(_request.getBodys()[i].Content_Disposition);
				if (access(file_dir.c_str(), F_OK) == 0 && access(file_dir.c_str(), W_OK) != 0)
				{
					// setErrorPage(FORBIDDEN_STATUS);
					return;
				}
				std::ofstream file(file_dir);
				std::stringstream ss(_request.getBodys()[i].content);
				while (std::getline(ss, buffer))
				{
					file << buffer.append("\n");
				}
				file.close();
				_body = "<html><head><body><div><h5>File Uploaded successfully</h5></div></body></head></html>";
			// }
		}
}
void	Response::generate_response()
{
	if (_request.get_method().compare("GET") == 0)
		get_method();
	else if (_request.get_method().compare("POST") == 0)
		post_method();
	// else if (_request.get_method().compare("POST") == 0)
	// 	delete_method();
	if (_status == OK || _status == MOVED_PERMANENTLY)
		build_header();
}
void    Response::init_response()
{
	// std::cout <<"==>"<< _status << std::endl;
	if(_status == OK)
		generate_response();
}
std::string Response::getHeader()
{
	return this->_headers;
}
Response::~Response()
{

}