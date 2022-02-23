#include "../headers/request.hpp"

Request::Request()
{
	request_string = "";
	body ="";
	method ="";
	url= "";
	protocol_version ="";
	boundary = "";
	body_on = 0;
	status_code = 200;
	maxbody_size = 150;
	request_error = 0;
	is_valid = 0;
}
int	Request::count_words(std::string str)
{
	int i =0;
	int count =0;

	while (str[i])
	{
		while (str[i] != ' ' && str[i] )
		{
			i++;
		}
		if(str[i] == ' ')
			i++;
		count++;
	}
	return count;
}
Request::Request(const std::string buffer, int maxbody_size, int is_valid)
{
	request_string = buffer;
	this->maxbody_size = maxbody_size;
	this->is_valid = is_valid;

	body ="21";
	method ="";
	url= "";
	protocol_version ="";
	url_queary = "";
	boundary = "";
	body_on = 0;
	status_code = 200;
	request_error = 0;
	req_header.insert(std::make_pair<std::string,std::string>("Connection","close"));

}

Request::Request(const Request &src)
{
	*this = src;
}

Request &Request::operator=(const Request &rhs)
{
	if (this != &rhs)
	{
		this->request_string = rhs.request_string;
		this->boundary = rhs.boundary;
		this->url_queary = rhs.url_queary;
		this->body = rhs.body;
		this->method = rhs.method;
		this->url = rhs.url;
		this->protocol_version = rhs.protocol_version;
		this->req_header =rhs.req_header;
		this->req_body = rhs.req_body;
		this->body_on = rhs.body_on;
		this->status_code = rhs.status_code;
		this->maxbody_size = rhs.maxbody_size;
		this->request_error = rhs.request_error;
		this->is_valid = rhs.is_valid;
		this->body_list = rhs.body_list;
	}
	return *this;
}
std::string Request::set_top_header(std::string &request)
{
	int leng;
	std::string line;
	std::string top_header;

	leng = request.find("\n");
	line = request.substr(0,leng-1);
	top_header = request.substr(0,leng);

	// std::cout << line <<std::endl;
	if(count_words(line)!= 3)
		throw std::runtime_error("Bad Request: Too many or too few arguments!");
	else
	{
		for (size_t i = 0; i < 3; i++)
		{
			leng = line.find(' ');
			if(i == 0)
				method = line.substr(0,leng);
			if(i == 1)
				url = line.substr(0,leng);
			if(i == 2)
				protocol_version = line.substr(0, leng-4);
			if(leng != npos)
				line = line.substr(leng + 1, line.length());
		}
		if (method != "GET" && method != "POST" && method != "DELETE")
		{
			throw std::runtime_error("Bad Request: Method Not Allowed");
		}
		else if (url.at(0) != '/')
			throw std::runtime_error("Bad Request: Absolute path required!");
		else if (protocol_version.find("HTTP") != npos)
		{
			if (protocol_version.substr(0, protocol_version.find("/")).compare("HTTP") != 0)
				throw std::runtime_error("Exception: Syntax error at : " + protocol_version);
			std::string version = protocol_version.substr(protocol_version.find("/") + 1);
			if (!std::isdigit(version[0]) || version[1] != '.' || !std::isdigit(version[2]))
				throw std::runtime_error("Exception: Syntax error at  " + protocol_version);
			else if (protocol_version.substr(protocol_version.find("/") + 1).compare("1.1") != 0)
			{
				status_code = 505;
				throw std::runtime_error("Bad Request: Wrrong HTTP version!");
			}	
		}

		if (this->url.find("?") != npos)
		{
			this->url_queary = this->url.substr(this->url.find("?") + 1);
			this->url = this->url.substr(0, this->url.find("?"));
		}
	}
	request = request.substr(request.find("\r\n") + 2,request.length());
	return (request);
}
void	Request::check_header_values(std::string str)
{
	if(str.find("Transfer-Encoding") != npos || str.find("Content-Length") != npos)
	{
	if (str.find(":") == npos || std::count(str.begin(), str.end(), ':') > 1 || str.find(": ") == npos)
	{
		throw std::runtime_error("Exception: Syntax error a: " + str);
	}
	if (str.find("Connection") != npos)
	{
		if (str.find("Connection: ") == npos)
			throw std::runtime_error("Exception: Syntax error : " + str);
	}
	}
	if (str.find("Host") != npos && str.find("Host: ") == npos)
		throw std::runtime_error("Exception: Syntax error : "+ str);
}

void    Request::parseRequest()
{
	int leng;
	std::string str;
	std::string key;
	std::string value;
	std::string buffer =request_string;
	std::pair<std::string,std::string> pair;
	// std::cout <<buffer;
	try
	{
		buffer = set_top_header(buffer);
		while ((leng = buffer.find("\r\n")) != npos)
		{
			if(leng == 0 && buffer.find("\r\n") != npos)
			{
				buffer = buffer.substr(leng+2,buffer.length());
				break;
			}
			str = buffer.substr(0,leng);
			check_header_values(str);
			if(str.find("Content-Type")!= npos && str.find("boundary") != npos)
			{
				value = str.substr(str.find("=") + 1,str.length());
				str = str.substr(0,str.find("; "));
				req_header.insert(std::make_pair<std::string,std::string>("boundary",value));
			}
			key =str.substr(0,str.find(' '));
			value = str.substr(str.find(' ') +1 , str.length());
			pair.first = key;
			pair.second = value;
			req_header.insert(pair);
			if(req_header.count("Transfer-Encoding") > 1 || req_header.count("Content-Length") > 1 || req_header.count("Host") > 1 )
				throw std::runtime_error("Execption: Duplicated Header ");
			buffer = buffer.substr(leng+2,buffer.length());
		}
		if(req_header.find("Host")->second[0] == ':')
			throw std::runtime_error("Exception: Syntax error at Host line ");
		// std::cout << "boundary :" << req_header.find("Content-type:")->second <<"|"<< std::endl;
		std::multimap<std::string,std::string>::iterator it;
		if((it = req_header.find("Content-Length:") )!= req_header.end())
		{
			for (size_t i = 0; i < it->second.size(); i++)
			{
				if (!std::isdigit(it->second[i]))
					throw std::runtime_error("Exception: Syntax error at : " + it->first + it->second);
			}
			try
			{
				if (std::stoi(it->second) > maxbody_size)
				{
					status_code = 413;
					throw std::runtime_error("Execption: Request Entity Too Large");
				}
			}
			catch (const std::exception &e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		// std::cout <<"|"<<buffer<<"|"<<std::endl;
		// std::cout << "=======================\n";
		// std::cout << method << std::endl; 
		// for(std::multimap<std::string ,std::string>::iterator it = req_header.begin(); it != req_header.end() ; it++)
		// {
		// 	std::cout << "key : " << it->first << "\t" <<"value : " << it->second <<std::endl;
		// }
		// std::cout << "=======================\n"; 
		parseBody(buffer);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	check_req_errors();
}
int Request::check_req_errors()
{
	if (status_code == 200)
	{
		for (size_t i = 0; i < method.size(); i++)
		{
			if (std::islower(method[i]))
			{
				status_code = 400;
				return this->status_code;
			}
		}
		if (!method.size())
			this->status_code = 400;
		else if (this->protocol_version.compare("HTTP/1.1") != 0 && status_code == 200)
		{
			// setStartLineVal("protocol", "HTTP/1.1");
			protocol_version = "HTTP/1.1";
			this->status_code = 400;
		}
		else if (this->method.compare("GET") != 0 && this->method.compare("POST") != 0 && this->method.compare("DELETE") != 0)
			this->status_code = 501;
		else if (this->method.compare("POST") == 0 && this->req_header.count("Content-Length:") < 1 && this->req_header.count("Transfer-Encoding:") < 1 )
			this->status_code = 400;
		else if (this->req_header.count("Content-Length") > 0 && !body_on && status_code != 413)
			this->status_code = 400;
		else if (!url.size() || (url.size() && url[0] != '/'))
			this->status_code = 400;
		else if (req_header.count("Host:") < 1)
			this->status_code = 400;
		if (this->status_code == 400)
		{
			req_header.insert(std::make_pair<std::string,std::string>("Connection","close"));
		}
	}
	return this->status_code;
}


Body Request::get_bodys(std::string body)
{
	std::string str;
	int leng;
	Body tmp_body;
	
	// while ((leng = body.find("\n")) == 0)
	// std::cout << body<< "|" <<std::endl;
	while (body.length() != 0)
	{
		leng = body.find('\n');
		if(leng > 0)
		{
		str = body.substr(0, leng);
		if(str.find("Content-Disposition:") == 0 )
			tmp_body.Content_Disposition = str.substr(str.find(":")+2, str.length());
		else if(str.find("Content-Type:") == 0 )
			tmp_body.Content_Type = str.substr(str.find(":") + 2, str.length());
		else
			tmp_body.content += str + "\n";
		}
		body = body.substr(leng+1, body.length());
	}
	// std::cout << tmp_body.content << std::endl;
	// std::cout << tmp_body.Content_Disposition << std::endl;
	// std::cout << tmp_body.Content_Type << std::endl;
	tmp_body.content.erase(0,1);
	return(tmp_body);
}

std::string retf(std::string body, std::string boundary)
{
	std::string newBody = "";

	std::string hex_string;
	int start;
	int fin;

	while (newBody.find(boundary+ "--") == npos)
	{
		try
		{
			start = body.find("\r\n") + 2;
			hex_string = body.substr(0, body.find("\r\n"));	
			fin = stoi(hex_string, 0, 16);
			body = body.substr(start);
			newBody += body.substr(0, fin);
			body = body.substr(fin + 2);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return newBody;
}

void	Request::parseBody(std::string buffer)
{
	int len = 0;
	std::string str;
	std::string tmp;
	std::string body_tmp;
	size_t start;
	size_t end;

	body_tmp =buffer;
	if(req_header.count("boundary") > 0)
	{
		boundary = "--" + req_header.find("boundary")->second;
		if(req_header.count("Transfer-Encoding:") >= 1 &&(req_header.find("Transfer-Encoding:")->second.find("chunked") != npos))
			body_tmp = retf(buffer, boundary);
		start = body_tmp.find(boundary) + boundary.length();
		end = body_tmp.length();
		body_tmp = body_tmp.substr(start, end);
		while (body_tmp != "--\r")
		{
			tmp = body_tmp.substr(1, body_tmp.find(boundary)-1);
			body_list.push_back(get_bodys(tmp));
			if(body.find(boundary + "--\r") == body.find(boundary))
				break;
			body_tmp = body_tmp.substr(body_tmp.find(boundary) + boundary.length(),body_tmp.length());
		}
	}
	else
	{
		std::string body = "";
		int leng;
		if(buffer.find("\n") == npos)
			this->body = buffer;
		else
		{
			while (buffer.length() != 0)
			{
				str = buffer.substr(0,buffer.find("\n"));
				if (str.back() == '\r')
					str.pop_back();
				body.append(str);
				buffer = buffer.substr(buffer.find("\n") + 1,buffer.length());
			}
		}
		if(this->body.size())
			body_on++;
	}
	// std::cout << "=========================\n";
	// for(std::vector<Body>::iterator it= body_list.begin() ; it != body_list.end();it++)
	// {
	// 	std::cout << (*it).content << std::endl;
	// 	std::cout << (*it).Content_Disposition << std::endl;
	// 	std::cout << (*it).Content_Type << std::endl;
	// }
	// std::cout << "=========================\n";
}

int	Request::get_status()
{
	return (this->status_code);
}
std::string	Request::get_method()
{
	return (this->method);
}
std::string	Request::get_url()
{
	return (this->url);
}
std::string	Request::get_protocol()
{
	return (this->protocol_version);
}
std::string	Request::get_header_value(std::string to_find)
{
	return (req_header.find(to_find)->second);
}

std::vector<Body> Request::getBodys()
{
	return this->body_list;
}