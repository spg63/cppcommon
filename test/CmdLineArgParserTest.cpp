//
// Created by Sean Grimes on 12/4/17.
//

#include "../src/CmdLineParser.hpp"
#include "catch.hpp"

#define require REQUIRE
#define test_case TEST_CASE

test_case("flag_and_variable"){
    int argc {3};
    const char *argv[] = {"/Users/xxxx/Git/the_program", "-f", "the_variable"};

    CmdLineParser cmd(argc, argv);
    require(cmd.anyOptions());
    require(cmd.numOptions() == 1);
    require(cmd.anyOptions());
    require(cmd.optExists("-f"));
    require(cmd.optExists("f"));
    require(cmd.getOptValue("-f") == "the_variable");
    require(cmd.getOptValue("f") == "the_variable");
    require(!cmd.optExists("-d"));
    require(!cmd.optExists("d"));
    require(cmd.getOptValue("-z") == "");
    require(cmd.getOptValue('z') == "");
}

test_case("multiple_flags_and_variables"){
    int argc {4};
    const char *argv[] = {"/Users/xxxx/Git/the_program", "-fvariable_one", "-d", "variable_two"};

    CmdLineParser cmd(argc, argv);
    require(cmd.anyOptions());
    require(cmd.numOptions() == 2);
    require(cmd.anyOptions());
    require(cmd.optExists("-f"));
    require(cmd.flagExists("f"));
    require(cmd.flagExists('f'));
    require(cmd.optExists("-d"));
    require(cmd.flagExists("d"));
    require(cmd.flagExists('d'));
    require(cmd.getOptValue("-f") == "variable_one");
    require(cmd.getOptValue("d") == "variable_two");
    require(!cmd.optExists("-z"));
    require(!cmd.flagExists('z'));
    require(cmd.getOptValue("z") == "");
    require(cmd.getOptValue('z') == "");
}

test_case("multiple_flags_only"){
    int argc {4};
    const char *argv[] = {"/Users/xxxx/Git/the_program", "-f", "-a", "-b"};

    CmdLineParser cmd(argc, argv);

    require(cmd.anyOptions());
    require(cmd.optExists("-f"));
    require(cmd.flagExists("f"));
    require(cmd.getOptValue("-f") == "");
    require(cmd.optExists("-a"));
    require(cmd.flagExists("a"));
    require(cmd.getOptValue("a") == "");
    require(cmd.optExists("b"));
    require(cmd.flagExists("-b"));
    require(cmd.getOptValue("b") == "");
}

test_case("single_flag_no_variable"){
    int argc {2};
    const char *argv[] = {"/Users/xxxx/Git/the_program", "-f"};
    const char *argv_noDash[] = {"/Users/xxxx/Git/the_program", "f"};

    CmdLineParser cmd(argc, argv);
    CmdLineParser no_dash(argc, argv_noDash);

    require(cmd.anyOptions());
    require(no_dash.anyOptions());
    require(cmd.numOptions() == 1);
    require(no_dash.numOptions() == 1);
    require(cmd.optExists("-f"));
    require(no_dash.optExists("-f"));
    require(cmd.flagExists("-f"));
    require(no_dash.flagExists("-f"));
    require(cmd.optExists("f"));
    require(no_dash.optExists("f"));
    require(cmd.flagExists("f"));
    require(no_dash.flagExists("f"));
    require(cmd.getOptValue("-f") == "");
    require(cmd.getOptValue("f") == "");
    require(no_dash.getOptValue("-f") == "");
    require(no_dash.getOptValue("f") == "");
    require(!cmd.optExists("z"));
    require(!no_dash.optExists("b"));
}

