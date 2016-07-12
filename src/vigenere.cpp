#include "substitution.hpp"
#include "vigenere.hpp"
#include "analyze.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

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

    int guess_period(std::string cipher){
        double iocEnglish = 0.0654195;
        int maxPeriod = 32;
        //start with a period of 2, since a period of 1 is just simple substitution.
        std::vector<double> scores;
        for(int i = 2; i < maxPeriod && i < cipher.length(); i++){
            std::string *periodic = new std::string[i];
            for(int  j = 0; j < cipher.length(); j++){
                periodic[j % i] += cipher[j];
            }
            double avgIdxCoin = 0.0;
            for(int j = 0; j < i; j++){
                avgIdxCoin += analyze::idx_coin(periodic[j]);
            }
            scores.push_back((avgIdxCoin / double(i)) / iocEnglish);
            delete[] periodic;
        }
        int guess;
        for(int  i = 0; i < cipher.length(); i++){
            guess = i+2;//plus 2 because we started counting at 2, not 0.
            if(scores[i] > 0.95 && scores[i + guess] > 0.95){ return guess; }
        }
    }

    std::string guess_key(std::string cipher){
        int period = guess_period(cipher);
        std::string *isokeys = new std::string[period]; //parts of the ciphertext encoded with the same key.
        for(int i = 0; i < cipher.length(); i++){
            isokeys[i % period] += cipher[i];
        }
        std::string key = "";
        for(int i = 0; i < period; i++){
            std::vector<double> chi_results;
            for(int j = 0; j < 26; j++){
                chi_results.push_back(analyze::chi_sq(subst::decode(isokeys[i], tabula(j), true)));
            }
            auto min = std::min_element(begin(chi_results), end(chi_results));
            int dist = std::distance(begin(chi_results), min);
            char ch = dist + 'A';
            key += ch;
        }
        delete[] isokeys;
        return key;
    }
}
