#ifndef GENERALCURLING_H
#define GENERALCURLING_H
#pragma once
#include <QObject>
#include <QNetworkReply>

#include <enums.h>

class qurling : public QObject {
    Q_OBJECT
public slots:
    void finishedqurling();
public:
    qurling(QObject *parent = nullptr) : QObject(parent){};
    void qurlstuff(const std::string& port, const std::string& pass, const std::string& location, const std::string method, const short status=0, std::string* postdata = nullptr);
};

void qwrite_callback(const std::string &str, const std::string methodtype);
void curlstuff(const std::string& port, const std::string& pass, const std::string& location, const std::string method, const short status = 0, std::string* postdata = nullptr);
#endif // GENERALCURLING_H
