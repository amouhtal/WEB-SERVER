
#include "TestServer.hpp"
#include <fstream>
#include <iostream>
int main()
{
	WS::TestServer t;
}







// int	count_words(std::string str)
// {
// 	int i =0;
// 	int count =0;

// 	while (str[i])
// 	{
// 		while (str[i] != ' ' && str[i] )
// 		{
// 			i++;
// 		}
// 		if(str[i] == ' ')
// 			i++;
// 		count++;
// 	}
// 	return count;
// }
// int test(int i)
// {
// 	if(i == 0)
// 	{
// 		throw std::runtime_error("Execption: Duplicated Header ");
// 		return 0;
// 	}
// 	return(1);
// }
// int main()
// {
// 	int i = 0;
// 	int ret = 200;
// 	try
// 	{
// 		ret = test(i);
// 		std::cout <<ret<<std::endl;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << ret <<'\n';
// 	}
	
// }