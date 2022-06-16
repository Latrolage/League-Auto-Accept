#ifndef LEAGUEFILES_H
#define LEAGUEFILES_H
#pragma once
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

class leagueDBfind {
    std::string installlocation, XDG_DATA_HOME, databaselocation, lockfile, portnum, password;
    sqlite3 *database;
public:
    leagueDBfind(); // constructor tries to find the location of the lutris database with the XDG_DATA_HOME environmental variable. If it doesn't, it gets set to ~/.local/share/lutris/pga.db
    int open();
    static int callback(void *currentDB, int a, char **res, char **azColName);
    int getinstalllocation();
    const std::string& getlocklocation();
    const std::string& getdatabaselocation();
    const std::string& thepassword();
    const std::string& theportnum();
    void setpassword(const std::string& pw);
    void setportnum(const std::string& port);
};

std::ifstream getfile(leagueDBfind& database);

std::vector<std::string> getdata(std::ifstream &lockfile);

#endif // LEAGUEFILES_H
