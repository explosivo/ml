#pragma once

#include <vector>
#include "types.h"

using namespace std;

class Node
{
public:
    Node(int numOfOutputs, int index);
    
    vector<double> weights;
    vector<double> dWeights;
    int index;
    double gradient;
    double output;
    
    void push(const vector<Node> &lastLayer);
    void updateInputWeights(vector<Node> &prevLayer);
};