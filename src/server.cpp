//============================================================================
// Name        : server.cpp
// Author      : Davlet
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <cstddef>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "userData.h"

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char loginBuf[userData::maxLoginSize];
    char passwordBuf[userData::maxPasswordSize];

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    sock = accept(listener, NULL, NULL);
    if(sock < 0)
    {
        perror("accept");
        exit(3);
    }

    //acept login
    auto readedBytes = recv(sock, loginBuf, userData::maxLoginSize, 0);
    std::string login(loginBuf);

    auto sendedBytes = send(sock, login.c_str(), readedBytes, 0);

    std::cout << "sendedBytes: " << sendedBytes << std::endl;
    std::cout << "readedBytes: " << readedBytes << std::endl;

    //acept password
    readedBytes = recv(sock, passwordBuf, userData::maxPasswordSize, 0);
    std::string password(passwordBuf);

    sendedBytes = send(sock, password.c_str(), readedBytes, 0);

    std::cout << "sendedBytes: " << sendedBytes << std::endl;
    std::cout << "readedBytes: " << readedBytes << std::endl;



    close(sock);
    close(listener);

    std::cout <<"server: " << login <<std::endl;
    std::cout <<"server: " << password <<std::endl;

    userData user(login, password);
    bool isWrited = user.writeInFile();
    if(isWrited)
    {
    	std::cout << "WRITED" << std::endl;
    }


    return 0;
}
