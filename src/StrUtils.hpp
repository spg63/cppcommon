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
    inline std::string trimSpecific(const std::string &s, const std::string &nonPrintCharsToRemove, bool removeWS);
    inline std::string trim(const std::string &s);
    inline std::vector<std::string> trimSpecificVec(const std::vector<std::string> &vec,
                                                    const std::string &nonPrintCharsToRemove, bool removeWS);
    inline std::vector<std::string> trimStrVec(const std::vector<std::string> &vec);
    inline std::vector<std::string> parseOnCharDelim(const std::string &line, const char delim);
    inline bool endsWithString(const std::string &str, const std::string &end);
    inline bool startsWithString(const std::string &str, const std::string &start);
    inline bool contains(const std::string &theString, const std::string &theSubString);
}

namespace{
    // Removes leading and trailing whitespace from a string
    inline std::string removeLTWS(std::string &trimmed){
        if(trimmed.empty()) return "";
        // Create a new string without the leading whitespace
        auto first_char = trimmed.find_first_not_of(" ");
        auto last_char = trimmed.size() - first_char;
        std::string no_leading_ws{trimmed.substr(first_char, last_char)};

        // Reset first char to 0;
        first_char = 0;

        // Create a new string without the trailing whitespace
        last_char = no_leading_ws.find_last_not_of(" ");
        return no_leading_ws.substr(first_char, last_char + 1);
    }
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
    // Remove all standard non-printing characters and leading / trailing WS by default
    return trimSpecific(s, "\t\n\a\b\f\r\v", true);
}

/**
    \brief Clean up a string of specified non-printing characters
    \details trimSpecific removes the specified non-printing characters throughout the entire string, will remove
    leading and trailing whitespace (spaces) if removeWS is true.
    @param s The string to be cleaned up
    @param nonPrintCharsToRemove A string of non-printing characters to remove
    @param removeWS Will remove leading and trailing whitespace if true
    @return The cleaned string
 */
std::string StrUtils::trimSpecific(const std::string &s, const std::string &nonPrintCharsToRemove, bool removeWS){
    if((s.empty() || s.find_first_not_of(nonPrintCharsToRemove) == std::string::npos) && (!nonPrintCharsToRemove.empty()))
        return "";

    std::string trimmed{s};

    // Loop through the supplied characters and clean it up
    for(auto &&ch : nonPrintCharsToRemove)
        trimmed.erase(remove(std::begin(trimmed), std::end(trimmed), ch), std::end(trimmed));

    return removeWS ? removeLTWS(trimmed) : trimmed;
}

/**
 * \brief Calls trimSpecific on all string in the vector using the specific charsToRemove string
 * @param vec The vector of strings to be cleaned
 * @param nonPrintCharsToRemove A string of non-printing characters to remove
 * @param removeWS Will remove leading and trailing whitespace if true
 * @return A vector of cleaned strings
 */
std::vector<std::string> StrUtils::trimSpecificVec(const std::vector<std::string> &vec,
                                                const std::string &nonPrintCharsToRemove, bool removeWS){
    std::vector<std::string> r_vec;
    for(auto &&str : vec)
        r_vec.push_back(trimSpecific(str, nonPrintCharsToRemove, removeWS));
    return r_vec;
}

/**
    \brief Calls trim on all strings in the vector
    @param v The vector of strings to be cleaned
    @return A vector of cleaned strings
*/
std::vector<std::string> StrUtils::trimStrVec(const std::vector<std::string> &v){
    std::vector<std::string> r_vec;
    for(auto &&str : v)
        r_vec.push_back(trim(str));
    return r_vec;
}

/**
    \brief Splits a string into vector based on a specific character
    \details Check FileUtils::csvToMatrix to see how this can be used in a useful manner
    @param line The string to be parsed
    @param delim The character to split the string on
*/
std::vector<std::string> StrUtils::parseOnCharDelim(const std::string &line, char delim){
    std::vector<std::string> vec;
    std::stringstream ss{line};
    std::string tmp;
    while(std::getline(ss, tmp, delim))
        vec.push_back(tmp);
    return vec;
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

/**
 * \brief Returns true is theSubString is a substring of theString
 * @param theString The full string to be searched
 * @param theSubString The substring to look for in theString
 * @return True if theSubString is a substring of theString
 */
bool StrUtils::contains(const std::string &theString, const std::string &theSubString){
    return theString.find(theSubString) != std::string::npos;
}
