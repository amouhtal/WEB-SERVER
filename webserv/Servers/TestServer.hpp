
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
			std::multimap<std::string,std::string> req;

			std::string		get_url()
			{
				std::ifstream myfile(url);
				std::string buff,line;

				while(getline(myfile, buff))
				{
					line = line + buff + "\n";
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
				while ((leng = buff.find('\n')) != std::string::npos)
				{
					str = buff.substr(0,leng);
					req.insert(std::make_pair<std::string,std::string>(str.substr(0,str.find(' ')),str.substr(str.find(' ') +1,str.length())));
					buff = buff.substr(leng+1,buff.length());
				}
				
				// std::cout <<"method : " <<method << " url : " << url << " protocol_version : " <<protocol_version <<std::endl;
				// std::cout <<"==============================" <<std::endl;
				// std::map<std::string,std::string>::iterator ite = req.end();
				// for (std::map<std::string,std::string>::iterator it = req.begin(); it != ite; it++)
				// {
				// 	std::cout<<"first :" << it->first<< " : " << it->second <<std::endl;
				// }
				
				// std::cout <<"==============================" <<std::endl;

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
				std::string hello = "HTTP/1.1 200 OK\nContent-Type: Text/html\nContent-Length: ";
				std::string file;
				file = get_url();
				std::string leng = std::to_string(file.length());
				hello = hello + leng + "\n\n" + file;
				std::cout << "|"<<hello << "|"<<std::endl;
				write(new_socket, hello.c_str(), strlen(hello.c_str()));
				close(new_socket);
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