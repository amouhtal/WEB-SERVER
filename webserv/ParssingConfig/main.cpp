// #include "library.hpp"
#include "parssingfile.hpp"

void    printServersINFO(std::vector<dataserver> newServers)
{
    for (size_t i = 0; i < newServers.size(); i++)
    {
        std::cout<<"\e[4;36m  _____    _____   ______     __    __    _____   ______" << std::endl;
        std::cout<<" / ____\\  / ___/  (   __ \\    ) )  ( (   / ___/  (   __ \\" << std::endl;
        std::cout<<"( (___   ( (__     ) (__) )  ( (    ) ) ( (__     ) (__) )" << std::endl;
        std::cout<<" \\___ \\   ) __)   (    __/    \\ \\  / /   ) __)   (    __/ " << std::endl;
        std::cout<<"     ) ) ( (       ) \\ \\  _    \\ \\/ /   ( (       ) \\ \\  _ " << std::endl;
        std::cout<<" ___/ /   \\ \\___  ( ( \\ \\_))    \\  /     \\ \\___  ( ( \\ \\_))" << std::endl;
        std::cout<<"/____/     \\____\\  )_) \\__/      \\/       \\____\\  )_) \\__/" << std::endl;
        std::cout<<"                            ["<<i<<"]                            " << std::endl;
        std::cout << std::endl;
        newServers[i].printServerData();
    }
}

int     main(int ac, char **av)
{
    // HTTPserver webserv(ac, av);
    try
    {

        ParssFile parss(ac, av);
        //split all servers by port NB: use 'port' variable not 'ports' to get every server port
        std::vector<dataserver> newServers = parss.server/*.SplitServers()*/;

        std::cout << parss.server[0].Location["/"].getL_Index() << std::endl;
        // printServersINFO(newServers);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}