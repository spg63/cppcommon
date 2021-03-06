//
//  FSUtils.hpp
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
#include <cstdlib>
#include <climits>
#include "StrUtils.hpp"

// Windows stat
#ifdef WIN32
    #define stat _stat
#endif
/**
    \brief Utility functions related to the file system
    \details FSUtils completely includes the older FileUtils namespace. In order to continue to use
    code written for the FileUtils namespace you need to define DEPRECATED_NAMESPACE at compile time.
    The definition will enable FileUtils by namespace FileUtils = FSUtils.
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 12-7-15
*/
namespace FSUtils{
    inline std::string readFullFile(const std::string &filename);
    inline std::vector<std::string> readLineByLine(const std::string &filename);
    inline size_t lineCount(const std::string &filename);
    inline std::vector<std::string> getFilesInDir(const std::string &dirpath,
                                                  const std::string &ext = "");
    inline std::vector<std::string> getDirsInDir(const std::string &path);
    inline bool makeDir(const std::string &dirpath);
    inline bool isFile(const std::string &filepath);
    inline bool isDir(const std::string &dirpath);
    inline bool isExc(const std::string &filepath);
    inline bool fexists(const std::string &filepath);
    inline bool dexists(const std::string &dirpath);
    inline bool dempty(const std::string &dirpath);
    inline bool deleteFile(const std::string &filepath);
    inline bool deleteDir(const std::string &dirpath);
    inline bool movef(const std::string &curpath, const std::string &newpath);
    inline bool moved(const std::string &curpath, const std::string &newpath);
    inline bool copyf(const std::string &curpath, const std::string &newpath);
    inline bool copyd(const std::string &curpath, const std::string &newpath);
    inline float fsize(const std::string &filepath, const std::string &order = "b");
    inline std::vector<std::vector<std::string>> csvToMatrix(const std::string &filename);
    inline void appendToFile(const std::string &filepath, const std::string &msg);
    inline std::string getWorkingDir();
    inline std::string getPermissions(const std::string &path);
    inline std::string getModifiedTime(const std::string &path);
    inline std::string getAccessTime(const std::string &path);
    inline bool clearFile(const std::string &path);
    
    const std::string THIS_DIR_DOT{"."};
    const std::string PREV_DIR_DOT{".."};
}

#if defined(DEPRECATED_NAMESPACE)
    namespace FileUtils = FSUtils;
#endif


/**
    \brief Read full file into a single string
    \details Will read a full file, all content, into a single string
    @param fileName File name if file is in cwd, else path to file
    @return The full file as a string
    @throws std::runtime_error if fileName can't be opened
*/
std::string FSUtils::readFullFile(const std::string &fileName){
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if(!ifs.is_open())
        throw std::runtime_error("Couldn't open " + fileName);
    
    std::ifstream::pos_type f_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    std::vector<char> bytes(f_size);
    ifs.read(bytes.data(), f_size);
    
    return std::string(bytes.data(), f_size);
}

/**
    \brief Read file into vector<string> of lines 
    \details Will read the full file then proceed to split it into a vector<string> of lines
    @param fileName File name if file is in cwd, else path to file
    @return A vector of strings which represent the lines of the file
    @throws std::runtime_error if fileName can't be opened
*/
std::vector<std::string> FSUtils::readLineByLine(const std::string &fileName){
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
    @throws std::runtime_error if fileName can't be opened
*/
size_t FSUtils::lineCount(const std::string &fileName){
    const char *fname = fileName.c_str();
    static const auto BUF_SIZE = 16*1024;
    int fd = open(fname, O_RDONLY);
    if(fd == -1)
        throw std::runtime_error("Couldn't open " + std::string(fname));
    
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
    @throws std::runtime_error if dirPath can't be opened
*/
std::vector<std::string> FSUtils::getFilesInDir(const std::string &dirPath, const std::string &ext){
        std::vector<std::string> files;
        
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(dirPath.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + dirPath);
        
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
            else
                files.emplace_back(ent->d_name);
        }
    }
    closedir(dir);
    
    return files;
}

/**
    \brief Get a list of directories in directory
    \details "." and ".." will not be returned in the list of directories
    @param path Path to the directory in question
    @return A vector of strings representing the directories in the directory
    @throws std::runtime_error if path can't be opened
*/
std::vector<std::string> FSUtils::getDirsInDir(const std::string &path){
    std::vector<std::string> dirs;
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(path.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + path);
    
    while((ent = readdir(dir)) != NULL){
        std::string it = ent->d_name;
        if(THIS_DIR_DOT == it || PREV_DIR_DOT == it)
            continue;
        if(isDir(std::string(path + "/" + it)))
            dirs.emplace_back(it);
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
bool FSUtils::isFile(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISREG(buffer.st_mode);
}

/**
    \brief Determine if resource at path is a directory
    @param path Path to the resource
    @return True if the resource is a directory, false otherwise
*/
bool FSUtils::isDir(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISDIR(buffer.st_mode);
}

/**
    \brief Determine if resource at path is executable
    @param path Path to the resource
    @return True if the resource is executable, false otherwise
*/
bool FSUtils::isExc(const std::string& path){
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IXUSR);
}

/**
    \brief Wrapper to create a directory
    @param path The path to where the directory should be created
    @return True if the directory exists at the end of this function
*/
bool FSUtils::makeDir(const std::string &path){
    if(dexists(path))
        return true;
/*
    // NOTE: I have no idea why flushing the stream is required, however without this flush only the root
    // directory is created when creating multiple directories on macOS Sierra 10.12.3 (and possibly other
    // systems)
    std::cout << std::flush;
*/
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return dexists(path) && isDir(path);
}

/**
    \brief Determine if a file exists
    @param path Path to the file in question
    @return True if file exists, false otherwise
*/
bool FSUtils::fexists(const std::string &path){
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}

/**
 \brief Determine if a directory exists
 @param path Path to the directory in question
 @return True if directory exists, false otherwise
*/
bool FSUtils::dexists(const std::string &path){
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

/**
    \breif Determine if a directory contains any files
    \details Will ignore "." and ".."
    @param dirpath The path to the directory in question
    @return True if the directory is empty
    @throws std::runtime_error if dirpath is not found
*/
bool FSUtils::dempty(const std::string &dirpath){
    if(!dexists(dirpath))
        throw std::runtime_error("Can't find " + dirpath);
    
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(dirpath.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + dirpath);
    
    while((ent = readdir(dir)) != NULL){
        std::string item(ent->d_name);
        if(THIS_DIR_DOT == item || PREV_DIR_DOT == item)
            continue;
        std::string it(dirpath + "/" + item);
        if(fexists(it) || dexists(it)) {
            closedir(dir);
            return false;
        }
    }
    closedir(dir);
    return true;
}

/**
    \brief Delete a file
    @param filepath The path to the file
    @return True if the file does not exist at the end of the function
*/
bool FSUtils::deleteFile(const std::string &filepath){
    if(!fexists(filepath))
        return true;
    
    std::remove(filepath.c_str());
    
    return !fexists(filepath);
}

/**
    \brief Delete a directory and contents
    @param dirpath The path to the directory
    @return True if the directory does not exist at the end of the function
    @throws std::runtime_error if dirpath can't be opened
*/
bool FSUtils::deleteDir(const std::string &dirpath){
    if(!dexists(dirpath))
        return true;
    
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(dirpath.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + dirpath + " for deletion");
    
    while((ent = readdir(dir)) != NULL){
        std::string it(ent->d_name);
        
        // Be very careful not to traverse vertically out of the directory
        if(THIS_DIR_DOT == it || PREV_DIR_DOT == it)
            continue;
        
        std::string relative_it(dirpath + "/" + it);
        
        if(isDir(relative_it)){
            // This really shouldn't ever be the case, since we couldn't be in this loop
            if(dempty(relative_it))
                std::remove(relative_it.c_str());
            // Make the recursive call
            else
                deleteDir(relative_it);
        }
        // Remove all the files from the directory
        else if(isFile(relative_it))
            std::remove(relative_it.c_str());
        else
            throw std::runtime_error("Is that a symlink?? Uh...not implemented!");
    }
    closedir(dir);
    
    if(isDir(dirpath) && dempty(dirpath))
        std::remove(dirpath.c_str());
    
    return !dexists(dirpath);
}

/**
    \brief Move a file 
    @param curpath The current path to the file
    @param newpath Where to move the file
    @return True if the file is successfully moved
*/
bool FSUtils::movef(const std::string &curpath, const std::string &newpath){
    if(!fexists(curpath) || !isFile(curpath))
        return false;
    
    std::rename(curpath.c_str(), newpath.c_str());
    
    return !fexists(curpath) && fexists(newpath);
}

/**
    \brief Move a directory
    @param curpath The current path to the directory
    @param newpath Where to move the file
    @return True if the directory is successfully moved
    @throws std::runtime_error if curpath can't be opened or if newpath can't be created
*/
bool FSUtils::moved(const std::string &curpath, const std::string &newpath){
    if(!dexists(curpath) || !isDir(curpath))
        return false;
    
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(curpath.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + curpath);
    
    if(!makeDir(newpath))
        throw std::runtime_error("Failed to create " + newpath);
    
    while((ent = readdir(dir)) != NULL){
        std::string it(ent->d_name);
        
        if(THIS_DIR_DOT == it || PREV_DIR_DOT == it)
            continue;
        
        std::string cur_item(curpath + "/" + it);
        std::string new_item(newpath + "/" + it);
        
        if(isFile(cur_item))
            movef(cur_item, new_item);
        else if(isDir(cur_item))
            moved(cur_item, new_item);
        else
            throw std::runtime_error("Is that a symlink?? Uh...not implemented!");
    }
    closedir(dir);
    
    if(isDir(curpath) && dempty(curpath))
        std::remove(curpath.c_str());

    return dexists(newpath);
}

/**
    \brief Copy a file
    @param curpath The current path to the file
    @param newpath Where to copy the file
    @return True if file is successfully copied
*/
bool FSUtils::copyf(const std::string &curpath, const std::string &newpath){
    if(!fexists(curpath) || !isFile(curpath))
        return false;
    if(fexists(newpath) || dexists(newpath))
        throw std::runtime_error(newpath + " already exists");
    
    {
        std::ifstream src(curpath, std::ios::binary);
        std::ofstream dst(newpath, std::ios::binary);
        dst << src.rdbuf();
    }

    return fexists(curpath) && fexists(newpath);
}

/**
    \brief Copy a directory
    @param curpath The current path to the directory
    @param newpath Where to copy the directory
    @return True if directory is successfully copied
    @throws std::runtime_error if curpath can't be opened or newpath can't be created
*/
bool FSUtils::copyd(const std::string &curpath, const std::string &newpath){
    if(!dexists(curpath) || !isDir(curpath))
        return false;
    if(fexists(newpath) || dexists(newpath))
        throw std::runtime_error(newpath + " already exists");
    
    DIR *dir;
    struct dirent *ent;
    
    if((dir = opendir(curpath.c_str())) == NULL)
        throw std::runtime_error("Couldn't open " + curpath);
    
    // Step 1, create the directory at the new path
    if(!makeDir(newpath))
        throw std::runtime_error("Failed to create " + newpath);
    
    while((ent = readdir(dir)) != NULL){
        std::string it(ent->d_name);
        // Don't want to do anything if we're on a dot
        if(THIS_DIR_DOT == it || PREV_DIR_DOT == it)
            continue;
        std::string cur_item(curpath + "/" + it);
        std::string new_item(newpath + "/" + it);
        
        if(isFile(cur_item))
            copyf(cur_item, new_item);
        // Make the recursive call
        else if(isDir(cur_item))
            copyd(cur_item, new_item);
        else
            throw std::runtime_error("Is that a symlink?? Uh...not implemented!");
    }
    closedir(dir);
    return dexists(newpath);
}

/**
    \brief Return size of file
    \details Optional second parameter to select the return value\n 
    bytes = "b"\n
    kilobytes = "kb"\n
    megabytes = "mb"\n
    gigabytes = "gb"
    @param filepath The path to the file
    @return The file size if the file exists in bytes (or in specified value, if used)
    \note This function returns a double, unlike standard size functions, to allow for partial
    values when getting size back in KB, MB, or GB
    @throws std::runtime_error if filepath can't be found
*/
float FSUtils::fsize(const std::string &filepath, const std::string &order){
    if(!fexists(filepath))
        throw std::runtime_error("Cannot find " + filepath);
    float sz;
    
    {
        std::ifstream f(filepath, std::ifstream::ate | std::ifstream::binary);
        sz = static_cast<float>(f.tellg());
    }
    
    if("b" == order)
        return sz;
    else if("kb" == order)
        return sz / 1000;
    else if("mb" == order)
        return sz / 1000000;
    else if("gb" == order)
        return sz / 1000000000;
    else
        return sz;
}


/**
    \brief Parse a csv file into a 2D vector
    \details The function parses a csv, reading the file line by line then splitting each line on 
    the commas, returning a 2D vector representing the rows/cols of the csv...i.e. a matrix.
    @param fileName File name if file is in cwd, else path to file
    @return A vector<vector<string>> that represents rows/column of the csv file
*/
std::vector<std::vector<std::string>> FSUtils::csvToMatrix(const std::string& fileName){
    auto file_lines = readLineByLine(fileName);
    std::vector<std::vector<std::string>> matrix;
    for(auto &&line : file_lines)
        matrix.emplace_back(StrUtils::parseOnCharDelim(StrUtils::trim(line), ','));
    return matrix;
}

/**
    \brief Wrapper to append text to a file
    @param fileName File name if file is in cwd, else path to file
    @param msg The text to append to the file
*/
void FSUtils::appendToFile(const std::string &fileName, const std::string &msg){
    std::ofstream out(fileName, std::ofstream::app);
    out << msg << "\n";
}

/**
    \brief Get the current working directory
    \details Will fail on path lengths > than SHRT_MAX
    @return A string representing the current working directory, blank string on failure
*/
std::string FSUtils::getWorkingDir(){
    size_t MAX_LEN = SHRT_MAX;
    std::string std_path{};
    
    {
        char *path = new char[MAX_LEN];
        getcwd(path, MAX_LEN - 1);
        if(path != nullptr) std_path = path;
        if(path != nullptr) delete[] path;
    }
    
    return std_path;
}


/**
    \brief Return the permissions (user group global) of specified file or directory
    @return A string representing the 3 digit permissions
    @throws std::runtime_error if path can't be found
*/
std::string FSUtils::getPermissions(const std::string &path){
    if(!dexists(path) && !fexists(path))
        throw std::runtime_error("Can't find " + path);
    
    struct stat buffer;
    stat(path.c_str(), &buffer);
    auto user = (buffer.st_mode & S_IRWXU) >> 6;
    auto group = (buffer.st_mode & S_IRWXG) >> 3;
    auto world = (buffer.st_mode & S_IRWXO);
    
    auto perms = std::to_string(user) + std::to_string(group) + std::to_string(world);
    return perms;
}

/**
    \brief Return the last modified time of a file or directory
    \return A time string
    @throws std::runtime_error if path can't be found
*/
std::string FSUtils::getModifiedTime(const std::string &path){
    if(!dexists(path) && !fexists(path))
        throw std::runtime_error("Can't find " + path);
    
    struct stat buffer;
    stat(path.c_str(), &buffer);
    struct tm *timeinfo = localtime(&buffer.st_mtime);
    std::string remove_newline(asctime(timeinfo));
    return remove_newline.substr(0, remove_newline.length() - 1);
}

/**
    \brief Return the last access time of a file or directory
    \return A time string
    @throws std::runtime_error if path can't be found
*/
std::string FSUtils::getAccessTime(const std::string &path){
    if(!dexists(path) && !fexists(path))
        throw std::runtime_error("Can't find " + path);
    
    struct stat buffer;
    stat(path.c_str(), &buffer);
    struct tm *timeinfo = localtime(&buffer.st_atime);
    std::string remove_newline(asctime(timeinfo));
    return remove_newline.substr(0, remove_newline.length() - 1);
}

/**
    \brief Clear the contents of a file
    \return True if file has been opened and cleared successfully
    @throws std::runtime_error if path can't be found
*/
bool FSUtils::clearFile(const std::string &path){
    if(!fexists(path))
        throw std::runtime_error("Can't find " + path);
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    return fsize(path) == 0;
}
