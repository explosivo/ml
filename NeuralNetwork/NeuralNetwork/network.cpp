#include "network.h"

Network::Network()
{
	//layers
	for (int i = 0; i < 3; i++)
	{
		layers.push_back(vector<Node>());
	}
	//inputs
	for (int i = 0; i < 6; i++)
	{
		layers[0].push_back(Node(100, i));
	}
	layers[0].back().output = 1.0;
	//hidden
	for (int i = 0; i < 100; i++)
	{
		layers[1].push_back(Node(2, i));
	}
	layers[1].back().output = 1.0;
	//outputs
	for (int i = 0; i < 2; i++)
	{
		layers[2].push_back(Node(0, i));
	}
	layers[2].back().output = 1.0;
}

void Network::predict(const vector<double> &inputs)
{
	for (int i = 0; i < inputs.size(); i++)
	{
		layers[0][i].output = inputs[i];
	}
	for (int i = 0; i < layers[1].size() - 1; i++)
	{
		layers[1][i].push(layers[0]);
	}
	layers[2][0].push(layers[1]);
}

void Network::backPropagate(double target)
{
	// Calculate output layer gradients

	for (int i = 0; i < layers[2].size() - 1; i++)
	{
		double d = target - layers[2][i].output;
		layers[2][i].gradient = d * activationDx(layers[2][i].output);
	}

	// Calculate hidden layer gradients

	for (int i = 0; i < layers[1].size(); i++)
	{
		double sum = 0.0;
		for (int j = 0; j < layers[2].size() - 1; j++) {
			sum += layers[1][i].weights[j] * layers[2][j].gradient;
		}
		layers[1][i].gradient = sum * activationDx(layers[1][i].output);
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights

	for (int n = 0; n < layers[2].size() - 1; ++n)
	{
		layers[2][n].updateInputWeights(layers[1]);
	}
	for (int n = 0; n < layers[1].size() - 1; ++n)
	{
		layers[1][n].updateInputWeights(layers[0]);
	}
}

double Network::getOutput()
{
	return layers.back()[0].output;
}
