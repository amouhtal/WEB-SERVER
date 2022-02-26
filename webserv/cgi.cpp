#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define child 0
#define stdin 0
#define stdout 1
using namespace std;
 
namespace SERVER
{

void LaunchCGI()
{
    pid_t pid;
    int pipefd[2];
    pid = fork();
    
    if (pipe(pipefd))
    {
        perror("[CGI ERROR] PIPE");
    }

    if (pid == child)
    {
        // setup envirenment
        close(pipefd[0]);
        dup2(pipefd[1], stdout);
        close(pipefd[1]);
        execlp("cat", "cat", "a",NULL);
        // perror("demo");       /* still around?  exec failed           */
        exit(1);
    }
    else
    {
		waitpid(-1, NULL, 0);

    
    }
}

}
