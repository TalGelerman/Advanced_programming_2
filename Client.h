#ifndef MAIN_CPP_CLIENT_H
#define MAIN_CPP_CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>

int ipAndPortValidation (const char *ipAddress , const int port);
int stringValidation (const std::string& input);

#include <unistd.h>

#endif //MAIN_CPP_CLIENT_H
