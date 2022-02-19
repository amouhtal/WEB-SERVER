#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "library.hpp"
#include "DataServer.hpp"

class dataserver;
class location
{
    friend class dataserver;

private:
    int autoindex;
    std::string index;
    std::map<std::string, int> allowed_methode;
    std::string fastcgi_pass;
    std::string Locationtype;
    // std::string type;
public:
    location();
    ~location();
    void setLocationtype(std::string);
    void setL_Type(std::string);
    void setL_AutoIndex(int);
    void setL_Index(std::string);
    void setL_Fastcgi_Pass(std::string);
    void setL_Allowed_Methods(std::map<std::string, int>);
    void set_L_type(std::string);
    void clearAll();
    int getL_AutoIndex();
    std::string getLocationtype();
    std::string get_L_type();
    std::string getL_Index();
    std::string getL_Type();
    std::string getL_Fastcgi_Pass();
    std::map<std::string, int> getL_Allowed_Methods();
};

#endif