#include "../headers/location.hpp"

location::location(){
    this->autoindex = -1;
    this->upload_enable = -1;
    this->upload_store = "";
    this->fastcgi_pass = "";
    this->Locationtype = "";
    this->index = "";
    this->return_nbr = -1;
    this->return_value = "";
    this->L_Root = "";
    this->isCgi = false;
    this->isRoot = false;
};

void location::setL_Root(std::string L_Rt)
{
    this->L_Root = L_Rt;
}

std::string  location::getL_Root()
{
    return this->L_Root;
}

void location::setL_Index(std::string index)
{
    this->index = index;

}
std::string location::getL_Index()
{
    return this->index;
}

void location::setL_AutoIndex(int autoIndex)
{
    this->autoindex = autoIndex;
}

int location::getL_AutoIndex()
{
    return this->autoindex;
}

void location::setLocationtype(std::string l_type)
{
    this->Locationtype = l_type;
}
std::string location::getLocationtype()
{
    return this->Locationtype;
}

void location::setLocationExtention(std::string l_exten)
{
    this->LocationExtention = l_exten;
}

std::string location::getLocationExtention()
{
    return this->LocationExtention;
}

void    location::set_L_upload_store(std::string store)
{
    this->upload_store = store;
}

std::string location::get_L_upload_store()
{
    return this->upload_store;
}

void    location::set_L_upload_enb(int enb)
{
    this->upload_enable = enb;
}

int location::get_L_upload_enb()
{
    return this->upload_enable;
}


void location::setL_Fastcgi_Pass(std::string f_pass)
{
    this->fastcgi_pass = f_pass;
}
std::string location::getL_Fastcgi_Pass()
{
    return this->fastcgi_pass;
}

void location::setL_Allowed_Methods(std::map<std::string, int> all_methode)
{
    this->allowed_methode = all_methode;
}


std::map<std::string, int> location::getL_Allowed_Methods()
{
    return this->allowed_methode;
}

void        location::setL_Return(int nbr, std::string str)
{
    if (nbr <= 0)
        throw std::runtime_error("\e[1;31mChek Your Return number in [Location /return]\e[1;32m");
    this->return_value = str;
    this->return_nbr = nbr;
}

std::string location::getL_Return_value()
{
    return this->return_value;
}

int location::getL_Return_nbr()
{
    return this->return_nbr;
}


void    location::clearAll()
{
    this->Locationtype.erase(0, strlen(this->Locationtype.c_str()));
    this->Locationtype.clear();
    this->autoindex = false;
    this->index.erase(0, strlen(this->index.c_str()));
    this->index.clear();
    this->fastcgi_pass.erase(0, strlen(this->fastcgi_pass.c_str()));
    this->fastcgi_pass.clear();
    this->allowed_methode.clear();
}

location::~location(){};