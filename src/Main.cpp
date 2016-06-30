#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "substitution.hpp"
#include "wordlist.hpp"
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
    return lhs.score < rhs.score;
}

void autosolve(std::string ciphertext){
    time_t wibbly = clock();
    utils::VString incompleteKeys = subst::solve_by_pattern(ciphertext);
    wibbly = clock() - wibbly;
    float elapsed = ((float)wibbly)/((float)CLOCKS_PER_SEC);
    std::cout << "\n\n" << elapsed << " seconds." << std::endl;
    std::string courseKey = incompleteKeys[0];
    subst::fine_solve(ciphertext, courseKey);
    utils::VString messages;
    std::string temp;
    for (size_t i = 0; i < incompleteKeys.size(); i++){
        subst::invert(incompleteKeys[i]);	//VERY IMPORTANT (to maintain proper mapping in the fill_blanks function)
        temp = subst::encode(ciphertext, incompleteKeys[i], true);
        messages.push_back(temp);
        utils::print_color(temp);
        std::cout << std::endl;
    }

    utils::VString keys, tempkeys;
    for (size_t i = 0; i < incompleteKeys.size(); i++){
        tempkeys = process::fill_blanks(ciphertext, messages[i], incompleteKeys[i]);
        keys.insert(keys.end(), tempkeys.begin(), tempkeys.end());
    }

    messages.clear();
    std::cout << std::endl;
    int r = 0;
    int percent = 0;
    std::vector<ScoredMessage> rankings;
    for (size_t i = 0; i < keys.size(); i++) {
        temp = subst::encode(ciphertext, keys[i], true);
        rankings.push_back(ScoredMessage(temp));
        if((100*i) / keys.size() != percent){
            percent = (100*i) / keys.size();
            std::cout << "\r" << percent << "\% complete";
        }
    }
    std::cout << "\n-------------------" << std::endl;
    std::sort(rankings.begin(), rankings.end());
    for (size_t i = rankings.size() - 1; i > rankings.size() - 26; i--) {
        std::cout << subst::invert(keys[i], true) << "   " << rankings[i].score << ":\n";
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
    while ( (c = getopt(argc, argv, "a:d:e:k:o:fht")) != -1 ) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
            case 'a':
            avalue_str = std::string(optarg);
            a_opt = true;
            break;
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
