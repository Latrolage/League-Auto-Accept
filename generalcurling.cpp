#ifndef curling
#define curling

#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <regex>
#include "generalcurling.h"
std::string chatmedata="", methodtype;
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    if (methodtype == "GET") chatmedata = "";
    if (methodtype == "PUT") return 0;
    for (size_t i(0); i != nmemb; ++i) {
        if (methodtype == "GET")
            chatmedata += ptr[i];
        else {
            //std::cout << ptr[i];
        }
    }
    //std::cout << std::endl;
    return 0;
}
void curlstuff(const std::string& port, const std::string& pass,const std::string location, const std::string method, const short status, std::string* postdata) {
    methodtype = method;
    //CURLcode ret; //unused
    CURL* curl;
    struct curl_slist *slist1;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, (static_cast<std::string>("Authorization: Basic ") + pass).c_str());
    curl = curl_easy_init();
    std::stringstream urlstring;
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
    urlstring << "https://127.0.0.1:" << port << location;
    curl_easy_setopt(curl, CURLOPT_URL, urlstring.str().c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    if (method == "GET") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    } else if (method == "PUT") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        slist1 = curl_slist_append(slist1, "Content-Type: application/json");
        switch (status) {
            case ONLINE:
            if (chatmedata.find(":\"online") != std::string::npos) {
                std::cout << "Already online";
                return;
            }
            chatmedata = std::regex_replace(chatmedata, std::regex("(.*)\"availability\":\".*?\"(.*)"), "$01\"availability\":\"online\"$02");
            break;

        case OFFLINE:
            if (chatmedata.find("offline") != std::string::npos) {
                std::cout << "Skip fakeoffline, (already offline)\n";
                return;
            }
            chatmedata = std::regex_replace(chatmedata, std::regex("(.*)\"availability\":\".*?\"(.*)"), "$01\"availability\":\"offline\"$02");
            break;
        }
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, chatmedata.c_str());
    }
    else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    }
    curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl=NULL;
    curl_slist_free_all(slist1);
    slist1=NULL;
}

#endif
