#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
    std::ifstream file("html_pages/not_find.html");
	std::ostringstream ss;
    std::string _body;
	ss << file.rdbuf();
	_body = ss.str();
	std::cout << _body << "|" << std::endl;
}