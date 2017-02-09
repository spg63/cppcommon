//
//  StrUtils.hpp
//  cppcommon
//
//  Created by Sean Grimes on 11/26/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <iomanip>

/**
    \brief Utility functions focusing on string
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 11-26-15
*/
namespace StrUtils{
    inline std::string removeCharFromStr(const std::string &s, const char ch);
    inline std::string toUpper(const std::string &s);
    inline std::string toLower(const std::string &s);
    inline std::string trim(const std::string &s);
    inline std::vector<std::string> trimStrVec(const std::vector<std::string> &vec);
    inline std::vector<std::string> parseOnCharDelim(const std::string &line, const char delim);
    inline std::string timeStamp();
    inline bool endsWithString(const std::string &str, const std::string &end);
    inline bool startsWithString(const std::string &str, const std::string &start);
}

/**
    \brief Remove character from a string
    @param s The string to be modified
    @param ch The character to search for and remove
*/
std::string StrUtils::removeCharFromStr(const std::string &s, const char ch){
    std::string s_ch{ch};
    
    if(s.length() == 0)
        return "";
    if(s.length() == 1){
        if(s_ch == s)
            return "";
        return s;
    }
        
    auto end = s.length() - 1;
    std::string test_s = s.substr(0, 1);
    std::string rest = s.substr(1, end);
    if(test_s == s_ch)
        return removeCharFromStr(rest, ch);
    return test_s + removeCharFromStr(rest, ch);
}
    
/**
    \brief Convert string to all uppercase 
    @param s The string to be uppercase'd
    @return A new string, all uppercase
*/
std::string StrUtils::toUpper(const std::string &s){
    std::string cpy(s);
    std::transform(std::begin(cpy), std::end(cpy), std::begin(cpy), ::toupper);
    return cpy;
}

/**
    \brief Convert string to all lowercase
    @param s The string to be lowercase'd
    @return A new string, all lowercase
*/
std::string StrUtils::toLower(const std::string &s){
    std::string cpy(s);
    std::transform(std::begin(cpy), std::end(cpy), std::begin(cpy), ::tolower);
    return cpy;
}

/**
    \brief Clean up a string
    \details trim removes leading and trailing whitespace and all non-printing characters using the erase/remove idiom
    @param s The string to be cleaned up
    @return The cleaned string
*/
std::string StrUtils::trim(const std::string &s){
    if(s.find_first_not_of("\t\n\a\b\f\r\v ") == std::string::npos)
        return "";
        
    std::string trimmed = s;
    // Removing tab chars
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\t'), std::end(trimmed));
    // Removing new line chars
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\n'), std::end(trimmed));
    // Removing 'audible bell'
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\a'), std::end(trimmed));
    // Removing backspace
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\b'), std::end(trimmed));
    // Removing form-feed
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\f'), std::end(trimmed));
    // Removing carriage return
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\r'), std::end(trimmed));
    // Removing vertical tab
    trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), '\v'), std::end(trimmed));
        
    auto first_char = trimmed.find_first_not_of(" ");
    auto last_char = trimmed.size() - first_char;
    std::string no_leading_ws = trimmed.substr(first_char, last_char);
    // Resetting first_char to 0;
    first_char = 0;
    last_char = no_leading_ws.find_last_not_of(" ");
    std::string all_trimmed = no_leading_ws.substr(first_char, last_char + 1);
    return all_trimmed;
}

/**
    \brief Calls trim on all strings in the vector
    @param v The vector of strings to be cleaned
    @return A vector of cleaned strings
*/
std::vector<std::string> StrUtils::trimStrVec(const std::vector<std::string> &v){
    std::vector<std::string> r_vec(v.size());
    for(auto i = 0; i < v.size(); ++i)
        r_vec[i] = trim(v[i]);
    return r_vec;
}

/**
    \brief Splits a string into vector based on a specific character
    \details Check FileUtils::csvToMatrix to see how this can be used in a useful manner
    @param line The string to be parsed
    @param delim The character to split the string on
*/
std::vector<std::string> StrUtils::parseOnCharDelim(const std::string &line, const char delim){
    std::vector<std::string> vec;
    std::stringstream ss{line};
    std::string tmp;
    while(std::getline(ss, tmp, delim))
        vec.push_back(tmp);
    return vec;
}

/**
 \brief Get current time stamp using chrono
 \details time stamp format: YEAR-MONTH-DAY HOUR:MINUTE:SECOND.MILLISECOND
 @return time stamp as a string
 */
std::string StrUtils::timeStamp() {
    auto now = std::chrono::system_clock::now();
    auto count = std::chrono::system_clock::to_time_t(now);
    auto secs = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto partial = now - secs;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(partial);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&count), "%Y-%m-%d %X");
    oss << "." << ms.count();
    return oss.str();
}

/**
 \brief Determine if string ends with another string
 @param str The string to be searched
 @param end The string to search for
 @return True if str ends with end, false otherwise
 */
bool StrUtils::endsWithString(const std::string &str, const std::string &end){
    return end.size() <= str.size() && str.find(end, str.size() - end.size()) != str.npos;
}

/**
 \brief Determine if string starts with another string
 @param str The string to be searched
 @param start The string to search for
 @return True if str starts with start, false otherwise
 */
bool StrUtils::startsWithString(const std::string &str, const std::string &start){
    return start.length() <= str.length() && std::equal(std::begin(start), std::end(start), std::begin(str));
}




