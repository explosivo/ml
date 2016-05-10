#include "node.h"

Node::Node(int numOfOutputs, int index)
{
	this->index = index;
	for (int i = 0; i < numOfOutputs; i++)
	{
		Connection con;
		con.weight = getRandom();
		outputWeights.push_back(con);
	}
}

void Node::next(const vector<Node> &lastLayer)
{
	double sum = 0;
	for (int i = 0; i < lastLayer.size(); i++)
	{
		sum += lastLayer[i].outputWeights[index].weight * lastLayer[i].output;
	}
	output = activationFunction(sum);
}

void Node::getOutputGrad(double target)
{
	double d = target - output;
	gradient = d * activationFunctionDerivative(output);
}

void Node::getHiddenGrad(const vector<Node> &nextLayer)
{
	double sum = 0;
	for (int i = 0; i < nextLayer.size(); i++)
	{
		sum += outputWeights[i].weight * nextLayer[i].gradient;
	}
	gradient = sum * activationFunctionDerivative(output);
}

void Node::updateInputWeights(vector<Node> &lastLayer)
{
	for (int i = 0; i < lastLayer.size(); i++)
	{
		Node node = lastLayer[i];
		double updatedDWeight = eta * node.output * gradient + (alpha * node.outputWeights[index].dWeight);
		node.outputWeights[index].dWeight = updatedDWeight;
		node.outputWeights[index].weight += updatedDWeight;
	}
}