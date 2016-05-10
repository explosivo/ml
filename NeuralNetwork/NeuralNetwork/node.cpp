#include "node.h"

Node::Node(int numOfOutputs, int index)
{
    this->index = index;
    for (int i = 0; i < numOfOutputs; i ++) {
        weights.push_back(getRandom());
        dWeights.push_back(0.0);
    }
}

void Node::push(const vector<Node> &lastLayer)
{
    double sum = 0.0;
    for (int i = 0; i < lastLayer.size(); i ++) {
        sum += lastLayer[i].output *
        lastLayer[i].weights[index];
    }
    output = activation(sum);
}

void Node::updateInputWeights(vector<Node> &lastLayer)
{
    for (int i = 0; i < lastLayer.size(); i ++)
    {
        double dWeight = eta * lastLayer[i].output * gradient + learningrate * lastLayer[i].dWeights[index];
        lastLayer[i].dWeights[index] = dWeight;
        lastLayer[i].weights[index] += dWeight;
    }
}