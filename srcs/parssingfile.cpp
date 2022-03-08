#include "../headers/parssingfile.hpp"

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
    this->servers.push_back(var);
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
        throw std::runtime_error("\e[1;31mCheck Your Config Extention\e[1;32m");
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
            throw std::runtime_error("\e[1;31mError: Check your config.conf\e[1;32m");
    if (content_file[0] != "server" || content_file[1] != OPEN_BRACKET)
        throw std::runtime_error("\e[1;31mError: Check your config.conf\e[1;32m");
    while (++i < content_file.size())
    {
        if (content_file[i] == OPEN_BRACKET)
            index_server.push_back(i);
        if (content_file[i] == CLOSE_BRACKET)
            index_server.push_back(i);
        if (content_file[i] == OPEN_BRACKET && content_file[i - 1].find("server") == std::string::npos)
            throw std::runtime_error("\e[1;31mError: Check you configuration file\e[1;32m");
    }
}

void    ParssFile::delete_cmments(std::string &line, char c)
{
    int i = 0;

    while (line[i])
    {
        if (line[i] == c)
        {
            line.erase(i, line.length());
            break ;
        }
        i++;
    }
}


void    ParssFile::fill_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string line;
    std::string _tmp;
    while(std::getline(file, line))
    {
        remove_spaces(line);  
        if (line.empty() || line == "\n")
            continue;
        if (line[0] == COMMENT2 || isspace(line[0]))
            continue;
        if (line.find(COMMENT2) != std::string::npos)
            delete_cmments(line, COMMENT2);
        ft_strtrim(line);
        if (line.length())
            line.append(";");
        if(line[0] != '\0')
            _tmp += line;
    }
    this->content_file = ft_split(_tmp, ';');
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
        throw std::runtime_error("\e[1;31mError: Check Your Bracket\e[1;32m");
    if ((open_brace != close_brace) || ((open_brace = 0) && (close_brace = 0)))
        throw std::runtime_error("\e[1;31mError: Check Your Brace\e[1;32m");
}

void    ParssFile::take_port(std::string &ptr, dataserver& dataSr)
{
    ft_strtrim(ptr);
    int port;
    if (ptr.empty())
        throw std::runtime_error("\e[1;31mError: Check Your Port\e[1;32m");
    ptr.erase(0, strlen(LISTEN));
    ft_strtrim(ptr);
    if (ptr.empty())
        throw std::runtime_error("\e[1;31mError: Check Your Port\e[1;32m");
    for (size_t i = 0; i < ptr.size() ; i++)
        if (ptr[i] < '0' || ptr[i] > '9')
            throw std::runtime_error("\e[1;31mError: Check your port\e[1;32m");
    port = atoi(ptr.c_str());
    if (dataSr.getListens().size())
    {
        for (size_t i = 0; i < dataSr.getListens().size(); i++)
        {
            if (dataSr.getListens()[i] == port)
                throw std::runtime_error("\e[1;31mError: You Have A Dublicate Port Please Check Your Port\e[1;32m");
        }
    }
    dataSr.addListen(port);
}

void    ParssFile::take_host(std::string & strhost, dataserver& dataHost)
{
    if (dataHost.getHost().empty())
    {
        int lenght = strlen(HOST);
        int cont = 0;
        int i = 0;
        int j = 0;
        std::string str;
        ft_strtrim(strhost);
        if (strhost.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Host\e[1;32m");
        strhost.erase(0, lenght);
        ft_strtrim(strhost);
        if (strhost.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Host\e[1;32m");
        if (strhost.find('.') != std::string::npos)
        {
            while (i < strhost.length())
            {
                if (strhost[i] == '.')
                {
                    cont++;
                    ++i;
                }
                str = str + strhost[i];
                i++;
            }
            if (cont != 3)
                throw std::runtime_error("\e[1;31mError: Check Your Host\e[1;32m");
            while (j < str[j])
            {
                if (str[j] < '0' || str[j] > '9')
                    throw std::runtime_error("\e[1;31mError: Check Your Host\e[1;32m");
                j++;
            }
            dataHost.setHost(strhost);
        }
        dataHost.setHost(strhost);
    }
    else
        throw std::runtime_error("\e[1;31mError: You Have A Dublicate Host Please Check Your Host\e[1;32m");
}

void    ParssFile::take_server_name(std::string &str, dataserver& dataServN)
{
    if (dataServN.getServer_name().empty())
    {
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Server Name\e[1;32m");
        str.erase(0, strlen(SERVER_NAME));
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Server Name\e[1;32m");
        dataServN.setServer_name(str);
    }
    else
        throw std::runtime_error("\e[1;31mError: You Have A Dublicate Server Name Please Check Your Server Name\e[1;32m");
}

void    ParssFile::take_C_M_B_S(std::string &str, dataserver& dataCMBS)
{
    int i = 0;
    i = dataCMBS.getClient_max_body_size();
    if (i == -1)
    {
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Client_Max_Body_Size \e[1;32m");
        str.erase(0, strlen(CLIENT_MAX_BODY_SIZE));
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError: Check Your Client_Max_Body_Size \e[1;32m");
        dataCMBS.setClient_max_body_size(atoi(str.c_str()));
    }
    else
        throw std::runtime_error("\e[1;31mError: Dublicate PLease Check Your Client_Max_Body_Size \e[1;32m");
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
        throw std::runtime_error("\e[1;31mError Empty: Check Your Error Page\e[1;32m");
    str.erase(0, strlen(ERROR_PAGE));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError Empty: Check Your Error Page\e[1;32m");
    nub_error = atoi(str.c_str());
    str.erase(0, lenght_int(nub_error));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError Empty: Check Your Error Page\e[1;32m");
    // if ((r = opendir(str.c_str())))
        dataEPage.setError_page(nub_error, str);
    // else
    //     throw std::runtime_error("\e[1;31mError Path: Check Your Error Page\e[1;32m");
}

void    ParssFile::take_Root(std::string &str, dataserver& dataRoot)
{
    DIR *r;
    if (dataRoot.getRoot().empty())
    {
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError Empty: Check Your Root In Server\e[1;32m");
        str.erase(0, strlen(ROOT));
        ft_strtrim(str);
        if (str.empty())
            throw std::runtime_error("\e[1;31mError Empty: Check Your Root In Server\e[1;32m");
        if ((r = opendir(str.c_str())))
            dataRoot.setRoot(str);
        else
            throw std::runtime_error("\e[1;31mError Path: Check Your Root In Server\e[1;32m");
    }
    else
        throw std::runtime_error("\e[1;31mError: Dublicate Root Please Check Your Root In Server\e[1;32m");
}

/********************************  LOCATION  ********************************/

void    ParssFile::take_L_Root(std::string &str, location& loc)
{
    DIR *r;
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Your Root\e[1;32m");
    str.erase(0, strlen(ROOT));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Your Root\e[1;32m");
    if ((r = opendir(str.c_str())))
    {
        loc.setL_Root(str);
        loc.isRoot = true;
    }
    else
        throw std::runtime_error("\e[1;31mError Path: Check Your Root In Location\e[1;32m");
}

void    ParssFile::take_L_autoindex(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Autoindex In Your Location\e[1;32m");
    str.erase(0, strlen(AUTOINDEX));
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Autoindex In Your Location\e[1;32m");
    str.compare("on") == 0 ? loc.setL_AutoIndex(1): str.compare("off") == 0 ?
        loc.setL_AutoIndex(0): throw std::runtime_error("\e[1;31mError: Check Autoindex In Your Location\e[1;32m");  
}

void    ParssFile::take_L_index(std::string &str, location &loc)
{
    std::string tmp;
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Index In Your Location\e[1;32m");
    str.erase(0, strlen(INDEX));
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Index In Your Location\e[1;32m");
    tmp = str;
    int i = std::count(tmp.begin(), tmp.end(), '.');
    if (tmp.find('.') == std::string::npos || i > 1)
        throw std::runtime_error("\e[1;31mError: Check Index In Your Location\e[1;32m");
    tmp.erase(0, tmp.length() - 5);
    if (tmp.compare(".html") != 0)
        throw std::runtime_error("\e[1;31mError: Check Index In Your Location\e[1;32m");
    loc.setL_Index(str);
}

void    ParssFile::take_L_fastcgi_pass(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("\e[1;31mError Empty: Check Fastcgi_Pass In Your Location\e[1;32m");
    str.erase(0, strlen(FASTCGI_PASS));
    if (str.empty())
        throw std::runtime_error("\e[1;31mError Empty: Check Fastcgi_Pass In Your Location\e[1;32m");
        loc.setL_Fastcgi_Pass(str);
        loc.isCgi = true;
}

void    ParssFile::take_L_Allow_Methods(std::string &str, location &loc)
{
    std::map<std::string, int> methodes;
    size_t i = 0;
    size_t cont = 0;
    ft_strtrim(str);
    str.erase(0, strlen(ALLOW_METHODS));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
    if (str.length() == 17 || str.length() == 10 || str.length() == 12 ||
        str.length() == 13 ||  str.length() == 6 || str.length() == 8 || str.length() == 5)
    {
        if (str[0] == '[' && (str[str.length() - 1]) == ']')
        {
            while (str[i])
                if (str[i++] == ',')
                    cont++;
            if (cont > 2)
                throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
            else if (cont != 2 && str.length() == 17)
                throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
            else if (cont != 1 && (str.length() == 10 || str.length() == 12 || str.length() == 13))
                throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
            else if (cont != 0 && (str.length() == 6 || str.length() == 8 || str.length() == 5))
                throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
            else
            {
                if (str.find("GET") != std::string::npos)
                    methodes.insert(std::pair<std::string, int>("GET", 1));
                if (str.find("POST") != std::string::npos)
                    methodes.insert(std::pair<std::string, int>("POST", 1));
                if (str.find("DELETE") != std::string::npos)
                    methodes.insert(std::pair<std::string, int>("DELETE", 1));
            }
        }
        else
            throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
    }
    else
        throw std::runtime_error("\e[1;31mError: Check allow_methods In Your Location\e[1;32m");
    loc.setL_Allowed_Methods(methodes);
}


void    ParssFile::take_L_Up_Enb(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Upload_Enable In Your Location\e[1;32m");
    str.erase(0, strlen(UPLOAD_ENABLE));
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Upload_Enable In Your Location\e[1;32m");
    str.compare("on") == 0 ? loc.set_L_upload_enb(1) : str.compare("off") == 0 ?
    loc.set_L_upload_enb(0): throw std::runtime_error("\e[1;31mError: Check Upload_Enable In Your Location\e[1;32m");
;   
}

void    ParssFile::take_L_Up_Store(std::string &str, location &loc)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Upload_Store In Your Location\e[1;32m");
    str.erase(0, strlen(UPLOAD_STORE));
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Upload_Store In Your Location\e[1;32m");
    loc.set_L_upload_store(str);
}

void ParssFile::split(std::string line, char splitter)
{
    int i = 0;
    int k = 0;
    int start = 0;
    int end = line.find(splitter);

    this->mapTmp.clear();
    while (end != -1)
    {
        k = end;
        this->mapTmp.insert(std::pair<int, std::string>(i, line.substr(start, end - start)));
        while (line[k] == splitter)
        {
            end++;
            k++;
        }
        start = end;
        end = line.find(splitter, start);
        i++;
    }
    this->mapTmp.insert(std::pair<int, std::string>(i, line.substr(start, end - start)));
}

void    ParssFile::take_L_Return(std::string &str, location &loc)
{
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Return In Your Location\e[1;32m");
    str.erase(0, strlen(RETURN));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Return In Your Location\e[1;32m");
    split(str, ' ');
    if (this->mapTmp.size() > 2)
        throw std::runtime_error("\e[1;31mError: Check Return In Your Location\e[1;32m");
    int nbr = atoi(str.c_str());
    str.erase(0, lenght_int(nbr));
    ft_strtrim(str);
    if (str.empty())
        throw std::runtime_error("\e[1;31mError: Check Return In Your Location\e[1;32m");
    loc.setL_Return(nbr, str);
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
        this->location_pointer[4] = &ParssFile::take_L_Up_Enb;
        this->location_pointer[5] = &ParssFile::take_L_Up_Store;
        this->location_pointer[6] = &ParssFile::take_L_Return;
        this->location_pointer[7] = &ParssFile::take_L_Root;

    }
}

location    ParssFile::getlocationInfo(int &start, int &end)
{
    location sv_loc = location();
    std::string arr[8] = {AUTOINDEX, INDEX, FASTCGI_PASS, ALLOW_METHODS, UPLOAD_ENABLE, UPLOAD_STORE, RETURN, ROOT};
    init_pointer(true);
    std::string tmp_type = this->content_file[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    if (tmp_type.empty())
        throw std::runtime_error("\e[1;31mError: Check Your Location!\e[1;32m");
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
            throw std::runtime_error("\e[1;31mError: Check your Location Invalid Line\e[1;32m");
    }
    return sv_loc;
}

std::string    ParssFile::getTypeExtention(std::string tmp)
{
    location sv_loc;
    int size = tmp.find('.');
    if(size != std::string::npos)
    {
        tmp.erase(0, ++size);
        return(tmp);
    }
    return "";
}

void    ParssFile::run_location(int &start, int end, dataserver &sv)
{
    std::string tmp_type = this->content_file[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    std::map<std::string, location> sv_loc;
    sv_loc.insert(std::pair<std::string, location>(tmp_type, getlocationInfo(start, end)));
    sv_loc.begin()->second.setLocationExtention(getTypeExtention(tmp_type));
    sv.addLocation(sv_loc);
}

std::vector<dataserver> ParssFile::getServer()
{
    return this->servers;
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
                        throw std::runtime_error("\e[1;31mError: Check your Location Braces\e[1;32m");
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
                    throw std::runtime_error("\e[1;31mError: I Dont now this line [" + this->content_file[start] + "]\e[1;32m");
            }
            start++;
        }
        this->add_server(sv);
        sv.clear_all();
        i+=2;
    }
}

std::vector<std::string> ParssFile::ft_split(std::string const &str, char c)
{
	std::vector<std::string> new_str;
	std::stringstream ss(str);
	std::string buff;

	while (getline(ss, buff, c))
    {
        ft_strtrim(buff);
        if (buff[0] != 0)
		    new_str.push_back(buff);
    }
	return new_str;

}

std::vector<dataserver>  ParssFile::SplitServers()
{
    std::vector<dataserver> server;

    for (size_t i = 0; i < this->servers.size(); i++)
    {
        std::vector<int> _ports;
        _ports = servers[i].getListens();
        for (size_t j = 0; j < _ports.size(); j++)
        {
            dataserver newServer;
            newServer = servers[i];
            newServer.setListen(_ports[j]);
            server.push_back(newServer);
        }
    }
    return server;
}

ParssFile::~ParssFile(){};