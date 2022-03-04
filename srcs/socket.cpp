#include "../headers/socket.hpp"
#include "../headers/server.hpp"

namespace SERVER
{

    void ASOCKET::SetupSocket(int port)
    {

        _port = port;
        CreatSocket();
        BindSocket();
        ListenSocket();
    }

    void ASOCKET::CreatSocket()
    {

        if ((_masterSockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            perror("[ERROR] in socket !");

        if (fcntl(_masterSockFD, F_SETFL, O_NONBLOCK) == -1)
            perror("[ERROR] in fcntl !");

        int opt = 1;
        if (setsockopt(_masterSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
            perror("[ERROR] in setsockopt !");
    }

    void ASOCKET::BindSocket()
    {
        std::memset(&_Adrress, 0, sizeof(_Adrress));
        _addrLen = sizeof(_Adrress);
        _Adrress.sin_family = AF_INET;
        _Adrress.sin_port = htons(_port);
        _Adrress.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(_masterSockFD, (struct sockaddr *)&_Adrress, sizeof(_Adrress)) == -1)
            perror("[ERROR] in bind !");
    }

    void ASOCKET::ListenSocket()
    {
        if (listen(_masterSockFD, 0) == -1)
            perror("[ERROR] in listen !");
        FD_SET(_masterSockFD, &_masterRFDs);

        _maxSockFD = (_masterSockFD > _maxSockFD) ? _masterSockFD : _maxSockFD;
        _masterSockFDs.push_back(_masterSockFD);
        std::cout << "Sock : " << _masterSockFD << std::endl;
    }

}