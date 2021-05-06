//============================================================================
// Name        : server.cpp
// Author      : Davlet
//============================================================================


#include <cstddef>
#include <cstdio>

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "userData.h"
#include "serverHelper.h"

#include <thread>
#include <memory>
#include <forward_list>
#include <mutex>
#include <chrono>

std::mutex mutx;

int main()
{
    int sock, listener;
    auto addrPtr = std::make_shared<struct sockaddr_in>();

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    if(binder(listener, addrPtr) < 0)
    {
        perror("bind");
        exit(2);
    }
    listen(listener, 5);


    //The forward list stored thread-objects
    std::forward_list<std::unique_ptr<std::thread>> threadPtrs;


    int i = 0;
    while(i < 5)
    {
    	sock = accept(listener, NULL, NULL);
    	if(sock < 0)
    	{
    	    perror("accept");
    	    exit(3);
    	}

    	threadPtrs.push_front(std::make_unique<std::thread>(clientsServing, sock));
    	i++;
    }

    close(listener);

	for (auto& ptr: threadPtrs)
	{
		if(ptr->joinable())
			ptr->join();
	}

    return 0;
}
