#pragma once
#include <string>

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля логирования
 */

/** @brief Класс для работы с журналом логов
 * @details Позволяет записывать события в лог-файл.
 */
class Logger
{
private:
    std::string pathToLogFile; ///< Путь к файлу лога

    /**
     * @brief Получение текущего времени
     * 
     * @return std::string Строка с текущим временем и датой.
     */
    std::string getTime();

public:
    /**
     * @brief Конструктор по умолчанию
     * 
     * Инициализирует объект Logger с умолчательным путем.
     */
    Logger();

    /**
     * @brief Конструктор с указанием пути
     * 
     * @param [in] path Путь к файлу лога.
     */
    Logger(const std::string& path); 

    /**
     * @brief Установка пути к файлу лога
     * 
     * @param [in] pathFile Путь к файлу лога.
     * @return int Код результата операции.
     * @throw CritError Если файл не открывается.
     */
    int setPath(const std::string& pathFile);

    /**
     * @brief Запись события в журнал
     * 
     * @param [in] message Сообщение для записи в лог.
     * @return int Код результата операции.
     * @throw CritError Если файл не открывается на запись.
     */
    int writeLog(const std::string& message);

    /**
     * @brief Получение пути к файлу лога
     * 
     * @return std::string Путь к файлу лога.
     */
    std::string getPath() const { return pathToLogFile; }
};
