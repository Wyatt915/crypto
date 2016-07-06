#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "substitution.hpp"
#include "wordlist.hpp"
#include "vigenere.hpp"
#include "process.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

const int ALPHA = 26;

const std::string help =
"\nUsage: crypto [OPTION]...\n"
"     -a \"string\"      Automatically attempt to solve substitution ciphers.\n"
"     -d \"string\"      Decrypts a substitution cipher. Requires -k.\n"
"     -e \"string\"      Encrypts a message using the key given by the -k option.\n"
"                      If no key is given, a key will be randomly generated.\n"
"     -h               Display this message.\n";

void load_word_list(){
    init_words();
    PATTLIST.resize(LENGTH_OF_LIST);
    for (int i = 0; i < LENGTH_OF_LIST; i++){
        PATTLIST[i] = analyze::char_pattern(WORDLIST[i]);
    }
}

struct ScoredMessage{
    std::string message;
    int score;
    ScoredMessage(std::string s): message(s){ score = analyze::prob_score(s); }
};

bool operator<(const ScoredMessage& lhs, const ScoredMessage& rhs){
    if(lhs.score != rhs.score){
        return lhs.score < rhs.score;
    }
    else{ return lhs.message > rhs.message; }
}

bool operator==(const ScoredMessage& lhs, const ScoredMessage& rhs){
    return (lhs.score == rhs.score && lhs.message == rhs.message);
}

void autosolve(std::string ciphertext){
    utils::VString courseKeys = subst::solve_by_pattern(ciphertext);
    utils::VString fineKeys = subst::fine_solve(ciphertext, courseKeys);
    utils::VString incompleteKeys;

    if(fineKeys.size() > 0){
        incompleteKeys = fineKeys;
    }
    else{ incompleteKeys = courseKeys; }

    utils::VString messages;
    std::string temp;
    for (size_t i = 0; i < incompleteKeys.size(); i++){
        temp = subst::decode(ciphertext, incompleteKeys[i], true);
        messages.push_back(temp);
    }

    utils::VString keys, tempkeys;
    for (size_t i = 0; i < incompleteKeys.size(); i++){
        tempkeys = process::fill_blanks(ciphertext, messages[i], incompleteKeys[i]);
        keys.insert(keys.end(), tempkeys.begin(), tempkeys.end());
    }

    messages.clear();
    int r = 0;
    int percent = 0;
    std::vector<ScoredMessage> rankings;
    for (size_t i = 0; i < keys.size(); i++) {
        temp = subst::encode(ciphertext, keys[i], true);
        rankings.push_back(ScoredMessage(temp));
    }
    std::cout << "\n-------------------" << std::endl;
    std::sort(rankings.rbegin(), rankings.rend());//sort in reverse (greatist to least)
    std::vector<ScoredMessage>::iterator last = std::unique(rankings.begin(), rankings.end());
    rankings.erase(last, rankings.end());
    for (size_t i = 0; i < 3 && i < rankings.size(); i++) {
        std::cout << keys[i] << "   " << rankings[i].score << ":\n";
        utils::print_color(rankings[i].message);
        std::cout << "\n\n";
    }

}

void test(){
    std::string s = "In any operating system worthy of that name, including "
    "Windows, pointers don't designate locations on the memory chip directly. "
    "They are locations in process-specific virtual memory space and the "
    "operating system then allocates parts of the physical memory chip to store "
    "the content of the parts where the process actually stores anything on demand";
    utils::sanitize(s);
    std::cout << "Plaintext:" << std::endl << s << "\n\n";
    std::string key = subst::null_key();
    utils::shuffle(key);
    std::cout << "Randomly generated key: " << key << "\n\n";
    subst::encode(s, key);
    std::cout << "Ciphertext:\n" << s << "\n\n";
    autosolve(s);
}

std::string read(std::string filename){
    std::string line;
    std::stringstream data;
    std::ifstream infile(filename.c_str());
    if(infile.is_open()){
        while(std::getline(infile,line)){
            data << line;
        }
        infile.close();
    }
    return data.str();
}

void write(std::string filename, std::string data){
    std::ofstream outfile (filename, std::ios::out);
    if(outfile.is_open()) {
        outfile << data;
        outfile.close();
    }
}

void crossword(std::string part){
    std::vector<std::string> matches = process::match_partial_words(part);
    std::vector<ScoredMessage> scored;
    for(std::string s : matches){
        scored.push_back(ScoredMessage(s));
    }
    std::sort(scored.rbegin(), scored.rend());
    for (size_t i = 0; i < 100 && i < scored.size(); i++) {
        std::cout << scored[i].message << std::endl;
    }
}

int main(int argc, char* argv[]){
    load_word_list();
    int c;
    std::string avalue_str;
    std::string dvalue_str;
    std::string evalue_str;
    std::string kvalue_str;
    std::string ovalue_str;
    bool a_opt = false;
    bool d_opt = false;
    bool e_opt = false;
    bool f_opt = false;
    bool k_opt = false;
    bool o_opt = false;
    while ( (c = getopt(argc, argv, "a:c:d:e:k:o:fht")) != -1 ) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
            case 'a':
            avalue_str = std::string(optarg);
            a_opt = true;
            break;
            case 'c':
            crossword(std::string(optarg));
            return 0;
            case 'd':
            d_opt = true;
            dvalue_str = std::string(optarg);
            break;
            case 'e':
            e_opt = true;
            evalue_str = std::string(optarg);
            break;
            case 'f':
            f_opt = true;
            break;
            case 'h':
            std::cout << help << std::endl;
            return 0;
            case 'k':
            k_opt = true;
            kvalue_str = std::string(optarg);
            break;
            case 'o':
            o_opt = true;
            ovalue_str = std::string(optarg);
            break;
            case 't':
            test();
            break;
            default:
            std::cout << "Bruh." << std::endl;
            break;
        }
    }

    if(a_opt){
        if(f_opt){ avalue_str = read(avalue_str); }
        utils::sanitize(avalue_str);
        try{
            autosolve(avalue_str);
        }
        catch(const char* e){
            std::cerr << e << std::endl;
            return 0;
        }
        catch(...){
            std::cerr << "Unknown exception" << std::endl;
        }

        return 0;
    }
    if(e_opt){
        if(f_opt){ evalue_str = read(evalue_str); }
        if(!k_opt){
            kvalue_str = subst::null_key();
            utils::shuffle(kvalue_str);
            std::cout << "Using randomly generated key: " << kvalue_str << std::endl;
        }
        utils::sanitize(evalue_str);
        subst::encode(evalue_str, kvalue_str);
        if(!o_opt){ std::cout << evalue_str << std::endl; }
        else{ write(ovalue_str, evalue_str); }
    }
    if(d_opt && k_opt){
        if(f_opt){ dvalue_str = read(dvalue_str); }
        utils::sanitize(dvalue_str);
        subst::decode(dvalue_str, kvalue_str);
        std::cout << dvalue_str << std::endl;
    }



    return 0;
}
