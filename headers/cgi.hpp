#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILD 0
#define CGI_BUFFER 1024

std::string LaunchCGI();

#endif