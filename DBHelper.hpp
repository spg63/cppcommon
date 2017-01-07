//
//  DBHelper.hpp
//  cppcommon
//
//  Created by Sean Grimes on 8/14/15.
//  Copyright © 2015. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <string.h>
#include <iostream>

/**
 \brief Easy to use sqlite3 interface for c++
 \details dbHelper provides an easy to use interface to sqlite3. Most sqlite3 functionality is available through this class by passing SQL queries to the provided functions; however it is not a full featured c++ wrapper for sqlite3. If more advanced features are needed I recommend looking elsewhere. Defining "DB_NO_EXCEPT" at compile-time will disable exceptions for dbHelper and make available a new variable, db_errno, which will be set to 0 on success and -1 on failure of any function calls.
 \author Sean Grimes, spg63@cs.drexel.edu
 \date 8-14-15
*/

class DBHelper{
public:
#ifdef DB_NO_EXCEPT
    int db_errno{0};
#endif
    
    /**
     @param file_path Path to location of sqlite3 db, or where one will be created
    */
    // Constructor will automatically try connecting to the supplied db
    DBHelper(std::string file_path) : path_to_db_(file_path) {}
    
    ~DBHelper(){}
    
    /**
        @param sql_statement The sql query to be executed for non-selection commands
    */
    void execute(std::string sql_statement){
        int rc = execute_generic_statement(sql_statement);
#ifdef PRINT_LIB_ERRORS
        if(rc != 0)
            fprintf(stderr, "create_table error -- error code: %d\n", rc);
#endif
    }
    
    // returns a vector<string> of elements returned from select statement
    // Note: caller needs do the string to int/double etc...conversion
    // You need to provide the number of results you're looking to get from the
    // query
    // i.e... select id from.... looking for 1 result, num_elements = 1
    // i.e... select * from.... need to know how many elements '*' will return
    //vector<string> select(string sql_statement, int num_elements);
    /**
        @param sql_statement The sql query used to select from the DB
        @return Results are returned as a 2D string vector which represents the columns and rows from the resultset of the select call
    */
    std::vector<std::vector<std::string>> select(std::string sql_statement){
        connect();
        
        std::vector<std::vector<std::string>> rows;
        sqlite3_stmt *stmt;
        auto sql_statement_len = sql_statement.length();
        
        // Prepare the sql statement
        rc_ = sqlite3_prepare_v2(db_, sql_statement.c_str(), (unsigned int)sql_statement_len, &stmt, NULL);
        if(rc_ != SQLITE_OK){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "Failed to prepare database %s\n", sqlite3_errstr(rc_));
#endif
#ifdef DB_NO_EXCEPT
            db_errno = -1;
            std::vector<std::vector<std::string>> failed_return;
            return failed_return;
#else
            throw std::runtime_error("Failed to prepare database " + std::string(sqlite3_errstr(rc_)));
#endif
        }
        
        // NOTE: This returns the number of columns in the result set returned by the
        //       prepared statement
        auto num_columns = sqlite3_column_count(stmt);
        
        // sqlite3 --> we're in 'c' land, so no c++ strings from DB
        char *result_p = new char[1024];
        if(result_p == nullptr){
            close();
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "select -- Couldn't allocate result_p, closing db\n");
#endif
#ifdef DB_NO_EXCEPT
            db_errno = -1;
            std::vector<std::vector<std::string>> failed_return;
            return failed_return;
#else
            throw std::runtime_error("dbHelper::select: Couldn't allocate result_p");
#endif
        }
        do{
            rc_ = sqlite3_step(stmt);
            if(rc_ == SQLITE_ROW){
                std::vector<std::string> row;
                for(int i = 0; i < num_columns; ++i){
                    // Copy data from 1 column of 1 row from DB return
                    strcpy(result_p, (char *)sqlite3_column_text(stmt, i));
                    // Use new "emplace_back" to construct string in-place
                    row.emplace_back(result_p);
                }
                // Place the columns from a single row into our rows vector
                rows.push_back(row);
            }
        }while(rc_ == SQLITE_ROW);
        
        delete[] result_p;
        result_p = nullptr;
        close();
#ifdef DB_NO_EXCEPT
        db_errno = 0;
#endif
        return rows;
    }
    
private:
    sqlite3* db_;
    char* err_msg_;
    int rc_;
    std::string path_to_db_;
    std::string table_name_;
    
private:
    int execute_generic_statement(std::string sql_statement){
        connect();
        
        if(sql_statement[sql_statement.length() - 1] != ';'){
            // Append semicolon if necessary
            sql_statement += ";";
        }
        
        // Callback can be null for some operations
        rc_ = sqlite3_exec(db_, sql_statement.c_str(), NULL, 0, &err_msg_);
        if(rc_ != SQLITE_OK){
            close();
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "SQL error: %s\n", err_msg_);
#endif
#ifdef DB_NO_EXCEPT
            db_errno = -1;
#else
            throw std::runtime_error("SQL error: " + std::string(err_msg_));
#endif
        }
        else{
            close();
#ifdef DB_NO_EXCEPT
            db_errno = 0;
#endif
            return 0;
        }
    }
    
    void connect(){
        rc_ = sqlite3_open(path_to_db_.c_str(), &db_);
        if(rc_){
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_));
#endif
#ifdef DB_NO_EXCEPT
            db_errno = -1;
#else
            throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(db_)));
#endif
        }
#ifdef DB_NO_EXCEPT
        db_errno = 0;
#endif
    }
    
    void close(){
        sqlite3_close(db_);
#ifdef DB_NO_EXCEPT
        db_errno = 0;
#endif
    }
};
