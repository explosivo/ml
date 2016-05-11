#include "fighter.h"
#include "node.h"

Node::Node(int numOfOutputs, int index)
{
    this->index = index;
    for (int i = 0; i < numOfOutputs; i ++) {
        weights.push_back(0);
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
    output = 1.0 / (1.0 + exp(-sum));
}