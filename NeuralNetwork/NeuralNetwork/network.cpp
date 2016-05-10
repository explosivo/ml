#include "network.h"

Network::Network(int totalTrainingSamples)
{
	this->totalTrainingSamples = (double)totalTrainingSamples;

	//layers
	for (int i = 0; i < 3; i++)
	{
		layers.push_back(vector<Node>());
	}
	//inputs
	for (int i = 0; i < 6; i++)
	{
		layers[0].push_back(Node(7, i));
	}
	layers[0].back().output = 1;
	//hidden
	for (int i = 0; i < 7; i++)
	{
		layers[1].push_back(Node(7, i));
	}
	layers[1].back().output = 1;
	//outputs
	for (int i = 0; i < 6; i++)
	{
		layers[2].push_back(Node(0, i));
	}
	layers[2].back().output = 1;
}

void Network::forwardPropagate(const vector<double> &inputs)
{
	for (int i = 0; i < inputs.size(); i++)
	{
		layers[0][i].output = inputs[i];
	}
	for (int i = 1; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			layers[i][j].next(layers[i - 1]);
		}
	}
}

void Network::backPropagate(const vector<double> &targets)
{
	error = 0;
	for (int i = 0; i < layers[2].size() - 1; i++)
	{
		double d = targets[i] - layers[2][i].output;
		error += d * d;
	}
	error = sqrt(error / layers[2].size() - 1);
	avgError = (avgError * 100.0 + error) / (100.0 + 1.0);

	for (int i = 0; i < layers[2].size() - 1; i++)
	{
		layers[2][i].getOutputGrad(targets[i]);
	}

	for (int i = 0; i < layers[1].size() - 1; i++)
	{
		layers[1][i].getHiddenGrad(layers[2]);
	}

	for (int i = 2; i > 0; i--)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			layers[i][j].updateInputWeights(layers[i - 1]);
		}
	}
}

vector<double> Network::getResults()
{
	vector<double> results;
	vector<Node> outputLayer = layers.back();
	for (int i = 0; i < outputLayer.size(); i++)
	{
		results.push_back(outputLayer[i].output);
	}
	return results;
}