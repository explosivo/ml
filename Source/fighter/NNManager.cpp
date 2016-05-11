#pragma once

#include "fighter.h"
#include "NNManager.h"

NNManager::NNManager()
{
	LoadNetwork();
}

NNManager::~NNManager()
{
}

double NNManager::Normalize(float x, int i)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%f %f %f"), x, minSet[i], maxSet[i]));
	return (double)((x - minSet[i]) / (maxSet[i] - minSet[i]));
}

int NNManager::Predict(float PlayerState, float Distance, float PlayerHealth, float AgentHealth, float RemainingTime)
{
	vector<double> set;
	set.push_back(Normalize(PlayerState, 0));
	set.push_back(Normalize(Distance, 1));
	set.push_back(Normalize(PlayerHealth, 2));
	set.push_back(Normalize(AgentHealth, 3));
	set.push_back(Normalize(RemainingTime, 4));
	network.predict(set);
	return ceil(network.getOutput()*4.0);
}

void NNManager::LoadNetwork()
{
	minSet.clear();
	maxSet.clear();
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

		for (int i = 0; i < network.layers.size() - 1; i++)
		{
			for (int j = 0; j < network.layers[i].size(); j++)
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