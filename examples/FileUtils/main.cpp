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
    std::cout << "readFullFile(LICENSE): \n";
    auto license_str = FileUtils::readFullFile(LICENSE);
    std::cout << license_str << std::endl;
    std::cout << std::endl;

    //----- readLineByLine
    std::cout << "readLineByLine(LICENSE): \n";
    auto license_vec = FileUtils::readLineByLine(LICENSE);
    for(auto &&str : license_vec)
        std::cout << str << "\n";
    std::cout << std::endl;

    //----- lineCount
    auto license_lc = FileUtils::lineCount(LICENSE);
    std::cout << "# of lines in LICENSE: " << license_lc << "\n";
    std::cout << std::endl;

    //----- getFilesInDir
    std::cout << "Listing files in current directory:\n";
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
    std::cout << "Listing directories in current directory:\n";
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
    std::cout << "fexists (other_fake_file.txt): " << FileUtils::fexists("other_fake_file.txt") << "\n";
    std::cout << std::endl;

    //----- dexists
    std::cout << "dexists (" << TEST_DIR << "): " << FileUtils::dexists(TEST_DIR) << "\n";
    std::cout << "dexists (other_fake_dir): " << FileUtils::dexists("other_fake_dir") << "\n";
    std::cout << std::endl;

    //----- deleteFile
    std::cout << "fexists (" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    FileUtils::deleteFile(TEST_FILE);
    std::cout << "after deleteFile fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << std::endl;
    
    //----- movef
    createFile();
    std::string mf = "moved_file.txt";
    std::cout << "movef(" << TEST_FILE << ", " << mf << "): " << FileUtils::movef(TEST_FILE, mf) << "\n";
    std::cout << "fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << "fexists(" << mf << "): " << FileUtils::fexists(mf) << "\n";
    std::cout << std::endl;

    //----- copyf
    createFile();
    std::string cf = "copied_file.txt";
    std::cout << "copyf(" << TEST_FILE << ", " << cf << "): " << FileUtils::copyf(TEST_FILE, cf) << "\n";
    std::cout << "fexists(" << TEST_FILE << "): " << FileUtils::fexists(TEST_FILE) << "\n";
    std::cout << "fexists(" << cf << "): " << FileUtils::fexists(cf) << "\n";
    std::cout << "fsize(" << TEST_FILE << "): " << FileUtils::fsize(TEST_FILE) << "\n";
    std::cout << "fsize(" << cf << "): " << FileUtils::fsize(cf) << "\n";
    std::cout << std::endl;

    //----- copyd
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
    FileUtils::copyd(baseFrom, baseTo);

    //----- moved
    std::string moveFrom = "toDir";
    std::string movedTo = "movedToDir";
    std::cout << "Moving \"toDir\" to \"movedToDir\"\n";
    std::cout << "dexists(" << moveFrom << "): " << FileUtils::dexists(moveFrom) << "\n";
    std::cout << "dexists(" << movedTo << "): " << FileUtils::dexists(movedTo) << "\n";
    FileUtils::moved(moveFrom, movedTo);
    std::cout << "dexists(" << moveFrom << "): " << FileUtils::dexists(moveFrom) << "\n";
    std::cout << "dexists(" << movedTo << "): " << FileUtils::dexists(movedTo) << "\n";
    std::cout << std::endl;

    //----- dempty
    std::string anotherDir = "anotherDir";
    std::cout << "Creating empty directory " << anotherDir << "\n";
    FileUtils::makeDir(anotherDir);
    std::cout << "dexists(" << anotherDir << "): " << FileUtils::dexists(anotherDir) << "\n";
    std::cout << "dempty(" << anotherDir << "): " << FileUtils::dempty(anotherDir) << "\n";
    std::cout << "dempty(" << movedTo << "): " << FileUtils::dempty(movedTo) << "\n";
    std::cout << std::endl;

    //----- deleteDir
    std::cout << "Testing deleteDir on " << movedTo << "\n";
    std::cout << "dexists(" << movedTo << "): " << FileUtils::dexists(movedTo) << "\n";
    FileUtils::deleteDir(movedTo);
    std::cout << "dexists(" << movedTo << "): " << FileUtils::dexists(movedTo) << "\n";
    std::cout << std::endl;
    
    //----- fsize
    std::cout << "fsize(main.cpp) [bytes]: " << FileUtils::fsize("main.cpp") << " B\n";
    std::cout << "fsize(main.cpp) [kilobytes]: " << FileUtils::fsize("main.cpp", "kb") << " KB" << std::endl;
    
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

    // Clean up all the files/dirs that were created
    FileUtils::deleteFile(fake_csv_file);
    FileUtils::deleteFile(mf);
    FileUtils::deleteFile(cf);
    FileUtils::deleteFile(TEST_FILE);
    FileUtils::deleteDir(TEST_DIR);
    FileUtils::deleteDir(baseFrom);
    FileUtils::deleteDir(movedTo);
    FileUtils::deleteDir(anotherDir);

    return 0;
}























