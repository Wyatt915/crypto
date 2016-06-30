#include "analyze.hpp"
#include "crypto_utils.hpp"
#include "graph.hpp"
#include "process.hpp"
#include "substitution.hpp"
#include "wordlist.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>

namespace subst{
    std::string null_key(){
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }

    std::string empty_key(){
        return std::string(26, '*');
    }

    void invert(std::string &key){
        std::string inv = empty_key();
        for (int i = 0; i < 26; i++){
            if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
        }
        key = inv;
    }

    std::string invert(std::string key, bool a){
        std::string inv = empty_key();
        for (int i = 0; i < 26; i++){
            if (key[i] != '*'){ inv[key[i] - 'A'] = i + 'A'; }
        }
        return inv;
    }


    void encode(std::string &text, const std::string &key){
        utils::capitalize(text);
        std::string ciphertext;

        for (size_t i = 0; i < text.length(); i++){
            if (!(text[i] >= 'A' && text[i] <= 'Z')){
                ciphertext += text[i]; //ignore non-alpha chars
            }
            else{ text[i] = key[text[i] - 'A']; }
        }
    }

    void decode(std::string &text, std::string key){
        invert(key);
        encode(text, key);
    }

    std::string encode(std::string text, const std::string key, bool){
        utils::capitalize(text);
        std::string ciphertext;

        for (size_t i = 0; i < text.length(); i++){
            if (!(text[i] >= 'A' && text[i] <= 'Z')){
                ciphertext += text[i]; //ignore non-alpha chars
            }
            else{ text[i] = key[text[i] - 'A']; }
        }
        return text;
    }

    std::string decode(std::string text, std::string key, bool){
        invert(key);
        std::string s = encode(text, key, true);
        return s;
    }

    //returns a list of all words of the same pattern
    utils::VString match_by_pattern(std::string in){
        utils::VString out;
        std::string pattern = analyze::char_pattern(in);
        for (int i = 0; i < LENGTH_OF_LIST; i++)
        {
            if (pattern == PATTLIST[i]){ out.push_back(WORDLIST[i]); }
        }
        return out;
    }

    //If a word has a lot of letters in it, it has more entropy.  However,
    //if there are many words that match its pattern or signature, than we
    //actually have less usable information than a word with only a few matches.
    float entropy(const utils::VString& column){
        float length = column[0].length();
        float size = column.size();
        return length/(size);
    }

    //sorts by length of word, from greatest to least
    bool sort_by_length(const std::string& lhs, const std::string& rhs){
        return lhs.length() > rhs.length();
    }

    //sorts a vector of vectors according to the size of each vector, greatest to least.
    bool sort_by_size(const utils::VString& lhs, const utils::VString& rhs){
        return lhs.size() > rhs.size();
    }

    //greatest to least
    bool sort_by_entropy(const utils::VString& lhs, const utils::VString& rhs){
        return entropy(lhs) > entropy(rhs);
    }

    //if there are more than [n] words, only keep the [n] longest.
    void largest_n(utils::VString& msg, int n){
        std::sort(msg.begin(), msg.end(), sort_by_length);
        if (msg.size() > n){
            utils::VString::iterator iter = msg.begin();
            std::advance(iter, n);
            msg.erase(iter, msg.end());
        }
    }

    //minimize the search space of the graph
    void optimize(utils::VVString& in){
        auto iter = in.begin();
        utils::VString current;

        while (iter != in.end()){
            current = *iter;
            //if there are no direct pattern matches, or there are too many to be practical, remove those columns.
            if (current.size() < 2){
                iter = in.erase(iter);
            }
            else{ iter++; }
        }

        iter = in.begin();
        std::sort(in.begin(), in.end(), sort_by_entropy);
        auto last = std::unique(in.begin(), in.end());
        in.erase(last, in.end());

        iter = in.begin();
        int max = 4096; //maximum search space (2^15)
        float searchSpace = 1;
        while (searchSpace < max && iter != in.end()){
            current = *iter;
            searchSpace += current.size();
            iter++;
        }
        in.erase(iter, in.end());
    }


    utils::VVString prepare_keys(utils::VVString matchedPlain, utils::VString cipherParsed){
        //make a list of possible keys for each word
        std::vector<std::vector<std::string> >keys;
        std::vector<std::string> templist;
        for (size_t i = 0; i < matchedPlain.size(); i++)
        {
            templist.clear();
            for (size_t j = 0; j < matchedPlain[i].size(); j++)
            {
                templist.push_back(
                    process::generate_key(
                        process::make_charPair_vec(
                            matchedPlain[i][j], cipherParsed[i]
                        )
                    )
                );
            }
            //This ensures that any list of the same keys is identical at every index.
            std::sort(templist.begin(), templist.end());
            keys.push_back(templist);
        }
        return keys;
    }

    //pass the full encrypted message to this function.
    //generates a list of possible (incomplete) keys
    utils::VString solve_by_pattern(std::string cipher){
        utils::VString cipherParsed = utils::parse(cipher);

        largest_n(cipherParsed, 16);

        //for each word, make a list of all words of the same pattern.
        utils::VVString matched;
        for (size_t i = 0; i < cipherParsed.size(); i++)
        {
            matched.push_back(match_by_pattern(cipherParsed[i]));
        }

        utils::VVString graphData = prepare_keys(matched, cipherParsed);

        optimize(graphData);

        utils::VString results;
        if(graphData.size() > 1){
            Graph g(graphData);
            results = g.get_key_vec();
        }
        else{ results = graphData[0]; }
        return results;
    }

    //This function takes the encrypted message and the keys generated from
    //solve_by_pattern() as parameters, and attempts to further refine the
    //result by looking at partially solved words and comparing them to
    //WORDLIST, and then going through the same depth-first search as
    //solve_by_pattern() uses in graph.cpp
    utils::VString fine_solve(std::string cipher, utils::VString courseKeys){
        int count = courseKeys.size();
        utils::VString fineKeys;
        for (size_t n = 0; n < count; n++) {
            std::string msg = decode(cipher, courseKeys[n], true);

            utils::VString cipherParsed = utils::parse(cipher);
            utils::VString plainParsed = utils::parse(msg);

            auto iterPlain = plainParsed.begin();
            auto iterCipher = cipherParsed.begin();

            while(iterPlain != plainParsed.end()){
                //we only want to look at partially solved words.
                if(analyze::is_complete(*iterPlain)){
                    iterPlain = plainParsed.erase(iterPlain);
                    iterCipher = cipherParsed.erase(iterCipher);
                }
                else{ iterCipher++; iterPlain++; }
            }

            utils::VVString matched;
            for (size_t i = 0; i < plainParsed.size(); i++) {
                matched.push_back(process::match_partial_words(plainParsed[i]));
            }

            utils::VVString graphData = prepare_keys(matched, cipherParsed);

            optimize(graphData);
            utils::VString results;
            if(graphData.size() > 1){
                Graph g(graphData);
                //g.debug = true;
                results = g.get_key_vec();
            }
            else{ results = graphData[0]; }

            for(int i = 0; i < results.size(); i++){
                if(!analyze::key_conflict(results[i], courseKeys[n])){
                    fineKeys.push_back(process::merge_keys(results[i], courseKeys[n]));
                }
            }
        }
        return fineKeys;
    }
}
