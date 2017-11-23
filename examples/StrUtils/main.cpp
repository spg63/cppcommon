#include "../../src/StrUtils.hpp"
#include <iostream>
#include <vector>

int main(){
    //----- removeCharFromStr    
    std::string has_too_many_fs = "Thifs sftfrfifnfg ifs bfrokefn";
    std::cout << "Str before removal of f's: " << has_too_many_fs << '\n';
    auto removed_str = StrUtils::removeCharFromStr(has_too_many_fs, 'f');
    std::cout << "After removal: " << removed_str << "\n\n";
    
    //----- toUpper && toLower
    std::string upper = "ALL UPPERCASE";
    std::string lower = "all lowercase";
    
    auto to_lower = StrUtils::toLower(upper);
    auto to_upper = StrUtils::toUpper(lower);

    std::cout << "toLower: " << to_lower << '\n';
    std::cout << "toUpper: " << to_upper << "\n\n";

    //----- trim
    std::string nonsense = "a b\bro\tk\nen st\r\vr\ti\nng";
    std::cout << "Broken string: " << nonsense << '\n';
    auto trimmed = StrUtils::trim(nonsense);
    std::cout << "Trimmed string: " << trimmed << "\n\n";

    //----- trimStrVec
    std::vector<std::string> broken_str_vec{"\tstr1", "st\tr2", "s\nt\tr3"};
    for(auto &&str : broken_str_vec)
        std::cout << "Broken string: " << str << '\n';
    auto trimmed_str_vec = StrUtils::trimStrVec(broken_str_vec);
    for(auto &&str : trimmed_str_vec)
        std::cout << "Trimmed string: " << str << '\n';
    std::cout << '\n';

    //----- parseOnCharDelim
    std::string csv_string = "sean,grimes,bob,dole";
    std::cout << "csv_string: " << csv_string << '\n';
    auto parsed_strings = StrUtils::parseOnCharDelim(csv_string, ',');
    for(auto &&str : parsed_strings)
        std::cout << "Parsed str: " << str << '\n';

    //----- timeStamp
    std::cout << "\nCurrent time: " << StrUtils::timeStamp() << '\n';

    std::cout << std::endl;

    return 0;
}
