#pragma once
#include <map>
#include <string>
#include <filesystem>

class Base
{
private:
    std::map<std::string, std::string> dataBase;
public:
    void connect(std::string filePath); 
    std::map<std::string, std::string> getData() { return dataBase; }
};
