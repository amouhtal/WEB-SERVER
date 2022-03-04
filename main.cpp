// #include "library.hpp"
#include "headers/server.hpp"

void printServersINFO(std::vector<dataserver> newServers)
{
    for (size_t i = 0; i < newServers.size(); i++)
    {
        std::cout << "\e[4;36m  _____    _____   ______     __    __    _____   ______" << std::endl;
        std::cout << " / ____\\  / ___/  (   __ \\    ) )  ( (   / ___/  (   __ \\" << std::endl;
        std::cout << "( (___   ( (__     ) (__) )  ( (    ) ) ( (__     ) (__) )" << std::endl;
        std::cout << " \\___ \\   ) __)   (    __/    \\ \\  / /   ) __)   (    __/ " << std::endl;
        std::cout << "     ) ) ( (       ) \\ \\  _    \\ \\/ /   ( (       ) \\ \\  _ " << std::endl;
        std::cout << " ___/ /   \\ \\___  ( ( \\ \\_))    \\  /     \\ \\___  ( ( \\ \\_))" << std::endl;
        std::cout << "/____/     \\____\\  )_) \\__/      \\/       \\____\\  )_) \\__/" << std::endl;
        std::cout << "                            [" << i << "]                            " << std::endl;
        std::cout << std::endl;
        newServers[i].printServerData();
    }
}

int main(int ac, char **av)
{
    // HTTPserver webserv(ac, av);
    try
    {

        ParssFile parss(ac, av);
        signal(SIGPIPE, SIG_IGN);
        // int counter = 0;
        // std::vector<int> ports = parss._getAllPorts();
        // for (size_t i = 0; i < ports.size(); i++)
        //     std::cout << ports[i] << std::endl;
        std::vector<dataserver> CurrrentServer = parss.SplitServers();
        // std::cout << CurrrentServer.size() << std::endl;
        // std::cout <<CurrrentServer[0].getListen() << std::endl;

        // for (std::vector<dataserver>::iterator CurrrentServer = parss.servers.begin(); CurrrentServer != parss.servers.end(); CurrrentServer++)
        // {
        //     _ports.push_back();
        // for (std::vector<*CurrrentServer>::iterator CurrrentServer = parss.servers.begin(); CurrrentServer != parss.servers.end(); CurrrentServer++)
        // {

        // }
            SERVER::ASERVER runServer;
            runServer.launch(CurrrentServer);

        // }
        // printServersINFO(newServers);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}