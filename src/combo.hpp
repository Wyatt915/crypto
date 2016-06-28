#include <string>
#include <vector>

class odometer {
private:
  bool finished;
  int numRotors;
  std::string vals;
  std::vector<int> indecies;
public:
  odometer (std::string values, int n);
  odometer();
  std::string read_rotors();
  std::vector<std::string> combos();
  void advance(int);
  //virtual ~odometer ();
};

std::vector<std::string> combos(int num, std::string set);
