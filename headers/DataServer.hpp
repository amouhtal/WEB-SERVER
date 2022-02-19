#ifndef DATASERVER_HPP
#define DATASERVER_HPP
#include "location.hpp"
#include "parssingfile.hpp"

class dataserver
{
    friend class FileParss;

private:
    int client_max_body_size;
    int listen;
    std::string host;
    std::string server_name;
    std::vector<int> index_listens;
    std::map<int, std::string> error_pages;
    std::string root;

public:
    std::map<std::string, location> Location;
    dataserver();
    ~dataserver();

    std::vector<int> getListens();
    void addListen(int);
    void setListen(int);
    void setHost(std::string &);
    void setServer_name(std::string &);
    void setClient_max_body_size(int);
    void setError_page(int &, std::string &);
    void setRoot(std::string &);
    void clearLocation(location &sv_loc);
    void addLocation(std::map<std::string, location> &sv_loc);
    void clear_all();

    void printServerData();
    void printServerALLData();

    std::string getHost();
    std::string getServer_name();
    std::map<int, std::string> getError_page();
    std::string getRoot();
    int getClient_max_body_size();
    int getListen();
};

#endif