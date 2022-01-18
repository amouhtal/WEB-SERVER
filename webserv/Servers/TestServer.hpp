
#ifndef TestServer_hpp
#define TestServer_hpp

#include "SimapleServer.hpp"
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>

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

			void	handle_error(std::string error)
			{
				int ret;
				if(error == "No such file or directory")
				{
					std::string hello = "HTTP/1.1 404 Not Found\nContent-Type: Text/html\nContent-Length: ";
					std::string file = get_file(ret,"not_find.html");
					std::string leng = std::to_string(file.length());
					hello = hello + leng + "\n\n" + file;
					write(new_socket, hello.c_str(), strlen(hello.c_str()));
					close(new_socket);
				}
				else if(error == "Permission denied")
				{
					std::string hello = "HTTP/1.1 406 Not Acceptable\nContent-Type: Text/html\nContent-Length: ";
					std::string file = get_file(ret,"not_acceptable.html");
					std::string leng = std::to_string(file.length());
					hello = hello + leng + "\n\n" + file;
					write(new_socket, hello.c_str(), strlen(hello.c_str()));
					close(new_socket);
				}
			}
			std::string		get_file(int &ret,std::string file)
			{
				std::ifstream myfile(file);
				std::string buff,line;

				if(myfile.good())
				{
					while(getline(myfile, buff))
					{
						line = line + buff + "\n";
					}
				}
				else
				{
					ret = -1;
					line = strerror(errno);
					return line;
				}
				// line.pop_back();
				return line;
			}

			void parse_request()
			{
				int leng;
				std::string buff(buffer);
				std::string str;

				leng = buff.find('\n');
				str = buff.substr(0,leng);
				buff = buff.substr(leng+1,buff.length());
				for (size_t i = 0; i < 3; i++)
				{
					leng = str.find(' ');
					if(i == 0)
					method = str.substr(0,leng);
					if(i == 1)
					url = str.substr(1,leng-1);
					if(i == 2)
					protocol_version = str.substr(0,leng);
					if(leng != std::string::npos)
					str = str.substr(leng+1,str.length());
				}
				while ((leng = buff.find("\r\n")) != std::string::npos)
				{
					str = buff.substr(0,leng);
					req_header.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find(' ')),str.substr(str.find(' ') +1,str.length())));
					buff = buff.substr(leng+2,buff.length());
				}
				if(buff[0] != '\n' && buff[0] != '\0')
				{
					while ((leng = buff.find("&")) != std::string::npos)
					{
						str = buff.substr(0,leng);
						req_body.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find('=')),str.substr(str.find('=') +1,str.length())));
						buff = buff.substr(leng+1,buff.length());
					}
					str = buff.substr(0,buff.length());
					req_body.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find('=')),str.substr(str.find('=') +1,str.length())));
				}
			}
			void accepter()
			{
				struct sockaddr_in adress = get_socket()->get_adress();
				int addrlen = sizeof(adress);
				new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&adress, (socklen_t *)&addrlen);
				read(new_socket, buffer, 30000);
				parse_request();
			}
			void handler()
			{
				std::cout << buffer << std::endl;
			}

			void responder()
			{
				std::string file;
				int ret=0;
				file = get_file(ret,url);
				if(ret == -1)
				{
					handle_error(file);
				}
				else
				{
					std::string hello = "HTTP/1.1 200 OK\nContent-Type: Text/html\nContent-Length: ";
					std::string leng = std::to_string(file.length());
					hello = hello + leng + "\n\n" + file;
					write(new_socket, hello.c_str(), strlen(hello.c_str()));
					close(new_socket);
				}

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

			TestServer() : SimapleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
			{
				launch();
			}
	};
}



#endif