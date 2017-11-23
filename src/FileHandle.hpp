//
// FileHandle.hpp
// cppcommon
//
// Created by Sean Grimes on 04/12/17
// Copyright © 2017 Sean Grimes. All rights reserved.
//

#pragma once

#include "FSUtils.hpp"

/**
    \brief Basic FileHandle class to ease interactions with files
    \details Content of file is loaded lazily, i.e. not until content is requested. Content will 
 automatically be reloaded if the file has been modified between calls for the content. 
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 04-14-17
*/
class FileHandle{
private:
    std::vector<std::string> content_vec_;
    std::string content_str_    {""};
    std::vector<char> binary_vec_;
    std::string path_           {""};
    size_t raw_size_            {0};
    size_t line_count_          {0};
    std::string permissions_    {""};
    std::string last_access_    {""};
    std::string last_modified_  {""};
    // 0 for file, 1 for dir
    short file_or_dir_          {-1};

    bool hasBeenModified(){
        auto modded = FSUtils::getModifiedTime(path_);
        if(last_modified_ == modded)
            return false;
        last_modified_ = modded;
        return true;
    }


public:
    FileHandle(const std::string &path) : FileHandle(path, false) {}
    FileHandle(const std::string &path, bool overwrite_if_existing) : path_(path) {
        /*
        if(FSUtils::isFile(path_))
            file_or_dir_ = 0;
        else if(FSUtils::isDir(path_))
            file_or_dir_ = 1;
         */
        // Check if it exists
        if(FSUtils::fexists(path_)){
           // if(FSUtils::isFile(path_)) file_or_dir_ = 0;
            //else if(FSUtils::isDir(path_)) file_or_dir_ = 1;
            // Overwrite it if necessary
            if(overwrite_if_existing){
                auto cleared = FSUtils::clearFile(path_);
                if(!cleared){
                    FSUtils::deleteFile(path_);
                    std::ofstream out(path_);
                    out << "";
                }
            }
        }
        // If it doesn't exist, create an empty file so something populates access/modified times
        else{
            std::ofstream out(path_);
            out << "";
        }
        
        last_access_ = FSUtils::getAccessTime(path_);
        last_modified_ = FSUtils::getModifiedTime(path_);
    }
    
    /**
        \brief Read a complete file as a single string
        @return A string representing the entire file
    */
    std::string readAsString(){
        if(content_str_ == "" || hasBeenModified())
            content_str_ = FSUtils::readFullFile(path_);
        return content_str_;
    }
    
    /**
        \brief Read file into vector<string> of lines
        @return A vector of strings, each string representing a line of the file
    */
    std::vector<std::string> readAsVector(){
        if(content_vec_.size() == 0 || hasBeenModified())
            content_vec_ = FSUtils::readLineByLine(path_);
        return content_vec_;
    }
    
};














