#include "interface.h"

void Interface::displayHelp(const po::options_description& options) {
    std::cout << options << std::endl;
}

bool Interface::parseArguments(int argc, const char** argv) {
    po::options_description options("Options");
    options.add_options()
        ("help,h", "Display help")
        ("databaseFile,b", po::value<std::string>()->default_value("base.txt"), "Path to the database")
        ("logFile,l", po::value<std::string>()->default_value("log.txt"), "Path to the log file")
        ("port,p", po::value<int>(&serverPort)->default_value(33333), "Server port");
   
    po::variables_map variableMap;
    try
    {
        po::store(po::parse_command_line(argc, argv, options), variableMap);
        po::notify(variableMap);
    }
    catch (const po::unknown_option& e)
    {
    		displayHelp(options);
        throw CritError("Unknown option: " + std::string(e.what())); // Изменено на выброс CritError
    }

    if (variableMap.count("help"))
    {
        displayHelp(options);
        return true; 
    }

    
    databaseFile = variableMap["databaseFile"].as<std::string>();
    logFile = variableMap["logFile"].as<std::string>();
    
    std::ifstream dbFile(databaseFile);
    std::ifstream lgFile(logFile);
    Logger logger(logFile);
    if (serverPort < 1024 || serverPort > 65535)
    	{throw CritError("Invalid port: " + std::to_string(serverPort) + " Critical error");}       
    return true;
}


void Interface::setupConnection(const std::string& databaseFile, const std::string& logFile)
{
    Base database;
    Logger logger(logFile);
		database.connect(databaseFile);
    Communicator communicator;
    communicator.connection(serverPort, database.getData(), &logger);
}
