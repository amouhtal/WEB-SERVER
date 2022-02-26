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


char **env2;
void LaunchCGI()
{
    pid_t pid;
    int pipefd[2];
    // int pipefd2[2];

    char buffer[CGI_BUFFER];
    std::string resContent;
    int ret;

    if (pipe(pipefd))
    {
        perror("[CGI ERROR] PIPE");
    }
    pid = fork();
    if (pid == CHILD)
    {
        // TODO setup envirenment
        close(pipefd[1]);
        dup2(pipefd[1], 1);
        // dup2(pipefd[0], 0);
        // close(pipefd[1]);
        // close(pipefd[0]);
        char *argv[]={"ls", NULL};
        if ((execve("/bin/ls", argv, env2)) == -1)
           perror("error in execve");
        exit(1);
    }
    else
    {
        
        wait(0);
        close(pipefd[1]);
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
        std::cerr << resContent << std::endl;
    }
    // dup2();
}

int main(int ac, char **av, char **env)
{
    env2 = env;
    LaunchCGI();
    return 0;
}
