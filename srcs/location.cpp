#include "../headers/location.hpp"

location::location(){};


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

void location::setLocationtype(std::string l_extention)
{
    this->Locationtype = l_extention;
}
std::string location::getLocationtype()
{
    return this->Locationtype;
}

// void location::set_L_type(std::string l_type)
// {
//     this->type = l_type;
// }
// std::string location::get_L_type()
// {
//     return this->type;
// }

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
