#include <iostream>
#include <string>

#include "base64.cpp"
#include "leaguefiles.cpp"
#include "curling.cpp"

int main(int argc, char** argv) {
    if (argc != 1 && static_cast<std::string>(argv[1]) == "--nopkexec") system("pkexec sh -c 'sysctl -w abi.vsyscall32=0' > /dev/null");
    std::cout << "STARTED LEAGUEAUTOACCEPT" << std::endl;
    while (true) {
        std::ifstream lockfile = getfile();
        std::vector<std::string> data(getdata(lockfile));
        std::cout << "Port number: " << data[0] << '\n' << "Auth: " << Base64::Encode(data[1]) << std::endl;
        curlstuff(data[0], Base64::Encode(data[1])); //data[0]=portnum,data[1]=password
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
