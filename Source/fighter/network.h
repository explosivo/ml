#pragma once

#include <vector>
#include <cmath>
#include "node.h"

using namespace std;

class Network
{
public:
    Network();
	~Network() {};
    
    vector<vector<Node>> layers;
    
    void predict(vector<double> &inputs);
    double getOutput();
};