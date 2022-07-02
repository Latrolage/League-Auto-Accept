#include "leaguefiles.h"
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

leagueDBfind::leagueDBfind() : lockfile("0"),portnum("0"),password("0") { // constructor tries to find the location of the lutris database with the XDG_DATA_HOME environmental variable. If it doesn't, it gets set to ~/.local/share/lutris/pga.db
    char* env = std::getenv("XDG_DATA_HOME");
    XDG_DATA_HOME = env ? env : "";

    if (XDG_DATA_HOME.empty()) {
        std::cout << "Environmental var: XDG_DATA_HOME doesn't exist. defaulting to ~/.local/share\n";
        XDG_DATA_HOME=static_cast<std::string>(std::getenv("HOME"))+"/.local/share";
    }
    databaselocation = XDG_DATA_HOME+"/lutris/pga.db";
}
int leagueDBfind::open() { //give sqlite3 object the location of the lutris database
    return sqlite3_open(databaselocation.c_str(), &database);
}
int leagueDBfind::callback(void *currentDB, int a, char **res, char **azColName) { //gets called by getinstalllocation.
    static_cast<leagueDBfind *>(currentDB)->installlocation = *res;
    static_cast<leagueDBfind *>(currentDB)->lockfile = static_cast<leagueDBfind *>(currentDB)->installlocation+"/drive_c/Riot Games/League of Legends/lockfile";
    return 0;
}
int leagueDBfind::getinstalllocation() { // run sql command to find the location league of legends lockfile
    return sqlite3_exec(database, "SELECT \"directory\" FROM \"main\".\"games\" WHERE \"slug\" LIKE 'league-of-legends'", callback, this, NULL); //callback function does not get called when using databaselocation.c_str()
}
const std::string& leagueDBfind::getlocklocation() { return lockfile; }
const std::string& leagueDBfind::getdatabaselocation() { return databaselocation; }
const std::string& leagueDBfind::thepassword() { return password; }
const std::string& leagueDBfind::theportnum() { return portnum; }
void leagueDBfind::setpassword(const std::string& pw) { password=pw; }
void leagueDBfind::setportnum(const std::string& port) { portnum=port; }


std::ifstream getfile(leagueDBfind& database) { //Get credentials from lockfile
#ifdef _WIN32  // i haven't tried this program with windows at all. I wanted to but I don't know how to cross compile for windows
    std::ifstream lockfile("C:\\Riot Games\\League of Legends\\lockfile");
#else
    std::ifstream lockfile;
    if (database.getlocklocation() != "0") { //if the lockfile is already set to something, then it doesn't need to be found again
        lockfile.open(database.getlocklocation());
    } else {
        int errcode = database.open();
        if (errcode) std::cerr << "couldn't open database to find league of legends install location\n"; //if the database wasn't opened properly
        errcode = database.getinstalllocation();
        if (errcode) std::cerr << "couldn't find install location errorcode: " << errcode << '\n';
        lockfile.open(database.getlocklocation());
    }
    while (!lockfile.is_open()) { // if it didn't get anything, something went wrong. Retry until process is killed
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "League isn't open yet or something else went wrong. Retrying..." << std::endl;
        lockfile.clear(std::ifstream::eofbit);
        lockfile.open(database.getlocklocation());
    }
#endif
    return lockfile;
}


std::vector<std::string> getdata(std::ifstream &lockfile) { //parses the lockfile for the portnum and password (which is in base64 in the lockfile)
    std::vector<std::string> svec;
    std::string portnum("");
    std::string pass("riot:");
    char ch;
    for (int coloncount = 0; !lockfile.eof();) {
        if (lockfile >> ch && ch == ':') {
            ++coloncount;
        } else switch (coloncount) {
            case 2:
                portnum += ch;
                break;
            case 3:
                pass += ch;
                break;
        }
    }
    svec.push_back(std::string(portnum));
    svec.push_back(std::string(pass));
    return svec;
}
