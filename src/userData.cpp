/*
 * userData.cpp
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: davlet
 */

#include "userData.h"

userData::userData(std::string _login, std::string _password)
                  : login(_login), password(_password)
{}

bool userData::writeInFile()
{
	if (isAlreadyExist())
	{
		return false;
	}

	std::ofstream fout("userdata.txt", std::ios::ate | std::ios::app);

	fout << login.c_str() << '\t' << password.c_str() << '\n';

	fout.close();

	return true;
};

bool userData::isAlreadyExist()
{
	//char buff[24];

	char* nameBuf     = new char[maxLoginSize];
	char* remainingBuf = new char[maxPasswordSize];

	std::ifstream fin("userdata.txt");
	std::streamsize nameLength = maxLoginSize;


	while (1)
	{
		fin.getline(nameBuf, nameLength, '\t');
		fin.getline(remainingBuf, nameLength, '\n');

		if (strncmp(nameBuf, login.c_str(), (unsigned long int)login.size()) == 0)
			return true;
		if (fin.fail())
		{
			fin.close();
			break;
		}
		//fin.seekg((int)fin.tellg() + 14);
	}

	fin.close();
    return false;
}
