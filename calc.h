#pragma once
#include <vector>
#include <iostream>
#include <limits>
#include <cmath>
class Calc
{
private:
    double result;
public:
    Calc(std::vector<double> numbers);
    double getResult() { return result; }
};
