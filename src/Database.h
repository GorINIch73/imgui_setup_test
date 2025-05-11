// Database.h  
#pragma once  
#include <sqlite3.h>  
#include <string>  
#include <vector>  
#include <map>  

class Database {  
public:  
    bool Open(const std::string& path);  
    void Close();  
    bool Execute(const std::string& query);  
    std::vector<std::map<std::string, std::string>> FetchAll(const std::string& query);  
    bool IsOpen() const { return db != nullptr; }  

private:  
    sqlite3* db = nullptr;  
};  