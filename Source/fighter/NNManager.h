#pragma once

#include "network.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class NNManager
{
public:
	NNManager();
	~NNManager();


	vector<double> minSet;
	vector<double> maxSet;
	Network network;

	double Normalize(float x, int i);
	int Predict(float PlayerState, float Distance, float PlayerHealth, float AgentHealth, float RemainingTime);
	void LoadNetwork();
};