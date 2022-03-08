#ifndef REQUEST_HPP
#define REQUEST_HPP
//  Informational 1xx
#define CONTINUE 100
#define SWITCHINGPROTOCOL 101

// Successful 2xx
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NON AUTHORITATIVE 203
#define NO_CONTENT 204
#define RESET_CONTENT 205
#define PARTIAL_CONTENT 206

// Redirection 3xx
#define MULTIPLE_CHOICES 300
#define MOVED_PERMANENTLY 301
#define FOUND 302
#define SEE_OTHER 303
#define NOT_MODIFIED 304
#define USE_PROXY 305

// Client Error 4xx
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define PAYMENT_REQUIRED 402
#define FORBIDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define PROXY_AUTHENTCATION_REQUIRED 407
#define REQUEST_TIMEOUT 408
#define CONFLICT 409
#define GONE 410
#define LENGTH_REQUIRED 411
#define PRECONDITION_FAILED 412
#define REQUEST_ENTITY_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define UNSUPPORTED_MEDIA_TYPE 415
#define REQUEST_RANGE_NOT_SATISFIABLE 416
#define EXPECTATION_FAILED 417

// Server Error 5xx
#define INTERNAL_SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GETEWAY 502
#define SERVICE_UNABAILABLE 503
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSOIN_NOT_SUPPORTED 505
// #include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <map>
#define npos std::string::npos

class Body
{
    public:
        std::string content;
        std::string Content_Type;
        std::string Content_Disposition;
        Body():content(""),Content_Type(""),Content_Disposition("")
        {}
};


class Request
{
private:

    std::string request_string;
    std::string boundary;
    std::string url_queary;
    std::string body;
	std::string method;
	std::string url;
	std::string protocol_version;
	std::multimap<std::string,std::string> req_header;
    std::multimap<std::string,std::string> req_body	;
    std::vector<Body> body_list;
    std::vector<std::string> cookies;
    std::string bounday_body;
    int body_on;
    int status_code;
    int maxbody_size;
    bool request_error;

    std::string set_top_header(std::string &line);
    int count_words(std::string str);
    void check_header_values(std::string str);
    Body get_bodys(std::string body);
public:
    Request();

    Request(const std::string buffer, int maxbody_size);
    Request(const Request &src);
    Request &operator=(const Request &rhs);
    ~Request();

    int check_req_errors();
    void parseRequest();
    void parseBody(std::string buffer);
    int get_status();
    std::string get_method();
    std::string get_url();
    std::string get_protocol();
	std::string	get_header_value(std::string to_find);
    std::vector<Body> getBodys();
    std::string get_bounday_body();
    std::string get_body();
    std::string get_queary_string();
    std::multimap<std::string,std::string> get_header();
    void    setUrl(std::string url);

};

#endif

