#include <string>
#include <vector>
#include <map>
#include "Node.h"
using namespace std;
using namespace Nodes;

std::string getPath(node nodeToSearchIn, const char character);
void writeBinaryFile(string compressedString);
string CreateCompressedString(string input, map<char, string> codesMap);
void compress(map<char, string> codesMap);
std::string readInputFile();
map<char, string> createCharactersMap(vector<node>& arr);
void saveCharactersMapToFile(map<char, string> characterCodes);
void buildMinHeap(vector<node>& arr);
void initializeCollection(vector<node>& arr, map<char, int> map);
