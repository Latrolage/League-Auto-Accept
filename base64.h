#ifndef BASE64_H
#define BASE64_H
#pragma once
#include <iostream>
class Base64 {
public:
    static std::string Encode(const std::string data);
    static std::string Decode(const std::string& input, std::string& out);
};

#endif // BASE64_H
