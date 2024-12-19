#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "logger.h"
#include "error.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для коммуникатора сервера
 */

/** @brief Класс коммуникатора
 * @details Устанавливает соединение с сервером, производит авторизацию клиента.
 */
class Communicator
{
public:
    /**
     * @brief Установка соединения с клиентами
     * 
     * @details Производит соединение с сервером и авторизует пользователя.
     * @param [in] port Номер порта для соединения.
     * @param [in] database Контейнер с базой данных пользователей.
     * @param [in] logger Указатель на объект Logger для записи событий.
     * @return int Код результата операции.
     */
    int connection(int port, const std::map<std::string, std::string>& database, Logger* logger);

    /**
     * @brief Вычисление MD5 хеша
     * 
     * @details Производит вычисление MD5 хеша для заданной строки.
     * @param [in] inputString Входная строка для хэширования.
     * @return std::string Результат хэширования.
     */
    std::string MD5(const std::string& inputString);

    /**
     * @brief Генерация соли
     * 
     * @details Генерирует соль для безопасного хранения паролей.
     * @return std::string Сгенерированная соль.
     */
    std::string generateSalt();
};
