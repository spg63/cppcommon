#include "../../src/NumUtils.hpp"
#include <iostream>

int main(){
    //----- getMedian
    std::vector<int> int_vec{1,10,15,3,12,4,19,3,29};
    std::cout << "vector: \n";
    for(auto &&i : int_vec)
        std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "vector's median: " << NumUtils::getMedian(int_vec) << "\n";
    std::cout << std::endl;

    //----- maxValue
    std::cout << "Max value in vector: " << NumUtils::maxValueVec(int_vec) << "\n";
    std::cout << std::endl;

    //----- minValue
    std::cout << "Min value in vector: " << NumUtils::minValueVec(int_vec) << "\n";
    std::cout << std::endl;

    //----- minMaxInVec
    auto minimax_res = NumUtils::minMaxInVec(int_vec);
    std::cout << "vector min: " << minimax_res.first << "   vector max: " << minimax_res.second << "\n";
    std::cout << std::endl;

    //----- strToInt
    std::cout << "String number: 5050\n";
    std::cout << "After converting to int: " << NumUtils::strToInt("5050") << "\n";
    std::cout << std::endl;

    //----- strToDouble
    std::cout << "String number: 5050.505025\n";
    std::cout << "After converting to double: " << NumUtils::strToDouble("5050.505025") << "\n";
    std::cout << std::endl;

    //----- sumOfDigits
    std::cout << "sumOfDigits (12345): " << NumUtils::sumOfDigits(12345) << "\n";
    std::cout << std::endl;

    //----- fibonacci
    std::cout << "fibonacci # of 75: " << NumUtils::fibonacci(75) << "\n"; 
    std::cout << std::endl;

    //----- randomInt
    std::cout << "randomInt (1, 10): " << NumUtils::randomInt(1, 10) << "\n";
    std::cout << std::endl;

    //----- randomDouble
    std::cout << "randomDouble (0, 1): " << NumUtils::randomDouble(0, 1) << "\n";
    std::cout << std::endl;

    //----- factorial
    std::cout << "factorial (10): " << NumUtils::factorial(10) << "\n";
    std::cout << std::endl;

    return 0;
}
