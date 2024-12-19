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
        
        // Проверка порта после его получения
        if (serverPort < 1024 || serverPort > 65535)
        {
            throw CritError("Invalid port: " + std::to_string(serverPort) + " Critical error");
        }
    }
    catch (const CritError& e)
    {
        std::cerr << e.what() << std::endl; // Вывод ошибки в консоль
        return false; // Завершаем выполнение метода
    }
    catch (const po::unknown_option& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        displayHelp(options);
        return false;
    }

    if (variableMap.count("help"))
    {
        displayHelp(options);
        return true; 
    }

    // Получаем и выводим пути к файлам
    databaseFile = variableMap["databaseFile"].as<std::string>();
    logFile = variableMap["logFile"].as<std::string>();
    
    std::cout << "Database file path: " << databaseFile << std::endl;
    std::cout << "Log file path: " << logFile << std::endl;

    return true;
}


void Interface::setupConnection(const std::string& databaseFile, const std::string& logFile)
{
    Base database;
    Logger logger(logFile);
    try
    {
        database.connect(databaseFile);
    }
    catch (const CritError& e)
    {
        logger.writeLog("Error connecting to the database: " + std::string(e.what()));
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
    catch (const std::exception& e)
    {
        logger.writeLog("Unknown error: " + std::string(e.what()));
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return;
    }

    Communicator communicator;
    communicator.connection(serverPort, database.getData(), &logger);
}

