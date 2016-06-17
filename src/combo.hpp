#include <string>
#include <vector>

class odometer {
private:
  int numRotors;
  std::string vals;
  std::vector<int> indecies;
  bool finished;
public:
  odometer();
  odometer (std::string values, int n);
  void advance(int);
  std::string read_rotors();
  std::vector<std::string> combos();
  //virtual ~odometer ();
};

std::vector<std::string> combos(int num, std::string set);
