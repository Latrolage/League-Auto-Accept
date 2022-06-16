#ifndef MENUCLICKEDACTIONS_H
#define MENUCLICKEDACTIONS_H
#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <thread>
#include <QThread>
#include <chrono>
#include <bitset>

#include "leaguefiles.h"
#include "base64.h"
#include "generalcurling.h"

class menuClickedActions : public QObject
{
    Q_OBJECT
    leagueDBfind database;
    unsigned short bools;
    void authentication();
    void acceptloop();
    void offlineloop();
signals:
public slots:
        void accept(bool);
        void offline(bool);
public:
        menuClickedActions();
};

#endif // MENUCLICKEDACTIONS_H
