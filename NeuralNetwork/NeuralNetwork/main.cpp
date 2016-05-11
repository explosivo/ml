#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.h"
#include "network.h"
#include "node.h"

using namespace std;

void readTrainingData();

vector<vector<double>> inputsSet;
vector<double> outputsSet;
vector<double> maxSet;
vector<double> minSet;

void normalize(vector<vector<double>> &set);
void saveData(const Network &network);
void loadNetwork(Network &network);

int main()
{
    readTrainingData();
    normalize(inputsSet);
    Network network;
    
    vector<double> inputVals, targetVals, resultVals;
    //for (int n = 0; n < 10; n ++)
    for (int i = 0; i < inputsSet.size(); i++)
    {
        network.predict(inputsSet[i]);
        cout << "output: " << network.getOutput();
        cout << " target: " << outputsSet[i];
        cout << " action: " << ceil(network.getOutput()*4.0) << endl;
        network.backPropagate(outputsSet[i]);
    }
    
    saveData(network);
    
	for (int i = 0; i < minSet.size(); i++)
	{
		cout << minSet[i] << " " << maxSet[i] << endl;
	}

    Network test;
    
    loadNetwork(test);
	for (int i = 0; i < minSet.size(); i ++)
	{
		cout << minSet[i] << " " << maxSet[i] << endl;
	}
       /* 
    for (int i = 0; i < 100; i++)
    {
        test.predict(inputsSet[i]);
        cout << "output: " << test.getOutput();
        cout << " target: " << outputsSet[i];
        cout << " action: " << ceil(test.getOutput()*4.0) << endl;
    }*/
	
    system("pause");
}

double max(vector<vector<double>> &set, int ind)
{
    double maxf = set[0][ind];
    for (int i = 1; i < set.size(); i++)
    {
        if (set[i][ind] > maxf)
            maxf = set[i][ind];
    }
    return maxf;
}

double min(vector<vector<double>> &set, int ind)
{
    double minf = set[0][ind];
    for (int i = 0; i < set.size(); i++)
    {
        if (set[i][ind] < minf)
            minf = set[i][ind];
    }
    return minf;
}

void normalize(vector<vector<double>> &set)
{
    for (int i = 0; i < set[0].size(); i++)
    {
        maxSet.push_back(max(set, i));
        minSet.push_back(min(set, i));
        for (int j = 0; j < set.size(); j++)
        {
            set[j][i] = (set[j][i] - minSet[i]) / (maxSet[i] - minSet[i]);
        }
    }
}

void readTrainingData()
{
    ifstream trainingData;
    trainingData.open("trainingData.dat");
    if (trainingData.is_open())
    {
        cout << "found" << endl;
        //PlayerState Distance PlayerHealth AgentState AgentHealth RemainingTime
        string line;
        while (getline(trainingData, line))
        {
            int count = 0;
            stringstream ss(line);
            string element;
            vector<double> inputs;
            vector<double> outputs;
            double temp;
            while (getline(ss, element, ' '))
            {
                switch (count)
                {
                    case 0:
                        temp = atof(element.c_str());
                        temp = temp / 4.0;
                        break;
                    case 1:
                        inputs.push_back(atof(element.c_str()));
                        break;
                    case 2:
                        //inputs.push_back(atof(element.c_str()));
                        break;
                    case 3:
                        inputs.push_back(atoi(element.c_str()));
                        break;
                    case 4:
                        //inputs.push_back(atof(element.c_str()));
                        break;
                    case 5:
                        //inputs.push_back(atof(element.c_str()));
                        break;
                };
                count++;
            }
            outputs.push_back(temp);
            outputsSet.push_back(temp);
            inputsSet.push_back(inputs);
            inputs.clear();
        }
    }
}

void saveData(const Network &network)
{
    ofstream outFile;
    outFile.open("network.dat");
    
    //print maxSet data
    outFile << maxSet[0];
    for (int i = 1; i < maxSet.size(); i ++)
    {
        outFile << " " << maxSet[i];
    }
    outFile << endl;
    
    //print minSet data
    outFile << minSet[0];
    for (int i = 1; i < minSet.size(); i ++)
    {
        outFile << " " << minSet[i];
    }
    outFile << endl;
    
    //print weights of each node
    for (int i = 0; i < network.layers.size() - 1; i ++)
    {
        for (int j = 0; j < network.layers[i].size(); j ++)
        {
            if (network.layers[i][j].weights.size() != 0)
                outFile << network.layers[i][j].weights[0];
            for (int h = 1; h < network.layers[i][j].weights.size(); h++)
            {
                outFile << " " << network.layers[i][j].weights[h];
            }
            outFile << endl;
        }
    }
}

void loadNetwork(Network &network)
{
	minSet.clear();
	maxSet.clear();
	cout << minSet.size() << " " << maxSet.size() << endl;
    ifstream dataFile;
    dataFile.open("network.dat");
    
    if (dataFile.is_open())
    {
        string line, element;

		getline(dataFile, line);
		stringstream ss(line);
		for (int i = 0; i < 5; i++)
		{
			getline(ss, element, ' ');

			maxSet.push_back(atof(element.c_str()));
		}

		getline(dataFile, line);
		ss = stringstream(line);
		for (int i = 0; i < 5; i++)
		{
			getline(ss, element, ' ');
			minSet.push_back(atof(element.c_str()));
		}

        for (int i = 0; i < network.layers.size() - 1; i ++)
        {
            for (int j = 0; j < network.layers[i].size(); j ++)
            {
                getline(dataFile, line);
                ss = stringstream(line);
                for (int h = 0; h < network.layers[i][j].weights.size(); h++)
                {
                    getline(ss, element, ' ');
                    network.layers[i][j].weights[h] = atof(element.c_str());
                }
            }
        }
    }
}