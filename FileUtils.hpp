//
//  FileUtils.hpp
//  cppcommon
//
//  Created by Sean Grimes on 12/7/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include "StrUtils.hpp"


/**
    \brief Utility functions related to the file system
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 12-7-15
*/
namespace FileUtils{
    inline std::string readFullFile(const std::string &filename);
    inline std::vector<std::string> readLineByLine(const std::string &filename);
    inline size_t lineCount(const std::string &filename);
    inline std::vector<std::string> getFilesInDir(const std::string &dirpath, const std::string &ext = "");
    inline std::vector<std::string> getDirsInDir(const std::string &path);
    inline bool isFile(const std::string &filepath);
    inline bool isDir(const std::string &dirpath);
    inline bool isExc(const std::string &filepath);
    inline bool fexists(const std::string &filepath);
    inline bool mkdirWrapper(const std::string &dirpath);
    inline std::vector<std::vector<std::string>> csvToMatrix(const std::string &filename);
    inline void appendToFile(const std::string &filepath, const std::string &msg);
}

/**
    \brief Read full file into a single string
    \details Will read a full file, all content, into a single string
    @param fileName File name if file is in cwd, else path to file
    @return The full file as a string
*/
std::string FileUtils::readFullFile(const std::string &fileName){
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if(!ifs.is_open())
        throw std::runtime_error("Couldn't open " + fileName);
    
    std::ifstream::pos_type f_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    std::vector<char> bytes(f_size);
    ifs.read(&bytes[0], f_size);
    
    return std::string(&bytes[0], f_size);
}

/**
    \brief Read file into vector<string> of lines 
    \details Will read the full file then proceed to split it into a vector<string> of lines
    @param fileName File name if file is in cwd, else path to file
    @return A vector of strings which represent the lines of the file
*/
std::vector<std::string> FileUtils::readLineByLine(const std::string &fileName){
    std::string line;
    std::ifstream in(fileName);
    if(!in.is_open())
        throw std::runtime_error("Couldn't open " + fileName);
        
    std::vector<std::string> vec;
    vec.reserve(lineCount(fileName));
    
    while(std::getline(in, line))
        vec.push_back(line);
        
    vec.shrink_to_fit();
    return vec;
}

/**
    \brief Returns number of lines in a file
    \details This code comes almost letter for letter from the "wc" utility, I am not the author and 
    after a brief search I can't find the author on the GNU source website
    @param fileName File name if file is in cwd, else path to file
    @return Number of lines in the file
*/
size_t FileUtils::lineCount(const std::string &fileName){
    const char *fname = fileName.c_str();
    static const auto BUF_SIZE = 16*1024;
    int fd = open(fname, O_RDONLY);
    if(fd == -1){
        throw std::runtime_error("Couldn't open " + std::string(fname));
    }
        
    char buf[BUF_SIZE + 1];
    size_t lines = 0;
    
    while(size_t bytes_read = read(fd, buf, BUF_SIZE)){
        if(bytes_read == (size_t) -1)
            throw std::runtime_error("Read Failure" + std::string(fname));
        
        if(!bytes_read)
            break;
            
        for(char *p = buf; (p = (char *) memchr(p, '\n', (buf + bytes_read) - p)); ++p)
                ++lines;
    }
    close(fd);
    return lines;
}

/**
    \brief Get a list of files in directory
    @param dirPath Path to the directory in question
    @param ext Optional file extension, will only return files meeting passed file extension
    @return A vector of strings representing the files in the directory
*/
std::vector<std::string> FileUtils::getFilesInDir(const std::string &dirPath, const std::string &ext){
        std::vector<std::string> files;
        
    DIR *dir;
    struct dirent *ent;
        
    if ((dir = opendir(dirPath.c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            if(isFile(std::string(dirPath + "/" + ent->d_name))){
                // Only add files with the requested file extension
                if(ext != ""){
                    // Note: This can easily be fooled
                    std::string f_name(ent->d_name);
                    if(StrUtils::endsWithString(f_name, ext))
                        files.push_back(f_name);
                }
                // Don't care what the file extension is
                else{
                    files.emplace_back(ent->d_name);
                }
            }
        }
        closedir(dir);
    }
    else {
        // could not open directory
        throw std::runtime_error("Could not open dir " + dirPath);
    }
    return files;
}

/**
    \brief Get a list of directories in directory
    @param path Path to the directory in question
    @return A vector of strings representing the directories in the directory
*/
std::vector<std::string> FileUtils::getDirsInDir(const std::string &path){
    std::vector<std::string> dirs;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(path.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            if(isDir(std::string(path + "/" + ent->d_name)))
                dirs.emplace_back(ent->d_name);
        }
    }
    return dirs;
}

/*
 * The next 3 functions *ONLY* tell you if something you know exists is a file, dir, has exec
 * permissions. They should not be used to test if something exists
 */

/**
    \brief Determine if resource at path is a file
    @param path Path to the resource
    @return True if the resource is a file, false otherwise
*/
bool FileUtils::isFile(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISREG(buffer.st_mode);
}

/**
    \brief Determine if resource at path is a directory
    @param path Path to the resource
    @return True if the resource is a directory, false otherwise
*/
bool FileUtils::isDir(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISDIR(buffer.st_mode);
}

/**
    \brief Determine if resource at path is executable
    @param path Path to the resource
    @return True if the resource is executable, false otherwise
*/
bool FileUtils::isExc(const std::string& path){
    struct stat buffer;
    if(stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IXUSR)
        return true;
    return false;
}

/**
    \brief Wrapper to create a directory
    @param path The path to where the directory should be created
    @return True if the directory is created successfully
*/
bool FileUtils::mkdirWrapper(const std::string &path){
    if(isDir(path))
        return true;
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return isDir(path);
}

/**
    \brief Determine if a file exists
    @param path Path to the file in question
    @return True if file exists, false otherwise
*/
bool FileUtils::fexists(const std::string &path){
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

/**
    \brief Parse a csv file into a 2D vector
    \details The function parses a csv, reading the file line by line then splitting each line on 
    the commas, returning a 2D vector representing the rows/cols of the csv...i.e. a matrix.
    @param fileName File name if file is in cwd, else path to file
    @return A vector<vector<string>> that represents rows/column of the csv file
*/
std::vector<std::vector<std::string>> FileUtils::csvToMatrix(const std::string& fileName){
    auto file_lines = readLineByLine(fileName);
    std::vector<std::vector<std::string>> matrix;
    for(auto &&line : file_lines)
        matrix.emplace_back(StrUtils::parseOnCharDelim(line, ','));
    return matrix;
}

/**
    \brief Wrapper to append text to a file
    @param fileName File name if file is in cwd, else path to file
    @param msg The text to append to the file
*/
void FileUtils::appendToFile(const std::string &fileName, const std::string &msg){
    std::ofstream out(fileName, std::ofstream::app);
    out << msg << "\n";
}
