
#ifndef TestServer_hpp
#define TestServer_hpp

#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

//  Informational 1xx
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

#endif

#include "../Request/request.hpp"
#include "../Request/response.hpp"
#include "SimapleServer.hpp"
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
namespace WS
{
	class TestServer : public SimapleServer
	{
		private :
			char buffer[30000] = {0};
			int new_socket;
			std::string method;
			std::string url;
			std::string protocol_version;
			std::multimap<std::string,std::string> req_header;
			std::multimap<std::string,std::string> req_body	;

			// void	handle_error(std::string error)
			// {
			// 	int ret;
			// 	if(error == "No such file or directory")
			// 	{
			// 		std::string hello = "HTTP/1.1 404 Not Found\nContent-Type: Text/html\nContent-Length: ";
			// 		std::string file = get_file(ret,"not_find.html");
			// 		std::string leng = std::to_string(file.length());
			// 		hello = hello + leng + "\n\n" + file;
			// 		write(new_socket, hello.c_str(), strlen(hello.c_str()));
			// 		close(new_socket);
			// 	}
			// 	else if(error == "Permission denied")
			// 	{
			// 		std::string hello = "HTTP/1.1 406 Not Acceptable\nContent-Type: Text/html\nContent-Length: ";
			// 		std::string file = get_file(ret,"not_acceptable.html");
			// 		std::string leng = std::to_string(file.length());
			// 		hello = hello + leng + "\n\n" + file;
			// 		write(new_socket, hello.c_str(), strlen(hello.c_str()));
			// 		close(new_socket);
			// 	}
			// }
			std::string		get_file(int &ret,std::string file)
			{
				// std::ifstream fin("foo.png", std::ios::in | std::ios::binary);
				// std::ostringstream oss;
				// oss << fin.rdbuf();
				// std::string data(oss.str());
				std::ifstream myfile(file,  std::ios::in | std::ios::binary);
				std::string buff,line;
				std::ostringstream my;

				my << myfile.rdbuf();

				line = my.str();
				// if(myfile.good())
				// {
				// 	// while(getline(myfile, buff))
				// 	// {
				// 	// 	line = line + buff + "\n";
				// 	// }
				// }
				// else
				// {
				// 	ret = -1;
				// 	line = strerror(errno);
				// 	return line;
				// }
				// line.pop_back();
				return line;
			}

			// void parse_request()
			// {
			// 	int leng;
			// 	std::string buff(buffer);
			// 	std::string str;

			// 	leng = buff.find('\n');
			// 	str = buff.substr(0,leng);
			// 	buff = buff.substr(leng+1,buff.length());
			// 	for (size_t i = 0; i < 3; i++)
			// 	{
			// 		leng = str.find(' ');
			// 		if(i == 0)
			// 		method = str.substr(0,leng);
			// 		if(i == 1)
			// 		url = str.substr(1,leng-1);
			// 		if(i == 2)
			// 		protocol_version = str.substr(0,leng);
			// 		if(leng != std::string::npos)
			// 		str = str.substr(leng+1,str.length());
			// 	}
			// 	while ((leng = buff.find("\r\n")) != std::string::npos)
			// 	{
			// 		str = buff.substr(0,leng);
			// 		req_header.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find(' ')),str.substr(str.find(' ') +1,str.length())));
			// 		buff = buff.substr(leng+2,buff.length());
			// 	}
			// 	if(buff[0] != '\n' && buff[0] != '\0')
			// 	{
			// 		while ((leng = buff.find("&")) != std::string::npos)
			// 		{
			// 			str = buff.substr(0,leng);
			// 			req_body.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find('=')),str.substr(str.find('=') +1,str.length())));
			// 			buff = buff.substr(leng+1,buff.length());
			// 		}
			// 		str = buff.substr(0,buff.length());
			// 		req_body.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find('=')),str.substr(str.find('=') +1,str.length())));
			// 	}
			// }
			void accepter()
			{
				struct sockaddr_in adress = get_socket()->get_adress();
				int addrlen = sizeof(adress);
				new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&adress, (socklen_t *)&addrlen);
				read(new_socket, buffer, 30000);
				Request r(buffer,150,1);
				r.parseRequest();
			}
			void handler()
			{
				// std::cout << buffer ;
			}

			void responder()
			{
				// url = "index.html";
				// 	int ret=0;
				// std::string file;
				// file = get_file(ret,url);
				// std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
				// if(ret == -1)
				// 	std::cout<< hello<<"\n";
				// std::string leng = std::to_string(file.length());
				// hello = hello + leng + "\n\n" + file;
				// write(new_socket, hello.c_str(), strlen(hello.c_str()));
				// close(new_socket);
				// if(method == "GET")
				// {
				// 	if(url[0] == '\0')
				// 		url = "index.html";
				// 	file = get_file(ret,url);
				// 	if(ret == -1)
				// 	{
				// 		handle_error(file);
				// 	}
				// 	else
				// 	{
				// 		std::string hello = "HTTP/1.1 200 OK\nContent-Type: Text/html\nContent-Length: ";
				// 		std::string leng = std::to_string(file.length());
				// 		hello = hello + leng + "\n\n" + file;
				// 		write(new_socket, hello.c_str(), strlen(hello.c_str()));
				// 		close(new_socket);
				// 	}
				// }
				// else if(method == "POST")
				// {
				// 	std::string file;
				// 	int ret=0;
				// 	file = get_file(ret,url);
				// 	if(ret == -1)
				// 	{
				// 		handle_error(file);
				// 	}
				// 	else
				// 	{
				// 	std::string hello = "HTTP/1.1 200 OK\nContent-Type: Text/html\nContent-Length: ";
				// 	std::string leng = std::to_string(file.length());
				// 	hello = hello + leng + "\n\n" + file;
				// 	write(new_socket, hello.c_str(), strlen(hello.c_str()));
				// 	close(new_socket);
				// 	}
				// }

			}

		public :
			void launch()
			{
				while (true)
				{
					std::cout << "=== WAITING ===" << std::endl;

					accepter();
					handler();
					responder();
					std::cout << "=== DONE ===" << std::endl;
				}
			}

			TestServer() : SimapleServer(AF_INET, SOCK_STREAM, 0, 800, INADDR_ANY, 10)
			{
				launch();
			}
	};
}



#endif