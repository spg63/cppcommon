//
// Utils.hpp
// cppcommon
//
// Created by Seam Grimes on 02/08/17.
// Copyrioght @ 2017 Sean Grimes. All rights reserved.
//

#pragma once

#include <type_traits>



namespace Utils{
    template <class T, class U>
    inline bool isSameType(T first, U second);
}

template <class T, class U>
bool Utils::isSameType(T first, U second){
    return std::is_same<T, U>::value;
}
