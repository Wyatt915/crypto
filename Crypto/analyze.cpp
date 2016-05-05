#include "analyze.h"
#include <algorithm>
#include <math.h>
#include <iostream>


/**
*  Counts the number of instances of each character from A to Z.
*  @param in must be UPPERCASE
*/
array<int, 26> countchars(string in){
	array<int, 26> out;

	for (unsigned int i = 0; i < 26; i++){ out[i] = 0; }

	for (unsigned int i = 0; i < in.length(); i++){
		out[in[i] - 'A']++;
	}
	return out;
}

/**
*  Finds the frequency of occurance of each letter in a given string.
*/
array<float, 26> frequency(string in){
	array<int, 26> count = countchars(in);
	int sum = accumulate(count.begin(), count.end(), 0);
	array<float, 26> freq;
	for (int i = 0; i < 26; i++){
		freq[i] = (float)count[i] / (float)sum;
	}
	return freq;
}

/**
*  Prints a histogram of letter frequency.
*  @param in String to be analyzed.
*  @param resolution the height of the graph, in lines.
*/
void print_histogram(string in, int resolution){
	array<float, 26> freq = frequency(in);
	float max_val = *max_element(freq.begin(), freq.end());

	for (int i = 0, j = resolution; i < resolution; i++, j--){
		cout << ' ';
		for (int k = 0; k < 26; k++){
			if ((freq[k] * (resolution / max_val)) >= j){
				cout << "# ";
			}
			else{
				cout << "  ";
			}
		}
		//create labels for the vertical axis.
		cout << "| ";
		printf("%.3f", 100 * (max_val - (((float)i / resolution)*max_val)));
		cout << "%\n";
	}

	cout << ' ';
	for (int i = 0; i < 26; i++){
		cout << (char)('A' + i) << ' ';
	}
	cout << '\n';
}


/**
*  Produces the character pattern of a given word.
*  For example, the input "ENCRYPTS" would give the output "********" because
*  each character in the word is unique, i.e. none of the letters repeat. In contrast,
*  The word "QUANTITATIVE" results in a pattern of "**A*BCBABC**" because the letter A
*  is the first letter that gets repeated, and it occurs in the 3rd and 8th positions.
*  Next, N is repeated, and since it is the second unique letter that is repeated, it is
*  called B in the pattern output. The second repeat letter occurs at positions 5, 7, and 9.
*
*  The reason for using the letters A, B, C...Z (in order) to represent other letters
*  is because if a word goes through a substitution cipher, its resultant pattern will
*  not change. For instance the pattern of "AGRICULTURALISTS" is "A*BC*DEFDBAECGFG". If
*  If AGRICULTURALISTS is passed through a substitution cipher, it will come out as something
*  like QUKOEXSZXKQSOLZL, but the pattern will remain as "A*BC*DEFDBAECGFG".
*/
string char_pattern(string in){
	capitalize(in);
	const int l = in.length();
	char *pattern = new char[l];
	bool *completed = new bool[l];
	char marker = 'A';
	bool match_found = false;

	//initialize the pattern
	for (int i = 0; i < l; i++){ pattern[i] = '*'; completed[i] = false; }

	for (int i = 0; i < l; i++){
		//look at the ith index of the input and find all other occurances of it.
		//when all occurences have been found, set completed[] to true at each position of the occurance.
		//mark each index in pattern[] with marker, increment marker, increment i, and loop
		if (completed[i]){ continue; }
		for (int j = i + 1; j < l; j++){
			if ((in[i] == in[j]) && !completed[j]){
				pattern[i] = marker;
				pattern[j] = marker;
				completed[j] = true;
				match_found = true;
			}
		}
		if (match_found){ marker++; }
		match_found = false;
	}

	string out(pattern, l);
	delete[] pattern;
	delete[] completed;

	return out;
}

string generate_key(vector<ChaPair> map){
	string out = string(26, '*');
	for (size_t i = 0; i < map.size(); i++)
	{
		out[map[i].x - 'A'] = map[i].mapsTo;
	}
	return out;
}

//x and mapsto will always be the same length
vector<ChaPair> make_chapair_vec(string x, string mapsto){
	vector<ChaPair> out;
	ChaPair temp;
	for (size_t i = 0; i < x.length(); i++)
	{
		temp.x = x[i];
		temp.mapsTo = mapsto[i];
		out.push_back(temp);
	}
	return out;
}

//returns a single key from a vector of incomplete keys
//which have been determined not to conflict.
string merge_keys(vector<string> keys){
	string out = string(26, '*');	//26 '*'s
	for (size_t i = 0; i < keys.size(); i++)
	{
		for (size_t j = 0; j < keys[i].length(); j++)
		{
			if (keys[i][j] != '*'){
				out[j] = keys[i][j];
			}
		}
	}
	return out;
}

string merge_keys(list<string> l){
	vector<string> keys{ std::make_move_iterator(std::begin(l)), std::make_move_iterator(std::end(l)) };
	string out = string(26, '*');	//26 '*'s
	for (size_t i = 0; i < keys.size(); i++)
	{
		for (size_t j = 0; j < keys[i].length(); j++)
		{
			if (keys[i][j] != '*'){
				out[j] = keys[i][j];
			}
		}
	}
	return out;
}

string merge_keys(string key1, string key2){
	string out = string(26, '*');	//26 '*'s
	for (int i = 0; i < 26; i++){
		if (key1[i] != '*'){
			out[i] = key1[i];
		}
	}
	for (int i = 0; i < 26; i++){
		if (key2[i] != '*'){
			out[i] = key2[i];
		}
	}
	return out;
}

//finds if 2 full or partial keys will not produce the same result
bool key_conflict(string key1, string key2){
	for (size_t i = 0; i < key1.length(); i++)
	{
		if (key1[i] != key2[i] && (key1[i] != '*' && key2[i] != '*')){
			//if the ith char of key1 is not the same as the ith char of key2
			//and the ith char of either is not a '*', then the keys are in conflict.
			return true;
		}

	}
	string t1, t2;
	t1 = key1;
	t2 = key2;
	t1 = merge_keys(t1, t2);
	remove_dpw(t1);
	bool accounted[26] = { 0 };
	for (int i = 0; i < t1.length(); i++){
		//if the char at i has already been accounted for then
		//it is a duplicate and therefore the keys conflict.
		if (accounted[t1[i] - 'A']){ return true; }
		else { accounted[t1[i] - 'A'] = true; }
	}
	return false;
}

bool key_conflict(list<string> keys){
	string key1, key2;
	key1 = *keys.begin();
	list<string>::iterator iter = keys.begin();
	++iter;
	while (iter != keys.end()){
		key2 = *iter;
		for (size_t i = 0; i < key1.length(); i++)
		{
			if (key1[i] != key2[i] && (key1[i] != '*' && key2[i] != '*')){
				//if the ith char of key1 is not the same as the ith char of key2
				//and the ith char of either is not a '*', then the keys are in conflict.
				return true;
			}

		}
		string t1, t2;
		t1 = key1;
		t2 = key2;
		t1 = merge_keys(t1, t2);
		remove_dpw(t1);
		bool accounted[26] = { 0 };
		for (int i = 0; i < t1.length(); i++){
			//if the char at i has already been accounted for then
			//it is a duplicate and therefore the keys conflict.
			if (accounted[t1[i] - 'A']){ return true; }
			else { accounted[t1[i] - 'A'] = true; }
		}
		key1 = merge_keys(key1, key2);
	}
	return false;
}

//----------------[Graph Junk]---------------

Graph::Graph(const vector<vector<string> >& data){
	verts = data;
	key = string(26, '*');
}

list<string> Graph::getKeyList(){
	dft(0, 0);
	keylist.unique();
	if (!key_conflict(keylist)){
		merge_keys(keylist);
	}
	return keylist;
}

vector<string> Graph::getKeyVec(){
	list<string> l = getKeyList();
	vector<string> v{ std::make_move_iterator(std::begin(l)), std::make_move_iterator(std::end(l)) };
	return v;
}

/**
*  Depth-first traversal
*/
void Graph::dft(unsigned int col, unsigned int row){
	//If there are no previous columns and we have gone past the end of the current column
	if (col == 0 && row >= verts[col].size()){
		return;
	}
	//If we have gone past the end of the current column
	if (row >= verts[col].size()){
		int backOfPath = path.back();
		path.pop_back();	//remove so it can be replaced.
		keysFromPath.pop_back();
		dft(col - 1, backOfPath + 1);	//move to the previous column and down one row
		return;
	}
	if (col == 0){
		path.clear();
		keysFromPath.clear();
		path.push_back(row);
		keysFromPath.push_back(verts[col][row]);
		dft(col + 1, 0);
		return;
	}
	else {
		//if the current vert does not conflict with those before it
		if (!key_conflict(verts[col][row], keysFromPath.back())){
			path.push_back(row);
			key = merge_keys(keysFromPath.back(), verts[col][row]);
			keysFromPath.push_back(key);
			if (col < verts.size() - 1){
				dft(col + 1, 0);			//if the next column exists, go to it.
			}
			else {							//if the next column does not exist
				paths.push_back(path);	//we have a completed path. add it to the list.
				keylist.push_back(keysFromPath.back());
				path.pop_back();			//remove the last index from the path
				keysFromPath.pop_back();
				//If we are not in the last row of the current column
				if (row < verts[col].size()) {
					dft(col, row + 1);	//move down the column
				}
				//if we are in the last row of the current column
				else{
					dft(col - 1, path.back() + 1);	//move to the previous column and down one row
				}
			}
		}
		//if the current vert does not qualify to our rule
		else{
			//If we are not in the last row of the current column
			if (row < verts[col].size()) {
				dft(col, row + 1);	//move down the column
			}
			//if we are in the last row of the current column
			else if (col > 0){
				int backOfPath = path.back();
				path.pop_back();				//remove so it can be replaced.
				keysFromPath.pop_back();
				dft(col - 1, backOfPath + 1);//move to the previous column and down one row
			}
		}
	}
}