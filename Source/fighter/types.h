#pragma once
#include "fighter.h"
#include <cmath>
#include <cstdlib>
using namespace std;

static double activation(double x)
{
    return 1.0/(1.0 + exp(-x));//tanh(x);
}
static double activationDx(double x)
{
    return activation(x) * (1 - activation(x));//-exp(-x);//1.0 - x * x;
}
static double getRandom()
{
	return rand() / double(RAND_MAX);
}

static double eta = 0.15;
static double learningrate = 0.00001; //was .5 before