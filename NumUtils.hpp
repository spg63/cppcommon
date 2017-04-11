//
//  NumUtils.hpp
//  cppcommon
//
//  Created by Sean Grimes on 11/26/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string.h>
#include <float.h>
#include <random>
#include <limits.h>
#include <cstdint>
#include <unordered_map>
#include <utility>

/**
 \brief Utility functions related to numerics
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 11-26-15
*/
namespace NumUtils{
    template<class T>
    inline T getMedian(std::vector<T> &vec);
    
    template<class T>
    inline T maxValueVec(const std::vector<T> &vec);
    
    template<class T>
    inline T minValueVec(const std::vector<T> &vec);
    
    template<class T>
    inline std::pair<T, T> minMaxInVec(const std::vector<T> &vec);
    
    inline int strToInt(const std::string &s);
    inline double strToDouble(const std::string &s);
    inline int sumOfDigits(int x);
    inline size_t fibonacci_lowmemory(int x);
    inline size_t fibonacci(int x);
    inline int randomInt(int start, int end);
    inline double randomDouble(double start, double end);
    inline size_t factorial(unsigned int x);
    
    template<class T>
    inline double getRadians(T degrees){ return degrees * M_PI / 180; }
}

/**
    \brief Gets median
    @param vec vector of elements to get the median from
    @return the median
    @author Andrew W.E. McDonald
 */
template<class T>
T NumUtils::getMedian(std::vector<T> &vec){
    size_t v_size = vec.size();
    // will automatically round down due to int division, subtract 1 due to 0 indexing
    size_t mid = (v_size/2) - 1;
    size_t midpo = mid + 1;
    if (v_size % 2 == 1){
        std::nth_element(vec.begin(), vec.begin()+(midpo), vec.end(),
                         [](T const a, T const b){
                             return b > a;
                         });
        return vec[midpo];
    }
    else{
        std::nth_element(vec.begin(), vec.begin()+(mid), vec.end(),
                         [](T const a, T const b){
                             return b > a;
                         });
        T m1 = vec[mid];
        std::nth_element(vec.begin(), vec.begin()+(midpo), vec.end(),
                         [](T const a, T const b){
                             return b > a;
                         });
        T m2 = vec[midpo];
        return ((m1+m2)/2.f);
        
    }
}

/**
    \brief get the max element in vector
    \details Only works for std::is_arithmetic types
    @param vec The vector to be searched
    @return: T The largest value
    @return: -1 If vector has 0 elements
*/
template<typename T>
T NumUtils::maxValueVec(const std::vector<T> &vec){
    auto max = std::max_element(std::begin(vec), std::end(vec));
    return vec[std::distance(std::begin(vec), max)];
}

/**
    \brief get the min element in vector
    \details Only works for std::is_arithmetic types
    @param vec The vector to be searched
    @return: T The smallest value
    @return: -1 If vector has 0 elements
*/
template<typename T>
T NumUtils::minValueVec(const std::vector<T> &vec){
    auto min = std::min_element(std::begin(vec), std::end(vec));
    return vec[std::distance(std::begin(vec), min)];
}

/**
    \brief Get the min and max element in a vector between range
    @param vec The vector to be searched
    @return: A std::pair<T, T> of the min and max element
        pair.first - holds the min value of vec \n
        pair.second - holds the max value of vec
*/
template<typename T>
std::pair<T, T> NumUtils::minMaxInVec(const std::vector<T> &vec){
    return std::make_pair(minValueVec(vec), maxValueVec(vec));
}

/**
    \brief Convert std::string to int
    @param s The string to be converted to an int
    @return int
    @throws std::runtime_error if conversion fails
*/
int NumUtils::strToInt(const std::string &s){
    char *end;
    long l;
    char *c_str = new char[s.length() + 1];
    strcpy(c_str, s.c_str());
    std::string err = "strToInt Error: ";
    errno = 0;
    l = strtol(c_str, &end, 10);
    if((errno == ERANGE && l == LONG_MAX) || l > INT_MAX)
        throw std::runtime_error(err + "Integer overflow on conversion.");
    if((errno == ERANGE && l == -LONG_MAX) || l < -INT_MAX)
        throw std::runtime_error(err + "Integer underflow on conversion.");
    if(*c_str == '\0' || *end != '\0')
        throw std::runtime_error(err + "Cannot convert " + s + " to an integer.");
    
    delete[] c_str;
    c_str = nullptr;
    
    return (int) l;
}

/**
    \brief Convert std::string to double
    @param s The string to be converted to a double
    @return double
    @throws std::runtime_error if conversion fails
*/
double NumUtils::strToDouble(const std::string &s){
    char *end;
    double d;
    char *c_str = new char[s.length() + 1];
    strcpy(c_str, s.c_str());
    std::string err = "strToDouble Error: ";
    errno = 0;
    d = strtod(c_str, &end);
    if((errno == ERANGE && d == DBL_MAX) || d > DBL_MAX)
        throw std::runtime_error(err + "Double overflow on conversion.");
    if((errno == ERANGE && d == -DBL_MAX) || d < -DBL_MAX)
        throw std::runtime_error(err + "Double underflow on conversion.");
    if(*c_str == '\0' || *end != '\0')
        throw std::runtime_error(err + "Cannot convert " + s + " to a double.");
    
    delete[] c_str;
    c_str = nullptr;
    
    return (double) d;
}
    
/**
 \brief CS101 function
 \details Sum the digits of an integer
 @param x The int to be summed
 @return The sum
*/
int NumUtils::sumOfDigits(int x){
    if(x < 0)
        return -sumOfDigits(-x);
    if(x < 10)
        return x;
    return (x % 10 + sumOfDigits(x / 10));
}
    
/**
    \brief Get the fibonacci number, slowly
    \note Be cautious of overflows on large numbers
    @param x The input number
    @return The fibonacci number
*/
size_t NumUtils::fibonacci_lowmemory(int x){
    if(x <= 1)
        return x;
    return(fibonacci_lowmemory(x - 1) + fibonacci_lowmemory(x - 2));
}

/**
 \brief Fibonacci number, using memoization
 \note Be cautious of overflows on large numbers
 \details Useful in graphic heavy applications
 @param x The input number
 @return The fibonacci number
*/
size_t NumUtils::fibonacci(int x){
    static std::unordered_map<int, size_t> memo;
    if(x <= 1)
        return x;
    if(memo.find(x) != memo.end())
        return memo[x];
    size_t res = fibonacci(x - 1) + fibonacci(x - 2);
    memo[x] = res;
    return res;
}
    
/**
    \brief Gets a random integer between the range (inclusive)
    \details Uses mt19937 and uniform_int_distribution
    @param start beginning of the range (inclusive)
    @param end end of the range (inclusive)
    @return random int between range given
*/
int NumUtils::randomInt(int start, int end){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(start, end);
    return dist(mt);
}
    
/**
    \brief Gets a random double between the range (inclusive)
    \details Uses mt19937 and uniform_real_distribution
    @param start beginning of the range (inclusive)
    @param end end of the range (inclusive)
    @return random double between range given
*/
double NumUtils::randomDouble(double start, double end){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(start, end);
    return dist(mt);
}
    
/**
    \brief Get factorial using memoization
    \note Be cautious of overflows on large numbers
    @param x input
    @return the factorial
*/
size_t NumUtils::factorial(unsigned int x){
    static std::unordered_map<unsigned int, size_t> memo;
    if(x == 0)
        return 1;
    if(memo.find(x) != memo.end())
        return memo[x];
    size_t res = x * factorial(x - 1);
    memo[x] = res;
    return res;
}
