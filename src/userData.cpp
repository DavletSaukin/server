/*
 * userData.cpp
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: Davlet
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
	    		break;
	    	}

	    }

	    fin.close();
	}

    return false;
}

//Returns true if password is correct, false if otherwise.
bool passwordCheck(std::string _login, std::string _password)
{
	char* nameBuf     = new char[userData::maxLoginSize];
	char* passwordBuf = new char[userData::maxPasswordSize];

	{
		std::lock_guard<std::mutex> guard(userData::mtx);
		std::ifstream fin("userdata.txt");
	    std::streamsize nameLength = userData::maxLoginSize;

	    auto loginSize    = _login.size();
	    auto passwordSize = _password.size();

	    while (1)
	    {
	    	fin.getline(nameBuf, nameLength, '\t');
	    	fin.getline(passwordBuf, nameLength, '\n');

	    	if(loginSize == strlen(nameBuf))
	    	{
	    		if (strncmp(nameBuf, _login.c_str(), (unsigned long int)_login.size()) == 0)
	    	    {
	    			if(passwordSize == strlen(passwordBuf))
	    			{
	    				if (strncmp(passwordBuf, _password.c_str(), (unsigned long int)_password.size()) == 0)
	    			    {
	    			    	fin.close();
	    	    	        return true;
	    			    }
	    				else
	    	            {
	    	            	fin.close();
	    	            	return false;
	    	            }
	    			}
	    			else
	    			{
	    				fin.close();
	    				return false;
	    			}
	    	    }
	    	}

	    	if (fin.fail())
	    	{
	    		break;
	    	}

	    }

	    fin.close();
	}

    return false;
}
