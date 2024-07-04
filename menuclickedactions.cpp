#include "menuclickedactions.h"
#include <QString>

menuClickedActions::menuClickedActions() : bools(0) {
     std::thread authvariables(&menuClickedActions::authentication, this);
     authvariables.detach();
};

void menuClickedActions::authentication() {
    std::string currentPass;
    while(true) {
        std::ifstream lockfile = getfile(database);
        std::vector<std::string> data(getdata(lockfile));
        database.setportnum(data[0]);
        database.setpassword(QString::fromStdString(data[1]).toUtf8().toBase64().toStdString());
                std::cout << "4" << std::endl;
        if (database.thepassword() != currentPass) {
            currentPass = database.thepassword();
            std::cout << "Port number: " << database.theportnum() << '\n' << "Auth: " << database.thepassword() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(4));
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
void menuClickedActions::statusChange(bool checked, short newStatus) {
    if (newStatus==NONE) {
        bools &= 0b1;
        curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "PUT", ONLINE);
        return;
    }
    checked ? (bools |= (0b1<<STATUSCHANGE)) : (bools &= ~(0b1<<STATUSCHANGE));
    if (checked) {
        bools = (0b1 << newStatus) | (bools & 0b1);
    }
    std::thread statusChangeLoop(&menuClickedActions::statusloop, this, newStatus);
    statusChangeLoop.detach();
}
void menuClickedActions::statusloop(short newStatus) {
    while (bools & (0b1 << newStatus)) {
        curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "GET"); //data[0]=portnum,data[1]=password
        switch (newStatus) {
        case ONLINE:
            curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "PUT", ONLINE);
            break;
        case OFFLINE:
            curlstuff(database.theportnum(), database.thepassword(), "/lol-chat/v1/me", "PUT", OFFLINE);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
}
