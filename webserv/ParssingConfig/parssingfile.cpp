#include "parssingfile.hpp"

ParssFile::ParssFile(int ac, char **av)
{
    this->check_argument(ac, av);
    this->fill_file_content();
    this->check_bracket_brace_file();
    this->find_OpenClose_EachServer();
    this->get_elements();
}

std::string ParssFile::getFileName()
{
    return this->file_name;
}
void    ParssFile::add_server(dataserver &var)
{
    this->server.push_back(var);
}

void    ParssFile::check_argument(int ac, char **argv)
{

        if (argv[1])
            this->file_name = argv[1];
        else
            this->file_name = "../confg/config.conf";
        int i;
        std::string tmp = this->file_name;
        while ((i = tmp.find("/")) != std::string::npos)
            tmp.erase(0, i + 1);
        this->extention = &tmp[static_cast<int>(tmp.find('.') + 1)];
        if (this->extention != "conf")
            throw std::runtime_error("Cheke Your Config Extention");
}

void    ParssFile::remove_spaces(std::string &refline)
{
    while (isspace(refline.front()))
        refline.erase(0, 1);
    while (isspace(refline.back()))
        refline.erase(refline.length() - 1, 1);
}

void    ParssFile::find_OpenClose_EachServer()
{
    int i = -1;
    if (content_file.size() == 0)
            throw std::runtime_error("Error: Check your config.conf");
    if (content_file[0] != SERVER || content_file[1] != OPEN_BRACKET)
        throw std::runtime_error("Error: Check your config.conf");
    while (++i < content_file.size())
    {
        if (content_file[i] == OPEN_BRACKET)
            index_server.push_back(i);
        if (content_file[i] == CLOSE_BRACKET)
            index_server.push_back(i);
    }
}

void    ParssFile::delete_cmments(std::string &line, char c)
{
    int i = 0;

    while (line[i])
    {
        if (line[i] == c)
        {
            line.erase(i, line.length() - 1);
            break ;
        }
        i++;
    }
}


void    ParssFile::fill_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string line;
    while(std::getline(file, line))
    {
        remove_spaces(line);
        if (line.empty() || line == "\n")
            continue;
        if (line[0] == COMMENT2 || isspace(line[0]))
            continue;
        if (line.find(COMMENT2) != std::string::npos)
            delete_cmments(line, COMMENT2);
        if (line.find(COMMENT1) != std::string::npos)
            delete_cmments(line, COMMENT1);
        this->content_file.push_back(line);
    }
}

void    ParssFile::check_bracket_brace_file()
{
    std::string ptr;
    int open_bracket = 0;
    int close_bracket = 0;
    int open_brace = 0;
    int close_brace = 0;

    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        open_bracket = open_bracket + std::count(ptr.begin(), ptr.end(), '[');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        close_bracket = close_bracket + std::count(ptr.begin(), ptr.end(), ']');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        open_brace = open_brace + std::count(ptr.begin(), ptr.end(), '{');
    }
    for(std::vector<std::string>::iterator it = content_file.begin(); it != content_file.end(); it++)
    {
        ptr = *it;
        close_brace = close_brace + std::count(ptr.begin(), ptr.end(), '}');
    }
    if ((open_bracket != close_bracket) || ((open_bracket = 0) && (close_bracket = 0)))
        throw std::runtime_error("Error: Cheke Your Bracket");
    if ((open_brace != close_brace) || ((open_brace = 0) && (close_brace = 0)))
        throw std::runtime_error("Error: Cheke Your Brace");
}

void    ParssFile::take_port(std::string &ptr, dataserver& dataSr)
{
    std::string str1;
    ft_strtrim(ptr);
    if (ptr.empty())
        throw std::runtime_error("Error: Cheke Your Port");
    str1 = ptr.substr (0,6);
    // if (str1 != LISTEN)
    //     throw std::runtime_error("Error: Check your Server ->line [" + ptr + "]");
    ptr.erase(0, strlen(LISTEN));
    ft_strtrim(ptr);
    if (ptr.empty())
        throw std::runtime_error("Error: Cheke Your Port");
    for (size_t i = 0; i < ptr.size() ; i++)
        if (ptr[i] < '0' || ptr[i] > '9')
            throw std::runtime_error("Error: Check you port");
    dataSr.addListen(atoi(ptr.c_str()));
}

void    ParssFile::take_host(std::string & strhost, dataserver& dataHost)
{
    int lenght = strlen(HOST);
    ft_strtrim(strhost);
    if (strhost.empty())
        throw std::runtime_error("Error: Cheke Your Host");
    // if (strhost != HOST)
    //     throw std::runtime_error("Error: Check your Server ->line [" + strhost + "]");
    strhost.erase(0, lenght);
    ft_strtrim(strhost);
    if (strhost.empty())
        throw std::runtime_error("Error: Cheke Your Host");
    dataHost.setHost(strhost);
}

void    ParssFile::take_server_name(std::string &str, dataserver& dataServN)
{
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Server Name");
    str.erase(0, strlen(SERVER_NAME));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Server Name");
    dataServN.setServer_name(str);
}

void    ParssFile::take_C_M_B_S(std::string &str, dataserver&dataCMBS)
{
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Client_Max_Body_Size ");
    str.erase(0, strlen(CLIENT_MAX_BODY_SIZE));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Client_Max_Body_Size ");
    dataCMBS.setClient_max_body_size(atoi(str.c_str()));
}

void ParssFile::ft_strtrim(std::string &str)
{
    while (isspace(str.front()))
        str.erase(0, 1);
    while (isspace(str.back()))
        str.erase(str.length() - 1, 1);
}

int lenght_int(int nbr)
{
    int i = 0;
    while (nbr != 0)
    {
        nbr = nbr / 10;
        i++;
    }
    i++;
    return i;
}

void    ParssFile::take_Error_Page(std::string &str, dataserver& dataEPage)
{
    int nub_error = 0;
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    str.erase(0, strlen(ERROR_PAGE));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    nub_error = atoi(str.c_str());
    str.erase(0, lenght_int(nub_error));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Error Page");
    dataEPage.setError_page(nub_error, str);
}

void    ParssFile::take_Root(std::string &str, dataserver& dataRoot)
{
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Root");
    str.erase(0, strlen(ROOT));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("Error: Cheke Your Root");
    dataRoot.setRoot(str);
}

/********************************  LOCATION  ********************************/

void    ParssFile::take_L_autoindex(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Autoindex In Your Location Server");
    str.erase(0, strlen(AUTOINDEX));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Autoindex In Your Location Server");
    str.compare("on") == 0 ? loc.setL_AutoIndex(1): str.compare("off") == 0 ? loc.setL_AutoIndex(0): loc.setL_AutoIndex(-1);   
}

void    ParssFile::take_L_index(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Index In Your Location Server");
    str.erase(0, strlen(INDEX));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Index In Your Location Server");
    loc.setL_Index(str);
}

void    ParssFile::take_L_fastcgi_pass(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("Error: Cheke Fastcgi_Pass In Your Location Server");
    str.erase(0, strlen(FASTCGI_PASS));
    if (str.empty())
        throw std::runtime_error("Error: Cheke Fastcgi_Pass In Your Location Server");
    loc.setL_Fastcgi_Pass(str);
}

void    ParssFile::take_L_Allow_Methods(std::string &str, location &loc)
{
    std::map<std::string, int> methodes;
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    str.erase(0, strlen(ALLOW_METHODS));
    if (str.empty())
        throw std::runtime_error("Error: Cheke allow_methods In Your Location Server");
    if (str.find("GET") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("GET", 1));
    if (str.find("POST") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("POST", 1));
    if (str.find("DELETE") != std::string::npos)
        methodes.insert(std::pair<std::string, int>("DELETE", 1));
    loc.setL_Allowed_Methods(methodes);
}


void    ParssFile::init_pointer(bool location)
{
    if (!location)
    {
        this->pointer[0] = &ParssFile::take_port;
        this->pointer[1] = &ParssFile::take_host;
        this->pointer[2] = &ParssFile::take_server_name;
        this->pointer[3] = &ParssFile::take_C_M_B_S;
        this->pointer[4] = &ParssFile::take_Error_Page;
        this->pointer[5] = &ParssFile::take_Root;
    }
    else
    {
        this->location_pointer[0] = &ParssFile::take_L_autoindex;
        this->location_pointer[1] = &ParssFile::take_L_index;
        this->location_pointer[2] = &ParssFile::take_L_fastcgi_pass;
        this->location_pointer[3] = &ParssFile::take_L_Allow_Methods;
    }
}

location    ParssFile::getlocationInfo(int &start, int &end)
{
    location sv_loc = location();
    std::string arr[4] = {AUTOINDEX, INDEX, FASTCGI_PASS, ALLOW_METHODS};
    init_pointer(true);
    std::string tmp_type = this->content_file[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    if (tmp_type.empty())
        throw std::runtime_error("Error: Check Your Location!");
    sv_loc.setLocationtype(tmp_type);
    bool validLine = true;
    for (int i = start + 2; i < end; i++)
    {
        for (size_t count = 0; count < LOCATION_MAX_ELEMENT ; count++)
        {
            if(this->content_file[i].find(arr[count])!= std::string::npos)
            {
                (this->*location_pointer[count])(this->content_file[i],sv_loc);
                validLine = true;
                break;
            }
            else
                validLine = false;
        }
        if (!validLine)
            throw std::runtime_error("Error: Check your Location ->line" + std::to_string(i) + "[" + this->content_file[i] + "]");
    }
    return sv_loc;
}

void    ParssFile::getTypeExtention(std::string &tmp)
{
    int size = 0;
    while (tmp[size] != '.' && tmp[size])
        size++;
    if (tmp[size] == '.')
        tmp.erase(0, ++size);
}

void    ParssFile::run_location(int &start, int end, dataserver &sv)
{
    std::string tmp_type = this->content_file[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    getTypeExtention(tmp_type);
    std::map<std::string, location> sv_loc;
    sv_loc.insert(std::pair<std::string, location>(tmp_type, getlocationInfo(start, end)));
    sv.addLocation(sv_loc);
}

std::vector<dataserver> ParssFile::getServer()
{
    return this->server;
}

void    ParssFile::get_elements()
{
    int start, end;
    bool isNotLocation = false;
    std::string arr[6] = {LISTEN, HOST, SERVER_NAME, CLIENT_MAX_BODY_SIZE, ERROR_PAGE, ROOT};
    init_pointer(false);
    dataserver sv;
    for (size_t i = 0;i < index_server.size();)
    {
        start = index_server[i] + 1;
        end = index_server[i + 1];
        while (start < end)
        {
            for (size_t count = 0; count < 6; count++)
            {
                isNotLocation = false;
                if (this->content_file[start].find(arr[count]) != std::string::npos)
                {
                    isNotLocation = true;
                    (this->*pointer[count])(this->content_file[start], sv);
                    break;
                }
            }
            if (!isNotLocation)
            {
                isNotLocation = true;
                if (this->content_file[start].find(LOCATION) != std::string::npos )
                {
                    if (this->content_file[start + 1] != OPEN_BRACE)
                        throw std::runtime_error("Error: Check your Location Braces ->line [" + this->content_file[start] + "]");
                    else
                    {
                        int locationEND = start;
                        while (this->content_file[locationEND].find(CLOSE_BRACE) == std::string::npos)
                            locationEND++;
                        this->run_location(start, locationEND, sv);
                        start = locationEND;
                        locationEND = 0;
                    }
                }
                else
                    throw std::runtime_error("Error: I Dont now this line [" + this->content_file[start] + "]");
            }
            start++;
        }
        this->add_server(sv);
        sv.clear_all();
        i+=2;
    }
}

std::vector<dataserver>  ParssFile::SplitServers()
{
    std::vector<dataserver> servers;

    for (size_t i = 0; i < this->server.size(); i++)
    {
        std::vector<int> _ports;
        _ports = server[i].getListens();
        for (size_t j = 0; j < _ports.size(); j++)
        {
            dataserver newServer;
            newServer = server[i];
            newServer.setListen(_ports[j]);
            servers.push_back(newServer);
        }
    }
    return servers;
}

ParssFile::~ParssFile(){};