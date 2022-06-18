#include <string>
#include <vector>
#include <map>
#include "Node.h"
using namespace std;
using namespace Nodes;

std::string getPath(node nodeToSearchIn, const char character);
std::string readInputFile();
map<char, string> createCharactersMap(vector<node>& arr);
void buildMinHeap(vector<node>& arr);
void initializeCollection(vector<node>& arr, map<char, int> map);
