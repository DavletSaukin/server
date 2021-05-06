/*
 * userData.h
 *
 *      Author: Davlet
 */

#ifndef USERDATA_H_
#define USERDATA_H_

#include <string>
#include <cstring>
#include <fstream>
#include <mutex>

//preliminary declaration
bool isAlreadyExist(std::string _login);
bool passwordCheck(std::string _login, std::string _password);

class userData {
private:
	std::string login;
	std::string password;

	static std::mutex mtx;

public:
    const static int maxLoginSize     = 100;
    const static int maxPasswordSize = 100;

	userData() = default;
	userData(std::string _login, std::string _password);

	virtual ~userData() = default;

    bool writeInFile();
    friend bool isAlreadyExist(std::string _login);
    friend bool passwordCheck(std::string _login, std::string _password);

};

#endif /* USERDATA_H_ */
