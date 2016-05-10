#pragma once

#include <vector>
#include "types.h"

using namespace std;

class Node
{
public:
	Node(int numOfOutputs, int index);
	~Node(){};

	vector<Connection> outputWeights;
	int index;
	double gradient;
	double output;

	void next(const vector<Node> &lastLayer);
	void getOutputGrad(double target);
	void getHiddenGrad(const vector<Node> &nextLayer);
	void updateInputWeights(vector<Node> &lastLayer);
};