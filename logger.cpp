#include "logger.h"
#include "error.h"
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <filesystem>

Logger::Logger() : pathToLogFile("") {}
Logger::Logger(const std::string& path) : pathToLogFile(path) {}

int Logger::setPath(const std::string& newPath)
{
    if (newPath.empty())
    	{throw CritError("Empty path for log file: CRITICAL ERROR");}
    if (newPath.find(".txt") == std::string::npos)
    	{throw CritError("Invalid log file extension: CRITICAL ERROR");}
    std::filesystem::path p(newPath);
    if (!std::filesystem::exists(p.parent_path()))
    	{throw CritError("Specified path to log file does not exist: CRITICAL ERROR");}

    pathToLogFile = newPath;
    return 0;
}

int Logger::writeLog(const std::string& message)
{
    if (pathToLogFile.empty())
    	{throw CritError("Empty path to log file: CRITICAL ERROR");}
    if (!std::filesystem::exists(pathToLogFile))
    	{throw CritError("Log file does not exist: CRITICAL ERROR");}
    std::ofstream logFile(pathToLogFile, std::ios_base::out | std::ios_base::app);
    if (!logFile.is_open())
    	{throw CritError("Failed to open log file: CRITICAL ERROR");}
    
    logFile << getTime() << ' ' << message << std::endl;
    return 0;
}


std::string Logger::getTime() {
    time_t rawTime;
    struct tm* timeInfo;
    char buffer[80];
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%F %X", timeInfo);
    return std::string(buffer);
}
