#pragma once
#include <system_error>
#include <stdexcept>
#include <string>
#include <cstdlib>

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для обработки ошибок
 */

/** @brief Класс для возбуждения критических ошибок
 * Возбуждает критические ошибки.
 */
class CritError : public std::runtime_error
{
public:
    /**
     * @brief Конструктор класса CritError
     * 
     * @param [in] message Сообщение об ошибке.
     */
    CritError(const std::string& message) : std::runtime_error(message) {}
};

/** @brief Класс для возбуждения некритических ошибок
 * Возбуждает некритические ошибки.
 */
class NoCritError : public std::runtime_error
{
public:
    /**
     * @brief Конструктор класса NoCritError
     * 
     * @param [in] message Сообщение об ошибке.
     */
    NoCritError(const std::string message) : std::runtime_error(message) {}
};
