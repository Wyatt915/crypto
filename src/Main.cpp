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
    // std::cout << "Plaintext:" << std::endl << s << "\n\n";
    // std::string key = subst::null_key();
    // utils::shuffle(key);
    // std::cout << "Randomly generated key: " << key << "\n\n";
    // subst::encode(s, key);
    // std::cout << "Ciphertext:\n" << s << "\n\n";
    // autosolve(s);
    utils::remove_dpw(s);
    std::string v = vigenere::encode(s,"LEMONS");
    std::cout << v << std::endl;
    std::string key = vigenere::guess_key(v);
    std::cout << key << std::endl;
    std::cout << vigenere::decode(v, key);
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

void full_analysis(std::string subject){
    utils::sanitize(subject);
    utils::remove_dpw(subject);
    //write(fname + ".sanitized", subject + "\n");
    std::string results;
    std::cout << "CHI-SQUARED STATISTIC: " << analyze::chi_sq(subject) << std::endl;
    std::cout << "INDEX OF COINCIDENCE:  " << analyze::idx_coin(subject) << std::endl;
    std::cout << "CHARACTER COUNTS:\n";
    std::array<long int, 26> c = analyze::count_chars(subject);
    for(int i = 0; i < 26; i++){std::cout << (char)(i + 'A') << '\t' << c[i] << '\n'; }
    std::cout << "\nTOTAL: " << std::accumulate(begin(c), end(c),0) << "\n\n";

    std::cout << "CHARACTER FREQUENCY:\n";
    std::array<double, 26> f = analyze::frequency(subject);
    for(int i = 0; i < 26; i++){std::cout << (char)(i + 'A') << '\t' << f[i] << '\n'; }
    std::cout << "\nSUM: " << std::accumulate(begin(f), end(f), 0.0) << "\n\n";
    std::cout << "\n\nHISTOGRAM:\n\n" << std::endl;
    analyze::print_histogram(subject, 16);
}

int main(int argc, char* argv[]){
    int c;
    std::string a_opt_arg, d_opt_arg, e_opt_arg, k_opt_arg, i_opt_arg, o_opt_arg;
    bool a_opt = false;
    bool d_opt = false;
    bool e_opt = false;
    bool i_opt = false;
    bool k_opt = false;
    bool o_opt = false;

    static struct option long_options[] = {
        {"analyze",     optional_argument, 0, 'a'},
        {"decode",      optional_argument, 0, 'd'},
        {"encode",      optional_argument, 0, 'e'},
        {"help",        no_argument,       0, 'h'},
        {"input-file",  required_argument, 0, 'i'},
        {"key",         required_argument, 0, 'k'},
        {"output-file", required_argument, 0, 'o'},
        {"test",        no_argument,       0, 't'},
        {0, 0, 0, 0}
    };

    while ( (c = getopt_long(argc, argv, "a::c:d::e::i:k:o:ht", long_options, NULL)) != -1 ) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
            case 'a':
                if(optarg != NULL){
                    a_opt_arg = std::string(optarg);
                }
                a_opt = true;
                break;
            case 'c':
                crossword(std::string(optarg));
                return 0;
            case 'd':
                d_opt = true;
                if(optarg != NULL){
                    d_opt_arg = std::string(optarg);
                }
                break;
            case 'e':
                e_opt = true;
                if(optarg != NULL){
                    e_opt_arg = std::string(optarg);
                }
                break;
            case 'h':
                std::cout << help << std::endl;
                return 0;
            case 'i':
                i_opt = true;
                i_opt_arg = std::string(optarg);
                break;
            case 'k':
                k_opt = true;
                k_opt_arg = std::string(optarg);
                break;
            case 'o':
                o_opt = true;
                o_opt_arg = std::string(optarg);
                break;
            case 't':
                load_word_list();
                test();
                return 0;
            case ':':   /* missing option argument */
                fprintf(stderr, "%s: option `-%c' requires an argument\n",
                argv[0], optopt);
                break;
            case '?':    /* invalid option */
            default:
                fprintf(stderr, "%s: option `-%c' is invalid: ignored\n",
                argv[0], optopt);
                break;
        }
    }

    if(a_opt){
        if(i_opt && a_opt_arg.length() > 0){
            std::cerr << "'--input-file' and '--analyze' cannot both have arguments.\n";
            return 1;
        }
        if(i_opt){ a_opt_arg = read(i_opt_arg); }
        else if(a_opt_arg == ""){
            std::cerr << "'analyze' must have an argument if no input file is specified.\n";
            return 1;
        }
        full_analysis(a_opt_arg);
        return 0;
    }
    if(e_opt){
        if(i_opt && e_opt_arg.length() > 0){
            std::cerr << "'--input-file' and '--encode' cannot both have arguments.\n";
            return 1;
        }
        if(i_opt){ e_opt_arg = read(i_opt_arg); }
        else if(e_opt_arg == ""){
            std::cerr << "'--encode' must have an argument if no input file is specified.\n";
            return 1;
        }
        if(!k_opt){
            k_opt_arg = subst::null_key();
            utils::shuffle(k_opt_arg);
            std::cout << "Using randomly generated key: " << k_opt_arg << std::endl;
        }
        utils::sanitize(e_opt_arg);
        subst::encode(e_opt_arg, k_opt_arg);
        if(!o_opt){ std::cout << e_opt_arg << std::endl; }
        else{ write(o_opt_arg, e_opt_arg); }
    }
    if(d_opt){
        if(i_opt && d_opt_arg.length() > 0){
            std::cerr << "'--input-file' and '--decode' cannot both have arguments.\n";
            return 1;
        }
        if(i_opt){ d_opt_arg = read(i_opt_arg); }
        else if(d_opt_arg == ""){
            std::cerr << "'--decode' must have an argument if no input file is specified.\n";
            return 1;
        }
        utils::sanitize(d_opt_arg);
        if(k_opt){
            subst::decode(d_opt_arg, k_opt_arg);
            std::cout << d_opt_arg << std::endl;
        } else {
            load_word_list();
            autosolve(d_opt_arg);
        }
    }
    return 0;
}
