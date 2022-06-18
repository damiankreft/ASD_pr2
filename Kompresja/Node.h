#pragma once

#include <string>
namespace Nodes {
    class node {
    public:
        std::string Label;
        int Count;
        node* Left;
        node* Right;
        node* Root;
        node();
        node(std::string label, int value, node* left = NULL, node* right = NULL);
    };
}
