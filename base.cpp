#include "error.h"
#include "base.h"
#include "interface.h"
#include <iostream>
#include <fstream>

void Base::connect(std::string filePath)
{
		if (filePath.empty())
    	{throw CritError("Empty path for database: CRITICAL ERROR");}
    if (filePath.find(".txt") == std::string::npos)
    	{throw CritError("Database has wrong extension: CRITICAL ERROR");} 
    if (!std::filesystem::exists(filePath))
    	{throw CritError("Specified DB-file does not exist: CRITICAL ERROR");}
    
    
    std::ifstream readFile;
    std::string line;
    readFile.open(filePath);
    
    if (readFile.is_open())
    {
        while (getline(readFile, line))
        {
            size_t delimiterIndex = line.find(':');
            if (delimiterIndex == std::string::npos)
            {
                throw CritError("Invalid [login:password] format: CRITICAL ERROR");
                exit(1);
            }
            
            std::string login = line.substr(0, delimiterIndex);
            std::string password = line.substr(delimiterIndex + 1);
            if (login.empty())
            	{throw CritError("No login");}
            if (password.empty())
            	{throw CritError("No password");}
            dataBase[login] = password;
        }
    }
    else
    	{throw CritError("The database did not open: CRITICAL ERROR");}
    if (dataBase.empty())
    	{throw CritError("Database is empty: CRITICAL ERROR");}
}
