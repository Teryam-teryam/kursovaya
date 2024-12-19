#pragma once
#include <vector>
#include <iostream>
#include <limits>
#include <cmath>

/** @file
 * @author Чувашов М.С.
 * @version 1.0
 * @date 19.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля вычислений
 */

/** @brief Класс для выполнения вычислений
 * @details Класс принимает вектор чисел и производит вычисления.
 */
class Calc
{
private:
    double result; ///< Переменная для хранения результата
public:
    /**
     * @brief Конструктор класса Calc
     * 
     * @details Инициализирует объект и выполняет вычисления на основе переданного вектора.
     * @param [in] numbers Вектор чисел для вычислений.
     */
    Calc(std::vector<double> numbers);

    /**
     * @brief Получить результат вычислений
     * 
     * @return double Результат вычислений.
     */
    double getResult() { return result; }
};
