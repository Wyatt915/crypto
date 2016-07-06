#include "substitution.hpp"
#include "vigenere.hpp"

//ROT-0 gives ABC...XYZ, ROT-1 gives BCD...YZA etc.
std::string tabula(int ROT){
    std::string caesar = "";
    for(int i = 0; i < 26; i++){
        caesar += ((i + ROT) % 26) + 'A';
    }
    return caesar;
}

namespace vigenere{
    std::string encode (std::string text, std::string key){
        std::string cipher = "";
        std::string current;
        int keyLength = key.length();
        int idx;
        for (size_t i = 0; i < text.length(); i++) {
            idx = i % keyLength;
            current = std::string(1,text[i]);
            cipher += subst::encode(current, tabula(key[idx] - 'A'), true);
        }
        return cipher;
    }

    std::string decode (std::string text, std::string key){
        std::string cipher = "";
        std::string current;
        int keyLength = key.length();
        int idx;
        for (size_t i = 0; i < text.length(); i++) {
            idx = i % keyLength;
            current = std::string(1,text[i]);
            cipher += subst::decode(current, tabula(key[idx] - 'A'), true);
        }
        return cipher;
    }
}
