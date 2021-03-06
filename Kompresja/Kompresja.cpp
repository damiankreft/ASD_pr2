#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

#include "Kompresja.h"
#include "Node.h"

/// <summary>
/// Definicja 'Byte' z wielkim 'B', bo namespace std zawiera 'byte'
/// </summary>
typedef unsigned char Byte;
Byte setBit(Byte x, unsigned int index) {
	return x | (1 << index);
}

using namespace std;
using namespace Nodes;

node::node() { }
node::node(std::string label, int value, node* left, node* right) : Left(left), Right(right)
{
	Label = label;
	Count = value;
}

string readInputFile()
{
	std::ifstream t = ifstream("input.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	t.close();
	return buffer.str();
}

void minHeapify(vector<node>& arr, int currentSize, int i)
{
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int largest = i;

	if (left < currentSize && arr[left].Count < arr[largest].Count)
	{
		largest = left;
	}

	if (right < currentSize && arr[right].Count < arr[largest].Count)
	{
		largest = right;
	}

	if (largest != i)
	{
		auto temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;

		minHeapify(arr, currentSize, largest);
	}
}

void buildMinHeap(vector<node>& arr)
{
	int arrayInitialSize = arr.size();

	for (int i = arrayInitialSize / 2 - 1; i >= 0; i--)
	{
		minHeapify(arr, arrayInitialSize, i);
	}
}

std::map<char, int> getWeights(string input)
{
	auto weights = map<char, int>();

	for (unsigned int i = 0; i < input.length(); i++)
	{
		if (weights.contains(input[i]))
		{
			weights[input[i]]++;
		}
		else
		{
			weights.insert(pair(input[i], 1));
		}
	}

	return weights;
}

void initializeCollection(vector<node>& arr, map<char, int> map)
{
	auto it = map.begin();

	while (it != map.end())
	{
		arr.push_back(node(string(1, it->first), it->second));
		it++;
	}
}

node extractMin(vector<node>& arr)
{
	auto minimum = arr.front();
	auto minimumIt = arr.begin();
	auto it = arr.begin();
	for (int i = 0; i < arr.size(); i++)
	{
		if (i != arr.size() - 1)
		{
			it++;
		}
		if (arr[i].Count < minimum.Count)
		{
			minimumIt = it;
			minimum = arr[i];
		}
	}

	iter_swap(minimumIt, --arr.end());
	auto extractedElement = arr.at(arr.size() - 1);
	node elemToReturn = extractedElement;

	arr.resize(arr.size() - 1);

	return elemToReturn;
}

map<char, string> createCharactersMap(vector<node>& arr)
{
	map<char, string> dict;
	auto mergedLabels = arr[0].Label;
	auto rootNode = arr[0];
	for (int i = 0; i < mergedLabels.length(); i++)
	{
		auto charToFind = mergedLabels[i];
		dict.insert(pair(mergedLabels[i], getPath(arr[0], charToFind)));
	}

	return dict;
}

void saveCharactersMapToFile(map<char, string> characterCodes)
{
	ofstream codesFile("huffmanCodesMap.txt");

	for (auto it = characterCodes.begin(); it != characterCodes.end(); it++)
	{
		codesFile << "\"" << it->first << "\": \"" << it->second << "\",\n";
	}

	codesFile.close();
}

string getPath(node nodeToSearchIn, char character)
{
	std::string pathToBuild;

	if (nodeToSearchIn.Left != NULL && nodeToSearchIn.Left->Label.find(character) != string::npos)
	{
		pathToBuild += '0';
		pathToBuild += getPath(*nodeToSearchIn.Left, character);
	}
	else if (nodeToSearchIn.Right != NULL && nodeToSearchIn.Right->Label.find(character) != string::npos)
	{
		pathToBuild += '1';
		pathToBuild += getPath(*nodeToSearchIn.Right, character);
	}

	return pathToBuild;
}

void writeBinaryFile(string compressedString)
{
	ofstream outputFile("output.bin", ios::out | ios::binary);

	if (!outputFile.is_open())
	{
		return;
	}

	// Workaround bo architektura nie pozwala zapisywac niczego mniejszego niz 8 bitow, dlatego uzywamy 8-bitowego bufora
	Byte bufferWorkaround = 0;
	int index = 0;
	for (int i = 0; i < compressedString.length(); i++)
	{
		if (compressedString[i] == '1')
		{
			bufferWorkaround = setBit(bufferWorkaround, index);
		}
		index++;
		if (index == 8)
		{
			outputFile.write(reinterpret_cast<char*>(&bufferWorkaround), sizeof(bufferWorkaround));
			bufferWorkaround = 0;
			index = 0;
		}
	}

	outputFile.close();
}

string CreateCompressedString(string input, map<char, string> codesMap)
{
	string compressedString = "";
	for (int i = 0; i < input.length(); i++)
	{
		compressedString += codesMap[input[i]];
	}

	return compressedString;
}

void compress(map<char, string> codesMap)
{
	string input = readInputFile();
	string compressedString = CreateCompressedString(input, codesMap);
	writeBinaryFile(compressedString);
}

int main()
{
	vector<node> testMinHeapify = vector<node>();
	initializeCollection(testMinHeapify, getWeights(readInputFile()));

	// Do testu
	//for (int i = 0; i < testMinHeapify.size(); i++)
	//{
	//    std::cout << "\"" << testMinHeapify[i].Label << "\": " << testMinHeapify[i].Count << ", ";
	//}
	// Koniec testu


	buildMinHeap(testMinHeapify);

	for (int i = 0; testMinHeapify.size() > 1; i++)
	{
		int count = 0;
		auto n = node();
		node n0 = extractMin(testMinHeapify);
		count += n0.Count;
		n.Left = new node(n0.Label, n0.Count, n0.Left, n0.Right);
		node n1 = extractMin(testMinHeapify);
		n.Right = new node(n1.Label, n1.Count, n1.Left, n1.Right);
		count += n1.Count;
		auto labelVal = n.Left->Label + n.Right->Label;

		n.Label = labelVal;
		n.Count = count;
		testMinHeapify.push_back(n);
		buildMinHeap(testMinHeapify);
	}

	// do testu
	/*std::cout << endl << "Heap after extractions " << std::endl;
	for (int i = 0; i < testMinHeapify.size(); i++)
	{
		std::cout << "\"" << testMinHeapify[i].Label << "\": " << testMinHeapify[i].Count << ", ";
	}*/

	auto charactersCodes = createCharactersMap(testMinHeapify);
	saveCharactersMapToFile(charactersCodes);
	compress(charactersCodes);
	//for (auto it = charactersCodes.begin(); it != charactersCodes.end(); it++)
	//{
	//    std::cout << "Key: \"" << it->first << "\", Value: " << it->second << endl;
	//}
	// Koniec testu
}


/*
Notatki:
// algorytm DFS do przeszukiwania drzewa
// https://stackoverflow.com/questions/34406972/how-to-traverse-a-huffman-tree-recursively-in-search-for-an-specific-element - finalnie nie skorzysta?em - a przynajmniej o tym nie wiem :winking:

// https://stackoverflow.com/questions/24181172/is-it-better-to-use-char-or-unsigned-char-array-for-storing-raw-data
*/