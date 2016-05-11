#pragma once

#include <vector>
#include <cmath>
#include "types.h"
#include "node.h"

using namespace std;

class Network
{
public:
    Network();
    
    vector<vector<Node>> layers;
    
    void predict(const vector<double> &inputs);
    void backPropagate(double target);
    double getOutput();
};