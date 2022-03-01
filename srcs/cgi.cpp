#include "../headers/cgi.hpp"
#include <fstream>
// #define stdin 0
// #define stdout 1
// using namespace std;

std::string LaunchCGI(location _location, std::string FilePath)
{
    pid_t pid;
    int Ifd[2];
    int Ofd[2];

    std::string head;
    std::string body;

    // int pipefd2[2];

    // int pipefd2[2];

    // int pipefd2[2];

    char buffer[CGI_BUFFER];
    std::string resContent;
    int ret;

    ret = 1;
    if (pipe(Ifd))
    {
        perror("[CGI ERROR] PIPE");
    }
    if (pipe(Ofd))
    {
        perror("[CGI ERROR] PIPE");
    }
    // if (pipe(pipefd2))
    // {
    //     perror("[CGI ERROR] PIPE");
    // }
    char *cgi_path = strcpy(cgi_path, _location.getL_Fastcgi_Pass().c_str());
    char *argv[] = {cgi_path , NULL};
    setenv("SCRIPT_FILENAME", FilePath.c_str(), 1);
    // setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    // setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);

    extern char **environ;
    pid = fork();
    if (pid == CHILD)
    {
        // TODO setup envirenment

        close(Ifd[0]);
        close(Ofd[1]);

        dup2(Ofd[0], 0);
        dup2(Ifd[1], 1);

        close(Ofd[0]);
        close(Ifd[1]);
        // dup2(pipefd[0], 0);
        // close(pipefd[1]);
        // close(pipefd[0]);
        if ((execve(argv[0], argv, environ)) == -1)
            perror("error in execve");
        exit(1);
    }
    else
    {
        close(Ifd[1]);

        close(Ofd[0]);
        // dup2(Ofd[1], 1);
        close(Ofd[1]);
        // TODO //   if (methode != GET)
        // std::ofstream file("out.txt");
        // write(Ofd[1], string , size);
        while (ret > 0)
        {
            memset(buffer, 0, CGI_BUFFER);
            ret = read(Ifd[0], buffer, CGI_BUFFER);
            // puts("here");
            resContent += buffer;
        }
        // file << resContent;
        close(Ifd[0]);
        // close(pipefd[0]);

        // wait(0);
    }
    // std::cout  << "|" << resContent << "|" << std::endl;

    head = resContent.substr(0, resContent.find("\r\n\r\n") + 2);
    body = resContent.substr(resContent.find("\r\n\r\n") + 4, resContent.size());

    // std::q2
    // std::cout << "find : " << head.find("\r\n");
    // head += head.find("\r\n") + 2;
    std::string line;
    while (head.size() != 0)
    {
        line = head.substr(0, head.find("\n") + 1);
        std::cout << "line : " << line;
        head = head.erase(0, head.find("\n") + 1);
    }
    return resContent;
}

// int main()
// {
//     LaunchCGI();
// }
