#include "../headers/response.hpp"
// #include "cgi.cpp"
std::string LaunchCGI();

Response::Response(dataserver &server,Request &request,int port) : _request(request),data_server(server)
{
	_status = _request.get_status();
	_headers = "HTTP/1.1 ";
	_body = "";
	_index_path = "";
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
	std::ifstream file("/Users/mel-hamr/Desktop/web-server/default_error/default_error.html");
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
bool	Response::is_cgi()
{
	for (std::map<std::string,location>::iterator i = data_server.Location.begin(); i != data_server.Location.end(); i++)
	{
		if ((i->first.find("php") != npos || i->first.find("py") != npos) && _request.get_url().find(i->second.getLocationExtention()) != npos) 
		{
			if(!i->second.isCgi)
				return false;
			else
				return true;
		}
	}
	return false;
}
bool Response::is_directory(const std::string &path)
{
	std::string s = path;
	DIR *r;

	if ((r = opendir(s.c_str())))
	{
		closedir(r);
		return true;
	}
	return false;
}

void	Response::read_file(std::string file_path)
{
	std::ostringstream streambuff;
	// exit(1);
	if (access(file_path.c_str(), F_OK) != 0)
	{
    set_error_page(NOT_FOUND);
	}
	else
	{
		if (access(file_path.c_str(), R_OK) == 0)
		{
			std::ifstream file(file_path);
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
}
std::string	Response::getHtmlCode()
{
	std::string htmlcode = "<!DOCTYPE html>\n\
	<html>\n\
		<head>\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
		<title>WEBSERV</title>\n\
		<style>\n\
			.container{margin:0;top:0;}\n\
			.div{font-size: 2.5rem;text-align: center;margin-top: 10%;color:black;}\n\
		</style>\n\
		</head>\n\
		<body>\n\
			<div class=\"container\">\n\
				<div class=\"div\">\n\
					<h1>this is my web-server</h1>\n\
				</div>\n\
			</div>\n\
		</body>\n\
	</html>";

	return htmlcode;
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
	else if (extension.compare("jpeg") == 0)
		return "image/jpeg";
	else if (extension.compare("jpg") == 0)
		return "image/jpg";
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
	this->_headers.append("Connection: " + _request.get_header_value("Connection:"));
	// this->_headers.append("\r\n");
	// this->_headers.append("Content-Type: " + getContentType());
	if (_request.get_header_value("Transfer-Encoding:").size())
	{
		this->_headers.append("\r\n");
		this->_headers.append("Transfer-Encoding: " + _request.get_header_value("Transfer-Encoding:"));
	}
	else
	{
		this->_headers.append("\r\n");
		this->_headers.append("Content-Length: " + std::to_string(_body.length()));
	}
	this->_headers.append("\r\n\r\n");
	this->_headers.append(_body);

	
}
std::string Response::autoindex_run(std::string rooted_path)
{
	DIR *directory = opendir(rooted_path.c_str());
	struct dirent *en;
	std::string fileName;
	std::string _autoIndexPage;
	_autoIndexPage = "<!DOCTYPE html>\n<html lang=\"en\">\n\
	<head>\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
		<title>AUTO INDEX</title>\n\
		</head>\n\
		<body>\n\
			<div style=\"margin-left: 5%; margin-top:10%;\">\n\
			<hr>\n";

	if (directory)
	{
		while ((en = readdir(directory)) != nullptr)
		{
			fileName = en->d_name;
			if (en->d_type == DT_DIR)
				fileName.append("/");
			_autoIndexPage.append("\t\t\t<p><a href=\"" + fileName + "\">" + fileName + "</a></p>\n");
		}
		closedir(directory);
	}
	_autoIndexPage += "\
				<hr>\n\
			</div>\n\
		</body>\n\
	</html>\n";
	return _autoIndexPage;
}
std::string	Response::get_root()
{
	if(_LocExist&& _location.isRoot)
		return _location.getL_Root();
	else
		return data_server.getRoot();
}
void	Response::get_method()
{
	std::string rooted_path = get_root()+ _request.get_url();
	if(_LocExist)
	{
		if( is_directory(rooted_path) == true)
		{
			if(_location.getL_AutoIndex() == false)
			{
				if(_location.getL_Index().size())
				{
					_request.setUrl(_location.getL_Index());
					if(rooted_path[rooted_path.length()-1] != '/' && _location.getL_Index()[0] != '/')
						rooted_path =rooted_path +"/"+ _location.getL_Index();
					else if(rooted_path[rooted_path.length()-1] == '/' &&_location.getL_Index()[0] == '/')
					{
						rooted_path.pop_back();
						rooted_path =rooted_path + _location.getL_Index();
					}
					else
						rooted_path =rooted_path + _location.getL_Index();
					std::cout << rooted_path << std::endl;
					read_file(rooted_path);
				}
				else
				{
					_body = getHtmlCode();
					_request.setUrl(_request.get_url() + ".html");
				}
			}
			else if(_location.getL_AutoIndex() == true)
				_body = autoindex_run(rooted_path);
		}
		else
			read_file(rooted_path);
	}
	else
		read_file(rooted_path);
}
std::string	Response::find_file_name(std::string dispo)
{
	std::string tmp = dispo.substr(dispo.find("filename=") + 10);
	tmp = tmp.substr(0,tmp.find("\""));
	return tmp;
}
std::string Response::get_upload_path()
{
	
	if(_location.get_L_upload_store().size())
	{
		std::string upload;
		if(_location.get_L_upload_store()[0] != '/')
			upload = '/' + _location.get_L_upload_store().substr(1);
		else
			upload = _location.get_L_upload_store().substr(1);
		return get_root() + upload;
	}
	return get_root();
}

void	Response::post_method()
{
	std::string file_path;
	std::string buffer;
	for (size_t i = 0; i < _request.getBodys().size(); i++)
	{
		if(!_location.get_L_upload_enb())
			set_error_page(UNAUTHORIZED);
		else
		file_path = get_upload_path();
			std::cout<< "->" << _location.get_L_upload_store() << "<-"<<std::endl;
			if (is_directory(file_path) == false)
				set_error_page(NOT_FOUND);
			else
			{
				std::string dispo= file_path + '/' + find_file_name(_request.getBodys()[i].Content_Disposition);
				if (access(dispo.c_str(), F_OK) == 0 && access(dispo.c_str(), W_OK) != 0)
				{
					set_error_page(FORBIDEN);
					return;
				}
				std::ofstream file(dispo);
				std::stringstream ss(_request.getBodys()[i].content);
				while (std::getline(ss, buffer))
				{
					file << buffer.append("\n");
				}
				file.close();
				_body = "<html><head><body><div><h5>File Uploaded successfully</h5></div></body></head></html>";
			}
	}
}
bool Response::find_location()
{
	std::string uri = _request.get_url();
    if (uri[0] != '/')
        uri = "/" + uri; 
    std::map<std::string, location>::iterator it = data_server.Location.begin();//Location.begin();
    // looking for exact match
    for (; it != data_server.Location.end(); it++) {
        if (it->first == uri) {
			_location = it->second;
            return (true);
        }
    }

    // no exact match found: looking for the longest match
    it = data_server.Location.begin();
    size_t pos = uri.length();

    while (pos != 0) {
        pos = uri.rfind("/", pos - 1);
        for (; it != data_server.Location.end(); it++) {
            // if (strncmp(uri.c_str(), it->first.c_str(), pos) == 0)
            if (it->first.compare(uri.substr(0,pos)) == 0)
            {
				_location = it->second;
				return true;
			}
			if (it->first == "/")
            {
				_location = it->second;
				return true;
			}
        }
    }
    return (false);
}
void	Response::delete_method()
{
	std::string directoryPath =get_root() + _request.get_url();

	if (is_directory(directoryPath))
		set_error_page(NOT_FOUND);
	else
	{
		if (access(directoryPath.c_str(), F_OK) != 0)
			set_error_page(NOT_FOUND);
		else
		{
			if (access(directoryPath.c_str(), W_OK) == 0)
			{
				if (std::remove(directoryPath.c_str()) != 0)
					set_error_page(INTERNAL_SERVER_ERROR);
			}
			else
				set_error_page(FORBIDEN);
		}
	}
}
void	Response::generate_response()
{
	_LocExist = find_location();
	if(_LocExist && is_cgi())
	{
		std::string filePath = get_root() + _request.get_url();
		if (access(filePath.c_str(), F_OK) == 0)
		{
			if (access(filePath.c_str(), R_OK) == 0 && access(filePath.c_str(), W_OK) == 0)
			{
				_body = LaunchCGI();
				// parseCgiResponse(_body);
				std::cout << _body << std::endl;
			}
			else
				set_error_page(FORBIDEN);
		}
		else
			set_error_page(NOT_FOUND);
	}
	else
	{
		if (_request.get_method().compare("GET") == 0)
		{
			get_method();
		}
		else if (_request.get_method().compare("POST") == 0)
			post_method();
		else if (_request.get_method().compare("DELETE") == 0)
			delete_method();
		if (_status == OK || _status == MOVED_PERMANENTLY)
		{
			build_header();
		}
	}
}
void    Response::init_response()
{
	
	if(_status == OK)
	{
		generate_response();
		std::cout  << "================REQUEST================" <<std::endl;
		std::cout << _headers.c_str() << std::endl;
		std::cout  << "=======================================" <<std::endl;
	}
}
std::string Response::getHeader()
{
	return this->_headers;
}
Response::~Response()
{
		_status = 200;
	_headers.clear();
	_body.clear();
	_index_path.clear();
	_autoIndex_page.clear();
	_dir_path.clear();
	_autoIndex = false;
	_not_found = false;
	_is_location = false;
	_port = 0;
	_redirected_location.clear();
	_cgi_body.clear();
}