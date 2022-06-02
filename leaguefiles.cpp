#ifndef leaguefiles
#define leaguefiles
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

class leagueDBfind {
        std::string installlocation, XDG_DATA_HOME, databaselocation, lockfile="0";
        sqlite3 *database;
    public:
    leagueDBfind() { // constructor tries to find the location of the lutris database with the XDG_DATA_HOME environmental variable. If it doesn't, it gets set to ~/.local/share/lutris/pga.db
        char* env = std::getenv("XDG_DATA_HOME");
        XDG_DATA_HOME = env ? env : "";

        databaselocation = XDG_DATA_HOME.empty() ? "" : XDG_DATA_HOME+"/lutris/pga.db";
        if (XDG_DATA_HOME.empty()) {
            std::cout << "Environmental var: XDG_DATA_HOME doesn't exist. defaulting to ~/.local/share\n";
            XDG_DATA_HOME=static_cast<std::string>(std::getenv("HOME"))+"/.local/share";
            databaselocation = XDG_DATA_HOME + "/lutris/pga.db";
        }
        //std::cout << "Using databaselocation: " << databaselocation << std::endl;;
    }
    int open() { //give sqlite3 object the location of the lutris database
        return sqlite3_open(databaselocation.c_str(), &database);
    }
    static int callback(void *currentDB, int a, char **res, char **azColName) { //gets called by getinstalllocation.
        static_cast<leagueDBfind *>(currentDB)->installlocation = *res;
        static_cast<leagueDBfind *>(currentDB)->lockfile = static_cast<leagueDBfind *>(currentDB)->installlocation+"/drive_c/Riot Games/League of Legends/lockfile";
        return 0;
    }
    int getinstalllocation() { // run sql command to find the location league of legends lockfile
        return sqlite3_exec(database, "SELECT \"directory\" FROM \"main\".\"games\" WHERE \"slug\" LIKE 'league-of-legends'", callback, this, NULL);
    }
    const std::string& getlocklocation() { return lockfile; }
};

std::ifstream getfile() { //Get credentials from lockfile
#ifdef _WIN32  // i haven't tried this program with windows at all. I wanted to but I don't know how to cross compile for windows
    std::ifstream lockfile("C:\\Riot Games\\League of Legends\\lockfile");
#else
    static leagueDBfind database;
    std::ifstream lockfile;
    if (database.getlocklocation() != "0" && lockfile.is_open()) { //if the lockfile is already set to something, then it doesn't need to be found again
                                                          //i think this whole getfile() function looks weird but i don't know how to make it better
        lockfile.clear(std::ifstream::eofbit);
        return std::ifstream(database.getlocklocation());
    }
    int errcode = database.open();
    //if the database wasn't opened properly
    if (errcode) std::cerr << "couldn't open database to find league of legends install location";
    database.getinstalllocation();
    lockfile.open(database.getlocklocation());
    std::cout << "lockfile location: " << database.getlocklocation() << std::endl;
#endif
    while (!lockfile.is_open()) { // if it didn't get anything, something went wrong. Retry until process is killed
        std::cout << "League isn't open yet or something else went wrong. Retrying..." << std::endl;
        lockfile.clear(std::ifstream::eofbit);
        lockfile.open(database.getlocklocation());
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return lockfile;
}


std::vector<std::string> getdata(std::ifstream &lockfile) {
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

/*int main() {
  leagueDBfind database;
  int code = database.open();
  if (code) {
  std::cerr << "couldn't open database to find league of legends install location";
  return 1;
  }
  code = database.getinstalllocation();
  }*/
#endif
