#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILD 0
#define CGI_BUFFER 1024
// #define stdin 0
// #define stdout 1
// using namespace std;

std::string LaunchCGI()
{
    pid_t pid;
    int pipefd[2];
    // int pipefd2[2];

    // int pipefd2[2];

    char buffer[CGI_BUFFER];
    std::string resContent;
    int ret;

    if (pipe(pipefd))
    {
        perror("[CGI ERROR] PIPE");
    }
    // if (pipe(pipefd2))
    // {
    //     perror("[CGI ERROR] PIPE");
    // }
    char *argv[] = {"/Users/mel-hamr/goinfre/.brew/bin/php-cgi", NULL};
    setenv("SCRIPT_FILENAME", "simple.php", 1);

    extern char **environ;
    pid = fork();
    if (pid == CHILD)
    {
        // TODO setup envirenment
        close(pipefd[1]);

        // close(pipefd2[1]);

        dup2(pipefd[1], 1);
        // dup2(pipefd[0], 0);
        // close(pipefd[1]);
        // close(pipefd[0]);
        if ((execve(argv[0], argv, environ)) == -1)
            perror("error in execve");
        exit(1);
    }
    else
    {
        close(pipefd[1]);

        // close(pipefd2[1]);
        // dup2(pipefd2[1], 1);
        // dup2(pipefd[0], 0);
        close(pipefd[0]);
        // TODO //   if (methode != GET)
        // write(pipefd[1], string , size);
        ret = 1;
        while (ret > 0)
        {
            memset(buffer, 0, CGI_BUFFER);
            ret = read(pipefd[0], buffer, CGI_BUFFER);
            // puts("here");
            resContent += buffer;
        }
        close(pipefd[0]);
        // std::cout << resContent << std::endl;
        // wait(0);
    }
    return resContent;
}