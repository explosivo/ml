#pragma once

#include <vector>
#include <cmath>
#include "node.h"
#include "types.h"

using namespace std;

class Network
{
public:
	Network(const vector<unsigned> &topology);
	~Network() {};

	vector<vector<Node>> layers;
	double error;
	double avgError;
	double totalTrainingSamples;

	void forwardPropagate(const vector<double> &inputs);
	void backPropagate(const vector<double> &targets);
	vector<double> getResults();
};