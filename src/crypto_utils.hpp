#pragma once
#include <string>
#include <vector>


namespace utils{
  typedef std::vector<std::string> VString;
  typedef std::vector<std::vector<std::string> > VVString;
  inline double square(double value){ return value * value; }
  int binary_search(std::string what);
  std::string char_vector_to_string(std::vector<char>);
  std::vector<std::string> parse(std::string message);
  std::vector<std::string> permute(std::string);
  void capitalize(std::string& in);
  void print_color(std::string msg);
  void print_list(const std::vector<std::string>& in, char delim);
  void print_list(const std::vector<std::string>& in, std::string delim);
  void print_list(const std::vector<std::string>& in);
  void remove_dp(std::string& in);
  void remove_dpw(std::string& in);//remove digits, punctuation, and spaces
  void remove_p(std::string& in);
  void sanitize(std::string& in);
  void shuffle(std::string& in);
}

struct CharPair{
  char x, mapsTo;
};

struct crypto_error{
  crypto_error(std::string);
  std::string what;
};
