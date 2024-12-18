#include "calc.h"
#include "error.h"


Calc::Calc(std::vector<double> numbers) {
    if (numbers.empty()) {
        throw NoCritError("Empty vector");
    }

    result = 0;
    double sum = 0;

    for (double number : numbers)
    {
        result += number * number;
        sum += number; 
    }    
    if (result < std::abs(sum) || std::isinf(result))
    {
        result = std::numeric_limits<double>::max();
    }
}
