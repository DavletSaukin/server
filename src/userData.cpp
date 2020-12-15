/*
 * userData.cpp
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: davlet
 */

#include "userData.h"

std::mutex userData::mtx;

userData::userData(std::string _login, std::string _password)
                  : login(_login), password(_password)
{}

bool userData::writeInFile()
{
	if (isAlreadyExist(this->login))
	{
		return false;
	}

	{
		std::lock_guard<std::mutex> guard(mtx);
		std::ofstream fout("userdata.txt", std::ios::ate | std::ios::app);
	    fout << login.c_str() << '\t' << password.c_str() << '\n';
	    fout.close();
	}

	return true;
};


//Returns true if _login is alredy used.
bool isAlreadyExist(std::string _login)
{
	char* nameBuf     = new char[userData::maxLoginSize];
	char* remainingBuf = new char[userData::maxPasswordSize];

	{
		std::lock_guard<std::mutex> guard(userData::mtx);
		std::ifstream fin("userdata.txt");
	    std::streamsize nameLength = userData::maxLoginSize;

	    auto loginSize = _login.size();

	    while (1)
	    {
	    	fin.getline(nameBuf, nameLength, '\t');
	    	fin.getline(remainingBuf, nameLength, '\n');

	    	if(loginSize == strlen(nameBuf))
	    	{
	    		if (strncmp(nameBuf, _login.c_str(), (unsigned long int)_login.size()) == 0)
	    	    {
	    	    	fin.close();
	    	    	return true;
	    	    }
	    	}

	    	if (fin.fail())
	    	{
	    		fin.close();
	    		break;
	    	}

	    }

	    fin.close();
	}

    return false;
}
