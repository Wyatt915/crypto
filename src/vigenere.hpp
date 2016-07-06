#pragma once
#include <string>

namespace vigenere{
    std::string encode (std::string text, std::string key);
    std::string decode (std::string text, std::string key);
}
