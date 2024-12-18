#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "logger.h"
#include "base.h"
#include "communicator.h"
#include "error.h"

namespace po = boost::program_options;

class Interface {
private:
    int serverPort;
    std::string databaseFile;
    std::string logFile;

public:
    Interface(): serverPort(33333), databaseFile("base.txt"), logFile("log.txt") {}
    bool parseArguments(int argc, const char** argv);
    void setupConnection(const std::string& databaseFile, const std::string& logFile);
    void displayHelp(const boost::program_options::options_description& options);
    int getServerPort() const { return serverPort; }
    std::string getDatabaseFile() { return databaseFile; }
    std::string getLogFile() { return logFile; }
};
