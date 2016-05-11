#pragma once

#include <vector>

using namespace std;

class Node
{
public:
    Node(int numOfOutputs, int index);
	~Node() {};
    
    vector<double> weights;
    vector<double> dWeights;
    int index;
    double gradient;
    double output;
    
    void push(const vector<Node> &lastLayer);
};