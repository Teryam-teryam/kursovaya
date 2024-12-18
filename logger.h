#pragma once
#include <string>

class Logger
{
private:
    std::string pathToLogFile; 
    std::string getTime();
public:
    Logger();
    Logger(const std::string& path); 
    int setPath(const std::string& pathFile);
    int writeLog(const std::string& message);
    std::string getPath() const { return pathToLogFile; }
};
