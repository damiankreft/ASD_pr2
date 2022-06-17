#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

class node {
public:
    string Label;
    int Count;

    node()
    {

    }

    node(string label, int value)
    {
        Label = label;
        Count = value;
    }

    node(string label, int value, node*n0, node* n1)
    {
        Label = label;
        Count = value;
        Left = n0;
        Right = n1;
    }

    node *Left;
    node *Right;
};

string readInputFile()
{
    std::ifstream t = ifstream("input.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
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
    arr.resize(arr.size() - 1);
    return extractedElement;
}

int main() 
{
    vector<node> testMinHeapify = vector<node>();
    testMinHeapify.push_back(node("B", 1));
    testMinHeapify.push_back(node("a", 7));
    testMinHeapify.push_back(node("r", 5));
    testMinHeapify.push_back(node("b", 3));
    testMinHeapify.push_back(node(" ", 2));
    testMinHeapify.push_back(node("m", 1));
    //initializeCollection(testMinHeapify, getWeights(readInputFile()));

    // Do testu
    for (int i = 0; i < testMinHeapify.size(); i++)
    {
        std::cout << "\"" << testMinHeapify[i].Label << "\": " << testMinHeapify[i].Count << ", ";
    }
    // Koniec testu


    buildMinHeap(testMinHeapify);

    for (int i = 0; testMinHeapify.size() > 1; i++)
    {
        auto n0 = extractMin(testMinHeapify);
        auto n1 = extractMin(testMinHeapify);
        auto mergedNode = node(n0.Label.append(n1.Label), n0.Count + n1.Count, &n0, &n1);

        testMinHeapify.push_back(mergedNode);
        buildMinHeap(testMinHeapify);
    }

    // do testu
    std::cout << endl << "Heap after extractions " << std::endl;
    for (int i = 0; i < testMinHeapify.size(); i++)
    {
        std::cout << "\"" << testMinHeapify[i].Label << "\": " << testMinHeapify[i].Count << ", ";
    }
    // Koniec testu
}