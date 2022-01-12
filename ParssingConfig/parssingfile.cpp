#include "parssingfile.hpp"

ParssFile::ParssFile(int ac, char **av)
{
    this->check_argument(ac, av);
}

void    ParssFile::check_argument(int ac, char **argv)
{
    if (ac < 1)
        throw std::runtime_error("Error: bad argument");
    else if (argv[1])
        this->file_name = argv[1];
    else
        this->file_name = "confg/config.conf";
    this->extention = &this->file_name[static_cast<int>(this->file_name.find('.') + 1)];
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
    int i = 0;
    if (content_file[0] != SERVER)
        throw std::runtime_error("Error: we don't find Server please check you config.conf");
    while (++i < content_file.size())
    {
        if (content_file[i].find(OPEN_BRACKET))
            index_server.push_back(i);
        if (content_file[i].find(CLOSE_BRACKET))
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
    std::ifstream file(this->file_name);
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
        close_brace = close_brace + std::count(content_file.begin(), content_file.end(), '}');
    }
    if (open_bracket != close_bracket)
        throw std::runtime_error("error: messing a open_bracket or close_bracket");
    if (open_brace != close_brace)
        throw std::runtime_error("error: messing a open_brace or close_brace");
}

ParssFile::~ParssFile(){};