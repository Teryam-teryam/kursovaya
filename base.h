#pragma once
#include <map>
#include <string>
#include <filesystem>

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля базы данных
 */

/** @brief Класс для работы с базой данных
 * @details Контейнер dataBase хранит в себе логины и пароли пользователей.
 * Для получения данных используется метод getData().
 */
class Base
{
private:
    std::map<std::string, std::string> dataBase; ///< Контейнер "логин:пароль"
public:
    /**
     * @brief Установка соединения с базой данных
     * 
     * @details Читает данные из указанного файла и заполняет контейнер dataBase.
     * @param [in] filePath Путь к файлу базы данных.
     * @throw CritError Если файл не открывается или имеет неверный формат.
     */
    void connect(std::string filePath); 

    /**
     * @brief Получить базу данных
     * 
     * @return std::map<std::string, std::string> Контейнер с логинами и паролями пользователей.
     */
    std::map<std::string, std::string> getData() { return dataBase; }
};
