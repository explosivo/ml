#include "network.h"
#include <iostream>
Network::Network(const vector<unsigned> &topology)
{/*
	this->totalTrainingSamples = (double)totalTrainingSamples;

	//layers
	for (int i = 0; i < 3; i++)
	{
		layers.push_back(vector<Node>());
	}
	//inputs
	for (int i = 0; i < 6; i++)
	{
		layers[0].push_back(Node(8, i));
	}
	layers[0].back().output = 1;
	//hidden
	for (int i = 0; i < 8; i++)
	{
		layers[1].push_back(Node(2, i));
	}
	layers[1].back().output = 1;
	//outputs
	for (int i = 0; i < 2; i++)
	{
		layers[2].push_back(Node(0, i));
	}
	layers[2].back().output = 1;
	*/
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		layers.push_back(vector<Node>());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have a new layer, now fill it with neurons, and
		// add a bias neuron in each layer.
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			layers.back().push_back(Node(numOutputs, neuronNum));
		}

		// Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
		layers.back().back().output = 1.0;
	}
	cout << "layer = " << layers.size() << " " << layers[0].size() << " " << layers[1].size() << " " << layers[2].size();
}

void Network::forwardPropagate(const vector<double> &inputs)
{
	for (int i = 0; i < inputs.size(); i++)
	{
		layers[0][i].output = inputs[i];
	}
	for (int i = 1; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size() - 1; j++)
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

	for (int i = layers.size() - 2; i > 0; i--)
	{
		vector<Node> &hiddenLayer = layers[i];
		vector<Node> &nextLayer = layers[i + 1];
		for (int j = 0; j < hiddenLayer.size(); j++)
		{
			layers[1][i].getHiddenGrad(nextLayer);
		}
	}

	for (int i = 2; i > 0; i--)
	{
		for (int j = 0; j < layers[i].size() - 1; j++)
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