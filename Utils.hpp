//
// Utils.hpp
// cppcommon
//
// Created by Seam Grimes on 02/08/17.
// Copyrioght @ 2017 Sean Grimes. All rights reserved.
//

#pragma once

#include <type_traits>


/**
 \brief Utility functions that don't fall into any other category
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 02-08-16
*/
namespace Utils{
    template <class T, class U>
    inline bool isSameType(T first, U second);
}

/**
    \brief Determine if two variables are of the same type
    @param first The first variable 
    @param second The second variable
    @return Returns true if both variables are of the same type, false otherwise
*/
template <class T, class U>
bool Utils::isSameType(T first, U second){
    return std::is_same<T, U>::value;
}
