#ifndef curling
#define curling

#include <curl/curl.h>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <iostream>
#include <sstream>
#include <regex>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <string>
#include <QEventLoop>
#include <QSslError>
#include <QString>

#include "generalcurling.h"

std::string chatmedata="";
void qwrite_callback(const std::string &str, const std::string methodtype) {
    if (methodtype == "PUT") return;
    if (methodtype == "GET") {
        chatmedata = str;
    }
    return;
}
void qurling::finishedqurling() {
    std::cout << "FINISHED QURLING" << std::endl;
}
void qurling::qurlstuff(const std::string& port, const std::string& pass, const std::string& location, const std::string method, const short status, std::string* postdata) {
    QList<QSslError> ignoreErrors;
    ignoreErrors << QSslError(QSslError::SelfSignedCertificate) << QSslError(QSslError::CertificateUntrusted);
    QUrl clientURL(QString("https://127.0.0.1:%1%2").arg(QString::fromStdString(port), QString::fromStdString(location)));
    QNetworkRequest request(clientURL);
    request.setRawHeader("Authorization", "Basic " + QString::fromStdString(pass).toLocal8Bit());
    QNetworkAccessManager networkManager;
    QNetworkReply *reply;
    if (method == "GET") {
        reply = networkManager.get(request);
    } else if (method == "PUT") {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        switch (status) {
        case ONLINE:
            if (chatmedata.find(":\"online") != std::string::npos) {
                return;
            }
            chatmedata = std::regex_replace(chatmedata, std::regex("(.*)\"availability\":\".*?\"(.*)"), "$01\"availability\":\"chat\"$02");
            break;
        case OFFLINE:
            if (chatmedata.find("offline") != std::string::npos) {
                return;
            }
            chatmedata = std::regex_replace(chatmedata, std::regex("(.*)\"availability\":\".*?\"(.*)"), "$01\"availability\":\"offline\"$02");
            break;
        }
        reply = networkManager.put(request, chatmedata.c_str());
    } else {
        reply = networkManager.post(request, QByteArrayLiteral(""));
    }
    reply->ignoreSslErrors();
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    qwrite_callback(reply->readAll().toStdString(), method);
    /*if (reply->error() == QNetworkReply::NoError) {
        qwrite_callback(reply->readAll().toStdString());
        std::cout << reply->readAll().toStdString() << std::endl;
    } else {
        std::cout << reply->errorString().toStdString() << std::endl;
    }*/
    return;
}

void curlstuff(const std::string& port, const std::string& pass, const std::string& location, const std::string method, const short status, std::string* postdata) {
    qurling qurlstuff;
    qurlstuff.qurlstuff(port, pass, location, method, status, postdata);
    return;
}

#endif
