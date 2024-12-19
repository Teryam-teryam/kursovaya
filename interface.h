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

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для интерфейса
 */

/** @brief Класс интерфейса
 * @details Управляет настройками сервера и взаимодействием с пользователем.
 */
class Interface {
private:
    int serverPort; ///< Номер порта сервера
    std::string databaseFile; ///< Путь к файлу базы данных
    std::string logFile; ///< Путь к файлу журнала

public:
    /**
     * @brief Конструктор по умолчанию
     * 
     * Инициализирует параметры по умолчанию.
     */
    Interface() : serverPort(33333), databaseFile("base.txt"), logFile("log.txt") {}

    /**
     * @brief Парсер аргументов командной строки
     * 
     * @details Читает операнды командной строки. В случае передачи операнда -h
     * производится вызов справки.
     * @param [in] argc Количество аргументов.
     * @param [in] argv Массив аргументов.
     * @return bool true, если аргументы обработаны корректно; false в противном случае.
     * @throw CritError В случае некорректного значения порта.
     */
    bool parseArguments(int argc, const char** argv);

    /**
     * @brief Настройка соединения
     * 
     * @details Устанавливает соединение с базой данных и журналом лога.
     * @param [in] databaseFile Путь к файлу базы данных.
     * @param [in] logFile Путь к файлу журнала.
     */
    void setupConnection(const std::string& databaseFile, const std::string& logFile);

    /**
     * @brief Отображение справки
     * 
     * @param [in] options Описание доступных опций.
     */
    void displayHelp(const boost::program_options::options_description& options);

    /**
     * @brief Получение порта сервера
     * 
     * @return int Номер порта сервера.
     */
    int getServerPort() const { return serverPort; }

    /**
     * @brief Получение пути к файлу базы данных
     * 
     * @return std::string Путь к файлу базы данных.
     */
    std::string getDatabaseFile() { return databaseFile; }

    /**
     * @brief Получение пути к файлу журнала
     * 
     * @return std::string Путь к файлу журнала.
     */
    std::string getLogFile() { return logFile; }
};
