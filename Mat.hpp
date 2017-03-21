//
// Mat.hpp
// cppcommon
//
// Created by Sean Grimes on 12/2/16
// Copyright © 2016 Sean Grimes. All rights reserved.
//

#pragma once
#include <vector> 
#include <Vec.hpp>

const unsigned MAX_DIMS{4};

class Mat{
private:
    std::vector<std::vector<double>> matrix_;
    
    template<class T>
    void init_pts(std::vector<std::vector<T>> pts){
        static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");
    }
    
    void init_store(){
        for(auto i = 0; i < MAX_DIMS; ++i)
            matrix_.push_back(std::vector<double>{0, 0, 0, 0});
    }
    
    
public:
    Mat() { init_store(); }
    Mat(const std::vector<std::vector<double>> &mat){
        if(mat.size() > 4
        init_store();
        
        
    }
    
    
    
};
