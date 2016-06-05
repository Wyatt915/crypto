//#include "substitution.h"
//#include "analyze.h"
//
//
////typedef std::vector<std::vector<std::string>>	str_2D;
////typedef std::vector<std::string>				strvec;
//
//std::string null_key(){
//	return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//}
//
//std::string empty_key(){
//	return std::string('*', 26);
//}
//
//bool sort_by_size(const strvec &lhs, const strvec &rhs){ return lhs.size() < rhs.size(); }
//
////minimize the search space of the graph
//void optimize(str_2D& in){
//	size_t pos = 0;
//	while (pos < in.size()){
//		//if there are no direct pattern matches, or there are too many to be practical, remove those columns.
//		if (in[pos].size() == 0 || in[pos].size() > 300){
//			in.erase(in.begin() + pos);
//			pos = 0;
//		}
//		else{ pos++; }
//	}
//	//move vectors with a large number of entries to the end.
//	//std::sort(in.begin(), in.end(), sort_by_size);
//}
//
////if there are more than 16 words, only keep the 16 longest.
//void keep_largest(strvec& in, int howmany){
//	if (in.size() > howmany){
//		strvec temp;
//		unsigned int maxLen = 0;
//		int maxLenIdx = 0; //where is the longest word
//		for (size_t i = 0; i < howmany; i++){
//			for (size_t j = 0; j < in.size(); j++){
//				if (in[j].length() > maxLen){
//					maxLen = in[j].length();
//					maxLenIdx = j;
//				}
//			}
//			temp.push_back(in[maxLenIdx]);
//			in.erase(in.begin() + maxLenIdx);
//			maxLen = 0;
//			maxLenIdx = 0;
//		}
//		in = temp;
//	}
//}
//
//
//strvec tokenize(std::string message){
//	strvec tokens;
//	std::string temp;
//	std::stringstream sst(message);
//	while (sst >> temp){
//		//each word is its own array element.
//		tokens.push_back(temp);
//	}
//	return tokens;
//}
//
////pass the full encrypted message to this function.
////generates a list of possible (incomplete) keys
//strvec solve_by_pattern(std::string message){
//	strvec messageParsed;
//	std::string temp;
//	std::stringstream sst(message);
//	while (sst >> temp){
//		//each word is its own array element.
//		messageParsed.push_back(temp);
//	}
//
//	keep_largest(messageParsed, 16);
//
//	print_list(messageParsed, ' ');
//
//
//	unsigned long numkeys = 0;//just for debugging.
//	//for each word, make a list of all words of the same pattern.
//	str_2D matched;
//	for (size_t i = 0; i < messageParsed.size(); i++)
//	{
//		matched.push_back(match_by_pattern(messageParsed[i]));
//	}
//
//	//make a std::list of possible keys for each word
//	std::vector<std::list<std::string> >keys;
//	std::list<std::string> templist;
//	for (size_t i = 0; i < matched.size(); i++)
//	{
//		templist.clear();
//		for (size_t j = 0; j < matched[i].size(); j++)
//		{
//			numkeys++;
//			templist.push_back(generate_key(make_chapair_vec(matched[i][j], messageParsed[i])));
//		}
//		templist.sort();
//		templist.unique();
//		keys.push_back(templist);
//	}
//
//	str_2D ragged;
//	ragged.resize(keys.size());
//	for (size_t i = 0; i < ragged.size(); i++)
//	{
//		ragged[i].assign(std::begin(keys[i]), std::end(keys[i]));
//	}
//
//	optimize(ragged);
//
//	Graph g(ragged);
//
//	strvec out = g.get_key_vec();
//
//	return out;
//}
