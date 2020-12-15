/*
 * serverHelper.h
 *
 *      Author: davlet
 */

#ifndef SERVERHELPER_H_
#define SERVERHELPER_H_

#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <memory>
#include "userData.h"

inline int binder(int _sock, std::shared_ptr<struct sockaddr_in> addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(3425);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	return bind(_sock, (struct sockaddr *)addr.get(), sizeof(addr));
}


inline void clientsServing(int _sock)
{
	short int clientReq;
	std::string login{};
	std::string password{};

	//accept client's request
	recv(_sock, &clientReq, sizeof(short int), 0);

	if(clientReq == 0) // Sign In
	{
		char loginBuf[userData::maxLoginSize];

		//acept login
	    auto readedBytes = recv(_sock, loginBuf, userData::maxLoginSize, 0);
	    login.assign(loginBuf, readedBytes);

	    if(!(isAlreadyExist(login)))
	    {
	    	short int serverMsg = 401;
	        send(_sock, &serverMsg, sizeof(short int), 0);
	    	return;
	    }
	    else
	    {
	    	//acept password
	    	char passwordBuf[userData::maxPasswordSize];
	        readedBytes = recv(_sock, passwordBuf, userData::maxPasswordSize, 0);
	        password.assign(passwordBuf, readedBytes);
	    }

	    //Create user's object
	    userData user(login, password);

		short int serverMsg = 200;
	    send(_sock, &serverMsg, sizeof(short int), 0);
	}
	else if (clientReq == 1) // Sign Up
	{
		char loginBuf[userData::maxLoginSize];

		//acept login
	    auto readedBytes = recv(_sock, loginBuf, userData::maxLoginSize, 0);
	    login.assign(loginBuf, readedBytes);

	    if(isAlreadyExist(login))
	    {
	    	short int serverMsg = 401;
	    	send(_sock, &serverMsg, sizeof(short int), 0);
	    	return;

	    	//std::string msg("This login is used already");
	    	//send(_sock, msg.c_str(), readedBytes, 0);
	    	//return;
	    }
	    else
	    {
	    	//acept password
	        char passwordBuf[userData::maxPasswordSize];
	        readedBytes = recv(_sock, passwordBuf, userData::maxPasswordSize, 0);
	        password.assign(passwordBuf, readedBytes);
	    }

	    //Create user's object
	    userData user(login, password);

	    bool isWrited = user.writeInFile();
	    if(!isWrited)
	   	{
	    	short int serverMsg = 401;
	        send(_sock, &serverMsg, sizeof(short int), 0);
	        return;
	   	}

	    short int serverMsg = 200;
	    send(_sock, &serverMsg, sizeof(short int), 0);
	}

	close(_sock);
}



#endif /* SERVERHELPER_H_ */
