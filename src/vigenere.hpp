#pragma once
#include <string>

namespace vigenere{
    std::string encode (std::string text, std::string key);
    std::string decode (std::string text, std::string key);
    int guess_period(std::string cipher);
    std::string guess_key(std::string cipher);
}
