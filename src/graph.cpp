#include "analyze.hpp"
#include "graph.hpp"
#include "process.hpp"

#include <iostream>

Graph::Graph(const std::vector<std::vector<std::string> >& data){
	verts = data;
	key = std::string(26, '*');
	numcalls = 0;
}

std::list<std::string> Graph::get_key_list(){
	dft(0, 0);
	keylist.sort();
	keylist.unique();
	/*if (!key_conflict(keylist)){
		merge_keys(keylist);
	}*/
	return keylist;
}

std::vector<std::string> Graph::get_key_vec(){
	std::list<std::string> l = get_key_list();
	std::vector<std::string> v{ make_move_iterator(begin(l)), make_move_iterator(end(l)) };
	return v;
}

/**
*  Depth-first traversal
*/
void Graph::dft(unsigned int col, unsigned int row){
	numcalls++;
	//std::cout << numcalls << std::endl;
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
		if (!analyze::key_conflict(verts[col][row], keysFromPath.back())){
			path.push_back(row);
			key = process::merge_keys(keysFromPath.back(), verts[col][row]);
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
		//if the current vert does conflict
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
