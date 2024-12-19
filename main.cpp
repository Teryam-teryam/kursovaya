#include <iostream>

#include "base.h"
#include "interface.h"
#include "communicator.h"
#include "calc.h"
#include "error.h"
#include "logger.h"
#include <string>

int main(int argc, const char** argv) {
    Interface appInterface;
    if (!appInterface.parseArguments(argc, argv)) {
        return 1;
    }
    std::string databaseFile = appInterface.getDatabaseFile();
    std::string logFile = appInterface.getLogFile();
    appInterface.setupConnection(databaseFile, logFile);
    
    return 0; 
}
