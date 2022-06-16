#ifndef GENERALCURLING_H
#define GENERALCURLING_H
#include <string>
#include <QtCore/QDebug>

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
void curlstuff(const std::string& port, const std::string& pass,const std::string location, const std::string method, std::string* postdata = nullptr);
#endif // GENERALCURLING_H
