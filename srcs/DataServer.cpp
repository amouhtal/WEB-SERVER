#include "../headers/DataServer.hpp"

dataserver::dataserver(){
    this->client_max_body_size = -1;
    this->server_name = "";
    this->host = "";
    this->root = "";
};

std::vector<int>dataserver::getListens()
{
    return this->index_listens;
}

void    dataserver::addListen(int listen)
{
    this->index_listens.push_back(listen);
}

void    dataserver::setListen(int listen)
{
    this->listen = listen;
}

int     dataserver::getListen()
{
    return this->listen;
}

std::string dataserver::getHost()
{
    return this->host;
}

void    dataserver::setHost(std::string &str)
{
    this->host = str;
}

void    dataserver::setServer_name(std::string &str)
{
    this->server_name = str;
}

std::string dataserver::getServer_name()
{
    return this->server_name;
}

int     dataserver::getClient_max_body_size()
{
    return this->client_max_body_size;
}

void    dataserver::setClient_max_body_size(int cmbs)
{
    this->client_max_body_size = cmbs;
}

std::map<int, std::string>  dataserver::getError_page()
{
    return this->error_pages;
}

void    dataserver::setError_page( int &nbr_error, std::string &error_bath)
{
    this->error_pages.insert(std::pair<int, std::string> (nbr_error, error_bath));
}

void    dataserver::setRoot(std::string &ptr)
{
    this->root = ptr;
}

std::string dataserver::getRoot()
{
    return this->root;
}

void    dataserver::clear_all()
{
    for (;index_listens.size();)
        this->index_listens.pop_back();
    for(;host.size();)
        this->host.pop_back();
    this->host.erase(host.begin(),host.end());
    this->host.clear();
    this->server_name.erase(server_name.begin(), server_name.end());
    this->server_name.clear();
    this->client_max_body_size = -1;
    this->error_pages.erase(error_pages.begin(), error_pages.end());
    this->error_pages.clear();
    this->root.erase(root.begin(), root.end());
    this->root.clear();
    for (size_t i = 0; i < Location.size();i++)
        Location.clear();
}

void    dataserver::addLocation(std::map<std::string, location> &sv_loc)
{
    std::map<std::string, location>::iterator it = sv_loc.begin();
    this->Location.insert(std::pair<std::string, location>(it->first, it->second));
}

void    dataserver::clearLocation(location &sv_loc)
{
    sv_loc.clearAll();
}

dataserver::~dataserver()
{
    host.clear();
    error_pages.clear();
}



void    dataserver::printServerData()
{
    std::cout << "\e[1;31mports                = \e[1;32m";
    for (size_t i = 0; i < this->index_listens.size(); i++)
        std::cout << "|" << this->index_listens[i] << "|";
    std::cout << std::endl;
    std::cout << "\e[1;31mhost                 = \e[1;32m|" << this->host << "|" << std::endl;
    std::cout << "\e[1;31mserver_name          = \e[1;32m|" << this->server_name << "|" << std::endl;
    std::cout << "\e[1;31mClient Max body size = \e[1;32m|" << this->client_max_body_size << "|" << std::endl;
    std::cout << "\e[1;31mroot                 = \e[1;32m|" << this->root << "|" << std::endl;
    std::cout << "\e[1;31mError_pages :"<<std::endl;
    for (std::map<int,std::string>::iterator it = this->error_pages.begin(); it != this->error_pages.end(); it++)
        std::cout << "\e[1;34mkey = \e[1;32m|" << it->first << "| \e[1;34mvalue = \e[1;32m|"+ it->second << "|" << std::endl;

    std::cout << "\e[1;33m///////////////////////LOACATION INFORMATION/////////////////\e[1;34m" << std::endl;
    std::vector<std::string> arr;
    for (std::map<std::string, location>::iterator it = Location.begin(); it != Location.end(); it++)
        arr.push_back(it->first);
    for (size_t i = 0; i < arr.size() ; i++)
    {
        std::cout << "\e[1;31mlocation type = |";
        std::cout << Location[arr[i]].getLocationtype() << "|" << std::endl;
        std::cout << "\e[1;34mAutoIndex     = \e[1;32m|";
        std::cout << Location[arr[i]].getL_AutoIndex() << "|" << std::endl;
        std::cout << "\e[1;34mIndex         = \e[1;32m|";
        std::cout << Location[arr[i]].getL_Index() << "|" << std::endl;
        if (Location[arr[i]].isCgi == true)
        {
            std::cout << "\e[1;34mfastCgiPass   = \e[1;32m|";
            std::cout << Location[arr[i]].getL_Fastcgi_Pass() << "|" << std::endl;
        }

        if (Location[arr[i]].isRoot == true)
        {
            std::cout << "\e[1;34mLocation_Root   = \e[1;32m|";
            std::cout << Location[arr[i]].getL_Root() << "|" << std::endl;
        }

        std::cout << "\e[1;34mupload_enable  = \e[1;32m|";
        std::cout << Location[arr[i]].get_L_upload_enb() << "|" << std::endl;
        std::cout << "\e[1;34mupload_store   = \e[1;32m|";
        std::cout << Location[arr[i]].get_L_upload_store() << "|" << std::endl;
        std::cout << "\e[1;34mReturn   = \e[1;32m|";
        std::cout << Location[arr[i]].getL_Return_nbr() << "|" << " " << "|" << Location[arr[i]].getL_Return_value() << "|"<< std::endl;
        std::map<std::string , int> test;
        test = Location[arr[i]].getL_Allowed_Methods();
        std::cout << "\e[1;34mGET           = \e[1;32m|" << test["GET"] << "|\e[1;34m, POST = \e[1;32m|" << test["POST"] << "|\e[1;34m, DELETE = \e[1;32m|" << test["DELETE"] << "|" << std::endl;
    }
}

void    dataserver::printServerALLData()
{
    std::cout << "\e[1;31mports                = \e[1;32m";
    std::cout << "|" << this->listen << "|";
    std::cout << std::endl;
    std::cout << "\e[1;31mhost                 = \e[1;32m|" << this->host << "|" << std::endl;
    std::cout << "\e[1;31mserver_name          = \e[1;32m|" << this->server_name << "|" << std::endl;
    std::cout << "\e[1;31mClient Max body size = \e[1;32m|" << this->client_max_body_size << "|" << std::endl;
    std::cout << "\e[1;31mroot                 = \e[1;32m|" << this->root << "|" << std::endl;
    std::cout << "\e[1;31mError_pages :"<<std::endl;
    for (std::map<int,std::string>::iterator it = this->error_pages.begin(); it != this->error_pages.end(); it++)
        std::cout << "\e[1;34mkey = \e[1;32m|" << it->first << "| \e[1;34mvalue = \e[1;32m|"+ it->second << "|" << std::endl;

    std::cout << "\e[1;33m///////////////////////LOACATION INFORMATION/////////////////\e[1;34m" << std::endl;
    std::vector<std::string> types;
    for (std::map<std::string, location>::iterator it = Location.begin(); it != Location.end(); it++)
        types.push_back(it->first);
    for (size_t i = 0; i < types.size() ; i++)
    {
        std::cout << "\e[1;31mlocation type = |";
        std::cout << Location[types[i]].getLocationtype() << "|" << std::endl;
        std::cout << "\e[1;34mAutoIndex     = \e[1;32m|";
        std::cout << Location[types[i]].getL_AutoIndex() << "|" << std::endl;
        std::cout << "\e[1;34mIndex         = \e[1;32m|";
        std::cout << Location[types[i]].getL_Index() << "|" << std::endl;
        if (Location[types[i]].isCgi == true)
        {
            std::cout << "\e[1;34mfastCgiPass   = \e[1;32m|";
            std::cout << Location[types[i]].getL_Fastcgi_Pass() << "|" << std::endl;
        }

        if (Location[types[i]].isRoot == true)
        {
            std::cout << "\e[1;34mLocation_Root   = \e[1;32m|";
            std::cout << Location[types[i]].getL_Root() << "|" << std::endl;
        }
        std::cout << "\e[1;34mupload_enable  = \e[1;32m|";
        std::cout << Location[types[i]].get_L_upload_enb() << "|" << std::endl;
        std::cout << "\e[1;34mupload_store   = \e[1;32m|";
        std::cout << Location[types[i]].get_L_upload_store() << "|" << std::endl;

       std::cout << "\e[1;34mReturn   = \e[1;32m|";
        std::cout << Location[types[i]].getL_Return_nbr() << "|" << " " << "|" << Location[types[i]].getL_Return_value() << "|"<< std::endl;


        std::map<std::string , int> test;
        test = Location[types[i]].getL_Allowed_Methods();
        std::cout << "\e[1;34mGET           = \e[1;32m|" << test["GET"] << "|\e[1;34m, POST = \e[1;32m|" << test["POST"] << "|\e[1;34m, DELETE = \e[1;32m|" << test["DELETE"] << "|" << std::endl;
    }
}