#pragma once

#include <cmath>
using namespace std;

struct Input
{
	int playerState, playerHealth, agentHealth, time;
	double distance;
};

struct Connection
{
	float weight, dWeight;
};

static double activationFunction(double x)
{
	return tanh(x);
}
static double activationFunctionDerivative(double x)
{
	return 1.0 - x * x;
}
static double getRandom()
{
	return rand() / double(RAND_MAX);
}

const double eta = 0.15;
const double alpha = 0.5;
