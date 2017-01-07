//
//  main.cpp
//  cppSQLiteHelper
//
//  Created by Sean Grimes on 12/18/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#include <iostream>
#include <string>
#include "../../DBHelper.hpp"

int main(int argc, const char * argv[]) {

    // Connect to the db, this will create the db if it doesn't exist
    DBHelper db("example.db");

    // Make sure table is freshly created if this is not the first run
    std::string drop = "DROP TABLE IF EXISTS person;";
    db.execute(drop);
    
    // Create the first table in the db
    std::string create_db_and_table = 
    "CREATE TABLE IF NOT EXISTS person(\
    ID INTEGER PRIMARY KEY AUTOINCREMENT,\
    FIRST_NAME TEXT NOT NULL,\
    LAST_NAME TEXT NOT NULL);";

    // Do the creation   
    db.execute(create_db_and_table);

    // Build some insert strings for the table just created
    std::string insert1 = "INSERT INTO person (ID, FIRST_NAME, LAST_NAME)"
                          "values (NULL, 'Sean', 'Grimes');";
    std::string insert2 = "INSERT INTO person (ID, FIRST_NAME, LAST_NAME)"
                          "values (NULL, 'Git', 'Hub');";

    // Do the insertion
    db.execute(insert1);
    db.execute(insert2);
    
    // Build the selection string
    std::string select_all = "SELECT * FROM person";

    // Sample selection --> return type is std::vector<std::vector<std::string>>, 'auto' works
    auto return_all = db.select(select_all);

    // Print the return
    for(auto &&vec : return_all){
        for(auto &&str : vec){
            std::cout << " | " << str;
        }
        std::cout << std::endl;
    }

    std::string select_last = "SELECT LAST_NAME FROM person WHERE FIRST_NAME like 'Sean';";
    auto result = db.select(select_last);

    for(auto &&vec : result){
        for(auto &&str : vec){
            std::cout << "name: " << str << std::endl;
        }
    }
    return 0;
}
