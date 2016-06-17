#include "combo.hpp"

bool unique(std::vector<int> v);

odometer::odometer(std::string values, int n){
  finished = false;
  numRotors = n;
  vals  = values;
  for (size_t i = 0; i < numRotors; i++) {
    indecies.push_back(0);
  }
}

//virtual odometer::~odometer(){}

//advance the [n]th rotor and cascade
void odometer::advance(int n){
  if (indecies[n] < vals.length() - 1) {
    indecies[n]++;
    return;
  }
  //if the next rotor exists
  else if(n < numRotors - 1){
    indecies[n] = 0;
    advance(n+1);
  }
  else{
    finished = true;
  }
  return;
}

std::string odometer::read_rotors(){
  std::string out(numRotors, '*');
  for (size_t i = 0; i < numRotors; i++) {
    out[i] = vals[indecies[i]];
  }
  return out;
}

std::vector<std::string> odometer::combos(){
  std::vector<std::string> out;
  while (!finished) {
    if(unique(indecies)){
      out.push_back(read_rotors());
    }
    advance(0);
  }
  return out;
}

//is every element unique?
bool unique(std::vector<int> in){
  for (size_t i = 0; i < in.size(); i++) {
    for (size_t j = i; j < in.size(); j++) {
      if((i!=j) && in[i] == in[j]){
        return false;
      }
    }
  }
  return true;
}

//How many ways to pick [num] values from [set]
std::vector<std::string> combos(int num, std::string set){
  odometer o(set, num);
  return o.combos();
}
