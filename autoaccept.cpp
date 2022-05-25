#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <cstddef>
#include <fstream>
#include <curl/curl.h>
#include <sstream>
#include <vector>
#include <chrono>
#include <sqlite3.h>

#include "base64.cpp"
#include "leaguefiles.cpp"
#include "curling.cpp"

int main() {
    (argc != 1 && static_cast<std::string>(argv[1]) == "--nopkexec") ? : system("pkexec sh -c 'sysctl -w abi.vsyscall32=0' > /dev/null");
    std::cout << "STARTED LEAGUEAUTOACCEPT" << std::endl;
//    std::cout << std::unitbuf;
    while (true) {
        std::ifstream lockfile = getfile();
        std::vector<std::string> data(getdata(lockfile));
        std::cout << "Port number: " << data[0] << '\n' << "Auth: " << Base64::Encode(data[1]) << std::endl;
        curlstuff(data[0], Base64::Encode(data[1])); //data[0]=portnum,data[1]=password
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
