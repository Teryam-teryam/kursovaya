#pragma once

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "logger.h"
#include "error.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

class Communicator
{
public:
    int connection(int port, const std::map<std::string, std::string>& database, Logger* logger);// Метод для установки соединения с клиентами
    std::string MD5(const std::string& inputString);// Метод для вычисления MD5 хеша
    std::string generateSalt();// Метод для генерации соли
};
