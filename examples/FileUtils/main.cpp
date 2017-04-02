#include "../../FileUtils.hpp"
#include <iostream>
#include <vector>

const std::string TEST_FILE = "fake_file.txt";
const std::string TEST_DIR = "fake_dir";
const std::string TEST_TEXT = "nothing worthwhile here";

void createFile(const std::string &path = ""){
    std::ofstream f;
    if(path != "")
        f.open(path);
    else
        f.open(TEST_FILE);
    f << TEST_TEXT;
}

int main(){
    createFile();
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
    
    //----- makeDir
    auto success = FileUtils::makeDir(TEST_DIR);
    if(success) std::cout << "makeDir successful for " << TEST_DIR << "\n";
    else std::cout << "makeDir failed for " << TEST_DIR << "\n";
    std::cout << std::endl;

    //----- getDirsInDir
    auto dirs_in_dir = FileUtils::getDirsInDir(".");
    for(auto &&dir : dirs_in_dir)
        std::cout << dir << '\n';
    std::cout << std::endl;

    //----- isFile
    std::cout << "isFile (" << TEST_FILE << "): " << FileUtils::isFile(TEST_FILE) << "\n";
    std::cout << "isFile (" << TEST_DIR << "): " << FileUtils::isFile(TEST_DIR) << "\n";
    std::cout << std::endl;

    //----- isDir
    std::cout << "isDir (" << TEST_DIR << "): " << FileUtils::isDir(TEST_DIR) << "\n";
    std::cout << "isDir (" << TEST_FILE << "): " << FileUtils::isDir(TEST_FILE) << "\n";
    std::cout << std::endl;

    //----- isExc
    std::cout << "isExc (out): " << FileUtils::isExc("out") << "\n";
    std::cout << "isExc (" << TEST_FILE << "): " << FileUtils::isExc(TEST_FILE) << "\n";
    std::cout << std::endl;

    //----- fexists
    std::cout << "fexists (" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << "fexists (fake_file.txt): " << FileUtils::fexists("fake_file.txt") << "\n";
    std::cout << std::endl;

    //----- dexists
    std::cout << "dexists (" << TEST_DIR << "): " << FileUtils::dexists(TEST_DIR) << "\n";
    std::cout << "dexists (fake_dir): " << FileUtils::dexists("fake_dir") << "\n";
    std::cout << std::endl;

    //----- deleteFile
    std::cout << "fexists (" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    FileUtils::deleteFile(TEST_FILE);
    std::cout << "after deleteFile fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << std::endl;

    //----- deleteDir

    
    //----- moveFile
    createFile();
    std::string mf = "moved_file.txt";
    std::cout << "moveFile(" << TEST_FILE << ", " << mf << "): " << FileUtils::moveFile(TEST_FILE, mf) << "\n";
    std::cout << "fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << "fexists(" << mf << "): " << FileUtils::fexists(mf) << "\n";
    std::cout << std::endl;

    //----- copyFile
    createFile();
    std::string cf = "copied_file.txt";
    std::cout << "copyFile(" << TEST_FILE << ", " << cf << "): " << FileUtils::copyFile(TEST_FILE, cf) << "\n";
    std::cout << "fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << "fexists(" << cf << "): " << FileUtils::fexists(cf) << "\n";
    std::cout << "fileSize(" << TEST_FILE << "): " << FileUtils::fileSize(TEST_FILE) << "\n";
    std::cout << "fileSize(" << cf << "): " << FileUtils::fileSize(cf) << "\n";
    std::cout << std::endl;

    //----- copyDir
    std::string baseFrom = "fromDir";
    std::string baseTo = "toDir";
    FileUtils::makeDir(baseFrom);
    FileUtils::makeDir(baseFrom + "/" + "sub1");
    FileUtils::makeDir(baseFrom + "/" + "sub1" + "/" + "subsub1");
    FileUtils::makeDir(baseFrom + "/" + "sub2");
    createFile(baseFrom + "/" + "test1.file");
    createFile(baseFrom + "/sub1/" + "test1.file");
    createFile(baseFrom + "/sub1/subsub1/" + "test1.file");
    createFile(baseFrom + "/sub2/" + "test1.file");
    FileUtils::copyDir(baseFrom, baseTo);
    
    //----- fileSize
    std::cout << "filesize\n";
    
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
