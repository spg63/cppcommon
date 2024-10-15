//
// Created by Sean Grimes on 11/22/17.
//


#include "../src/StrUtils.hpp"
#include "catch.hpp"

// All caps is killing me
#define require REQUIRE
#define test_case TEST_CASE
#define require_throws REQUIRE_THROWS

test_case("removeCharFromStr"){
    std::string tooManyFs{"Thifs sftfrfifnfgf ifs bfroken"};
    std::string correctFs{"This string is broken"};
    std::string singleChar{"f"};
    std::string correctSingle{""};
    std::string empty{""};
    std::string correctEmpty{""};
    std::string nonPrintChar{"\n\b\t\n"};
    std::string correctNonPrint{"\b\t"};

    require(StrUtils::removeCharFromStr(tooManyFs, 'f') == correctFs);
    require(StrUtils::removeCharFromStr(tooManyFs, 'z') == tooManyFs);
    require(StrUtils::removeCharFromStr(singleChar, 'f') == correctSingle);
    require(StrUtils::removeCharFromStr(empty, 'a') == correctEmpty);
    require(StrUtils::removeCharFromStr(nonPrintChar, '\n') == correctNonPrint);
}

test_case("toUpper"){
    std::string fullLower{"a proper lowercase string"};
    std::string correctFull{"A PROPER LOWERCASE STRING"};
    std::string partialLower{"A pArTiAl LoWerCase STR"};
    std::string correctLower{"A PARTIAL LOWERCASE STR"};
    std::string singleChar{"a"};
    std::string correctChar{"A"};
    std::string empty{""};
    std::string correctEmpty{""};

    require(StrUtils::toUpper(fullLower) == correctFull);
    require(StrUtils::toUpper(partialLower) == correctLower);
    require(StrUtils::toUpper(singleChar) == correctChar);
    require(StrUtils::toUpper(empty) == correctEmpty);
}

test_case("toLower"){
    std::string fullUpper{"A PROPER UPPERCASE STRING"};
    std::string correctFull{"a proper uppercase string"};
    std::string partialUpper{"a PaRtIal uPpErCasE Str"};
    std::string correctUpper{"a partial uppercase str"};
    std::string singleChar{"A"};
    std::string correctChar{"a"};
    std::string empty{""};
    std::string correctEmpty{""};

    require(StrUtils::toLower(fullUpper) == correctFull);
    require(StrUtils::toLower(partialUpper) == correctUpper);
    require(StrUtils::toLower(singleChar) == correctChar);
    require(StrUtils::toLower(empty) == correctEmpty);
}

test_case("trim"){
    std::string cleanString{"This string is perfectly fine"};
    std::string correctClean{"This string is perfectly fine"};
    std::string dirtyString{"a b\tr\noken \rs\vt\tring"};
    std::string correctDirty{"a broken string"};
    std::string leadingWS{"     leading ws"};
    std::string correctLeading{"leading ws"};
    std::string trailingWS{"trailing ws   "};
    std::string correctTrailing{"trailing ws"};
    std::string bothWS{"   extra ws   "};
    std::string correctBoth{"extra ws"};
    std::string emptyStr{""};
    std::string correctEmpty{""};
    std::string npOnly{"\n\t\v"};
    std::string correctNP{""};

    require(StrUtils::trim(cleanString) == correctClean);
    require(StrUtils::trim(dirtyString) == correctDirty);
    require(StrUtils::trim(leadingWS) == correctLeading);
    require(StrUtils::trim(trailingWS) == correctTrailing);
    require(StrUtils::trim(bothWS) == correctBoth);
    require(StrUtils::trim(emptyStr) == correctEmpty);
    require(StrUtils::trim(npOnly) == correctNP);
}

test_case("trimSpecific"){
    std::string allTrimChars("\t\n\a\b\f\r\v");
    std::string noTrim("This string is perfect");
    require(StrUtils::trimSpecific(noTrim, allTrimChars, true) == noTrim);

    std::string trimWhiteSpaceOnly("   This string is nearly perfect   ");
    std::string correctWSOnly("This string is nearly perfect");
    require(StrUtils::trimSpecific(trimWhiteSpaceOnly, allTrimChars, true) == correctWSOnly);

    std::string trimCharsOnly("   This s\ntring h\b\t\aas som\ve iss\f\bues   ");
    std::string correctCharsOnly("   This string has some issues   ");
    require(StrUtils::trimSpecific(trimCharsOnly, allTrimChars, false) == correctCharsOnly);

    std::string leaveTheNPChars("  This s\ntring i\b\ts coo\fl  ");
    std::string correctNPChars("This s\ntring i\b\ts coo\fl");
    require(StrUtils::trimSpecific(leaveTheNPChars, "", true) == correctNPChars);

    std::string emptyStr("");
    require(StrUtils::trimSpecific(emptyStr, allTrimChars, true) == emptyStr);
    require(StrUtils::trimSpecific(emptyStr, "", true) == emptyStr);
    require(StrUtils::trimSpecific(emptyStr, "", false) == emptyStr);

    std::string trimAllWS{"This String Will No\nt Have Spaces"};
    std::string correctAllWS{"ThisStringWillNotHaveSpaces"};
    require(StrUtils::trimSpecific(trimAllWS, allTrimChars + " ", true) == correctAllWS);
}

test_case("trimStrVec"){
    std::vector<std::string> badStrings{"b\nad one", "  bad two  ", " \nbad\t three\n "};
    std::vector<std::string> goodStrings{"bad one", "bad two", "bad three"};
    auto res = StrUtils::trimStrVec(badStrings);
    for(auto i = 0; i < res.size(); ++i)
        require(goodStrings[i] == res[i]);
}

test_case("trimSpecificVec"){
    std::vector<std::string> badStrings{"b\nad one", " bad t\two ", "bad three"};
    std::vector<std::string> goodStrings{"bad one", "bad t\two", "bad three"};
    auto res = StrUtils::trimSpecificVec(badStrings, "\n\a\b", true);
    for(auto i = 0; i < res.size(); ++i)
        require(goodStrings[i] == res[i]);
}

test_case("parseOnCharDelim"){
    std::string fakeCSVLine{"one,two,three"};
    std::string fakeCSVLineExtraComma{"one,two,three,"};
    std::string blankCSVLine{""};
    std::string commaCSVLine{",,,"};

    auto res = StrUtils::parseOnCharDelim(fakeCSVLine, ',');
    auto resExtra = StrUtils::parseOnCharDelim(fakeCSVLineExtraComma, ',');
    require(res.size() == 3);
    require(res[0] == "one");
    require(res[1] == "two");
    require(res[2] == "three");

    require(resExtra.size() == 3);
    require(resExtra[0] == "one");
    require(resExtra[1] == "two");
    require(resExtra[2] == "three");

    auto res2 = StrUtils::parseOnCharDelim(blankCSVLine, ',');
    require(res2.size() == 0);

    auto res3 = StrUtils::parseOnCharDelim(commaCSVLine, ',');
    require(res3.size() == 3);
    for(auto i = 0; i < res3.size(); ++i)
        require(res3[i].empty());
}

test_case("endsWithString"){
    std::string test("this is a long test string");
    std::string str("string");
    std::string longStr("long test string");
    std::string complete("this is a long test string");
    std::string notEnding("stringg");
    std::string blank{};

    require(StrUtils::endsWithString(test, str));
    require(StrUtils::endsWithString(test, str));
    require(StrUtils::endsWithString(test, longStr));
    require(StrUtils::endsWithString(test, complete));
    require(!StrUtils::endsWithString(test, notEnding));
    require(!StrUtils::endsWithString(blank, complete));
    require(StrUtils::endsWithString(test, blank));
}

test_case("startsWithString"){
    std::string test("awesome string");
    std::string str("awesome");
    std::string shortStr("awe");
    std::string longStr("awesome string");
    std::string notStart("aweesome");

    require(StrUtils::startsWithString(test, str));
    require(StrUtils::startsWithString(test, shortStr));
    require(StrUtils::startsWithString(test, longStr));
    require(!StrUtils::startsWithString(test, notStart));
}

test_case("contains"){
    std::string test{"This is a string"};
    std::string str("This is");
    std::string notIn("not");
    std::string space(" ");
    std::string full("This is a string");

    require(StrUtils::contains(test, str));
    require(StrUtils::contains(test, space));
    require(!StrUtils::contains(test, notIn));
    require(StrUtils::contains(test, full));
}
