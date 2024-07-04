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
#include "enums.h"
#include "generalcurling.h"

class menuClickedActions : public QObject
{
    Q_OBJECT
    leagueDBfind database;
    unsigned short bools;
    void authentication();
    void acceptloop();
    void statusloop(short);
signals:
public slots:
        void accept(bool);
        void statusChange(bool, short);
public:
        menuClickedActions();
};

#endif // MENUCLICKEDACTIONS_H
