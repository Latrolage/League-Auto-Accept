#ifndef curling
#define curling

#include <curl/curl.h>
#include <iostream>
#include <sstream>

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    for (int i(0); i != nmemb; ++i) {
        std::cout << ptr[i];
    }
    return 0;
}
void curlstuff(const std::string& port, const std::string& pass) { 
    CURLcode ret;
    CURL* curl;
    struct curl_slist *slist1;
    slist1 = NULL;
    slist1 = curl_slist_append(slist1, (static_cast<std::string>("Authorization: Basic ") + pass).c_str());
    curl = curl_easy_init();
    std::stringstream urlstring;
    urlstring << "https://127.0.0.1:" << port << "/lol-matchmaking/v1/ready-check/accept";
//    std::cout << urlstring.str() << std::endl;
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
//    curl_easy_setopt(curl, CURLOPT_URL, "https://127.0.0.1:39961/lol-matchmaking/v1/ready-check/accept");
    curl_easy_setopt(curl, CURLOPT_URL, urlstring.str().c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.83.1");
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl=NULL;
    curl_slist_free_all(slist1);
    slist1=NULL;
    std::cout << '\n' << std::endl;
}
#endif
