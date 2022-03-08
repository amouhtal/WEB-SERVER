#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "library.hpp"

// class dataserver;
class location
{
    // friend class dataserver;
    private:
        int autoindex;
        int upload_enable;
        int return_nbr;
        std::string index;
        std::map<std::string, int> allowed_methode;
        std::string L_Root;
        std::string fastcgi_pass;
        std::string Locationtype;
        std::string LocationExtention;
        std::string upload_store;
        std::string return_value;
    public:
        location();
        ~location();
        bool isCgi;
        bool isRoot;
        void		setLocationtype(std::string);
        void		setLocationExtention(std::string);
        void		setL_Type(std::string);
        void 		setL_AutoIndex(int);
        void 		setL_Index(std::string);
        void 		setL_Fastcgi_Pass(std::string);
        void        setL_Root(std::string);
        void        setL_Return(int, std::string);
        void        setL_Allowed_Methods(std::map<std::string, int>);
        void        set_L_type(std::string);
        void        set_L_upload_enb(int);
        void        set_L_upload_store(std::string);
        void        clearAll();
        int 		getL_AutoIndex();
        int         get_L_upload_enb();
        std::string get_L_upload_store();
        std::string getLocationtype();
        std::string getLocationExtention();
        std::string getL_Root();
        std::string get_L_type();
        std::string getL_Index();
        std::string	getL_Type();
        std::string getL_Fastcgi_Pass();
        std::map<std::string, int> getL_Allowed_Methods();
        std::string getL_Return_value();
        int  getL_Return_nbr();
};

#endif