#include "menuclickedactions.h"

menuClickedActions::menuClickedActions() : bools(0) {
     std::thread authvariables(&menuClickedActions::authentication, this);
     authvariables.detach();
};

void menuClickedActions::authentication() {
    while(true) {
        std::ifstream lockfile = getfile(database);
        std::vector<std::string> data(getdata(lockfile));
        database.setportnum(data[0]);
        std::string tmp = Base64::Encode(data[1]);
        database.setpassword(Base64::Encode(data[1]));
        std::cout << "Port number: " << database.theportnum() << '\n' << "Auth: " << database.thepassword() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void menuClickedActions::accept(bool checked) {
    checked ? (bools |= 0b1) : (bools &= ~0b1);
    if (bools & 0b1) {
        std::thread acceptThread(&menuClickedActions::acceptloop, this);
        acceptThread.detach();
    }
}
void menuClickedActions::acceptloop() {
    while (bools & 0b1) {
        curlstuff(database.theportnum(), database.thepassword(), "/lol-matchmaking/v1/ready-check/accept", "POST");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
    return;
}
void menuClickedActions::offline(bool checked) {
    checked ? (bools |= (0b1<<1)) : (bools &= ~(0b1<<1));
    if (bools & (0b1 << 1)) {
        std::thread offlineThread(&menuClickedActions::offlineloop, this);
        offlineThread.detach();
    }
}
void menuClickedActions::offlineloop() {
    while (bools & (0b1 << 1)) {
        curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "GET"); //data[0]=portnum,data[1]=password
        curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "PUT"); //data[0]=portnum,data[1]=password
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
    return;
}
