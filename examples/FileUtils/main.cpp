#include "../../FileUtils.hpp"
#include <iostream>
#include <vector>

int main(){
    const std::string LICENSE = "../../LICENSE";
    //----- readFullFile
    auto license_str = FileUtils::readFullFile(LICENSE);
    std::cout << license_str << std::endl;
    std::cout << std::endl;

    //----- readLineByLine
    auto license_vec = FileUtils::readLineByLine(LICENSE);
    for(auto &&str : license_vec)
        std::cout << str << "\n";
    std::cout << std::endl;

    //----- lineCount
    auto license_lc = FileUtils::lineCount(LICENSE);
    std::cout << "# of lines in LICENSE: " << license_lc << "\n";
    std::cout << std::endl;

    //----- getFilesInDir
    auto files_in_dir = FileUtils::getFilesInDir(".");
    for(auto &&file : files_in_dir)
        std::cout << file << '\n';
    std::cout << std::endl;
    
    //----- mkdirWrapper
    auto success = FileUtils::mkdirWrapper("test_dir");
    if(success) std::cout << "mkdirWrapper successful for 'test_dir'\n";
    else std::cout << "mkdirWrapper failed for 'test_dir'\n";
    std::cout << std::endl;

    //----- getDirsInDir
    auto dirs_in_dir = FileUtils::getDirsInDir(".");
    for(auto &&dir : dirs_in_dir)
        std::cout << dir << '\n';
    std::cout << std::endl;

    //----- isFile
    std::cout << "isFile (main.cpp): " << FileUtils::isFile("main.cpp") << "\n";
    std::cout << "isFile (test_dir): " << FileUtils::isFile("test_dir") << "\n";
    std::cout << std::endl;

    //----- isDir
    std::cout << "isDir (test_dir): " << FileUtils::isDir("test_dir") << "\n";
    std::cout << "isDir (main.cpp): " << FileUtils::isDir("main.cpp") << "\n";
    std::cout << std::endl;

    //----- isExc
    std::cout << "isExc (out): " << FileUtils::isExc("out") << "\n";
    std::cout << "isExc (main.cpp): " << FileUtils::isExc("main.cpp") << "\n";
    std::cout << std::endl;

    //----- fexists
    std::cout << "fexists (main.cpp): " << FileUtils::fexists("main.cpp") << "\n";
    std::cout << "fexists (fake_file.txt): " << FileUtils::fexists("fake_file.txt") << "\n";
    std::cout << std::endl;

    //----- endsWithString
    std::string str{"this string"};
    std::string end{"string"};
    std::string not_end{"blah"};
    std::cout << "endsWithString (this string)[string]: " << FileUtils::endsWithString(str, end) 
              << "\n";
    std::cout << "endsWithString (this string)[blah]: " << FileUtils::endsWithString(str, not_end)
              << "\n";
    std::cout << std::endl;
    
    //----- appendToFile
    std::string fake_csv_str{"sean,grimes\nbob,dole"};
    std::string fake_csv_file{"fake.csv"};
    FileUtils::appendToFile(fake_csv_file, fake_csv_str);

    //----- csvToMatrix
    auto csv = FileUtils::csvToMatrix(fake_csv_file);
    if(csv.size() != 0 && csv[0].size() != 0)
        std::cout << "appendToFile successful\n";
    else
        std::cout << "appendToFile failed\n";
    std::cout << std::endl;

    for(auto &&row : csv)
        for(auto &&item : row)
            std::cout << "csv item: " << item << '\n';
    std::cout << std::endl;

    return 0;
}
