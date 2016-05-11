#include "fighter.h"
#include "network.h"

Network::Network()
{
    //layers
    for (int i = 0; i < 3; i++)
    {
        layers.push_back(vector<Node>());
    }
    //inputs
    for (int i = 0; i < 3; i++)
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

void Network::predict(vector<double> &inputs)
{
    for (int i = 0; i < inputs.size(); i ++)
    {
        layers[0][i].output = inputs[i];
    }
    for (int i = 0; i < layers[1].size() - 1; i ++)
    {
        layers[1][i].push(layers[0]);
    }
    layers[2][0].push(layers[1]);
}

double Network::getOutput()
{
    return layers.back()[0].output;
}
