/*
 * userData.h
 *
 *      Author: davlet
 */

#ifndef USERDATA_H_
#define USERDATA_H_

#include <string>
#include <cstring>
#include <fstream>

class userData {
private:
	std::string login;
	std::string password;

	bool isAlreadyExist();

public:
    const static int maxLoginSize     = 100;
    const static int maxPasswordSize = 100;

	userData() = default;
	userData(std::string _login, std::string _password);

	virtual ~userData() = default;

    bool writeInFile();

};

#endif /* USERDATA_H_ */
