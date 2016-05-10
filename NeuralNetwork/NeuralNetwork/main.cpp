#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.h"
#include "network.h"

using namespace std;
void readTrainingData();

vector<vector<double>> inputsSet;
vector<vector<double>> outputsSet;
void normalize(vector<vector<double>> &set);

int main()
{
	readTrainingData();
	normalize(inputsSet);

	vector<double> res;
	vector<unsigned> topology;
	topology.push_back(5);
	topology.push_back(7);
	topology.push_back(1);
	Network network(topology);
	for (int i = 0; i < inputsSet.size(); i++)
	{
		network.forwardPropagate(inputsSet[i]);
		network.backPropagate(outputsSet[i]);
		res = network.getResults();
		cout << i << " : " << res[0] << " " << outputsSet[i][0] << endl;
	}
	system("pause");
	return 0;
}

/*
// neural-net-tutorial.cpp
// David Miller, http://millermattson.com/dave
// See the associated video for instructions: http://vimeo.com/19569529


#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

void readTrainingData();

vector<vector<double>> inputsSet;
vector<vector<double>> outputsSet;

// Silly class to read training data from a text file -- Replace This.
// Replace class TrainingData with whatever you need to get input data into the
// program, e.g., connect to a database, or take a stream of data from stdin, or
// from a file specified by a command line argument, etc.

class TrainingData
{
public:
	TrainingData(const string filename);
	bool isEof(void) { return m_trainingDataFile.eof(); }
	void getTopology(vector<unsigned> &topology);

	// Returns the number of input values read from the file:
	unsigned getNextInputs(vector<double> &inputVals);
	unsigned getTargetOutputs(vector<double> &targetOutputVals);

private:
	ifstream m_trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned> &topology)
{
	string line;
	string label;

	getline(m_trainingDataFile, line);
	stringstream ss(line);
	ss >> label;
	if (this->isEof() || label.compare("topology:") != 0) {
		abort();
	}

	while (!ss.eof()) {
		unsigned n;
		ss >> n;
		topology.push_back(n);
	}

	return;
}

TrainingData::TrainingData(const string filename)
{
	m_trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(vector<double> &inputVals)
{
	inputVals.clear();

	string line;
	getline(m_trainingDataFile, line);
	stringstream ss(line);

	string label;
	ss >> label;
	if (label.compare("in:") == 0) {
		double oneValue;
		while (ss >> oneValue) {
			inputVals.push_back(oneValue);
		}
	}

	return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals)
{
	targetOutputVals.clear();

	string line;
	getline(m_trainingDataFile, line);
	stringstream ss(line);

	string label;
	ss >> label;
	if (label.compare("out:") == 0) {
		double oneValue;
		while (ss >> oneValue) {
			targetOutputVals.push_back(oneValue);
		}
	}

	return targetOutputVals.size();
}


struct Connection
{
	double weight;
	double deltaWeight;
};


class Neuron;

typedef vector<Neuron> Layer;

// ****************** class Neuron ******************
class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) const { return m_outputVal; }
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);

private:
	static double eta;   // [0.0..1.0] overall net training rate
	static double alpha; // [0.0..n] multiplier of last weight change (momentum)
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight(void) { return rand() / double(RAND_MAX); }
	double sumDOW(const Layer &nextLayer) const;
	double m_outputVal;
	vector<Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;
};

double Neuron::eta = 0.15;    // overall net learning rate, [0.0..1.0]
double Neuron::alpha = 0.5;   // momentum, multiplier of last deltaWeight, [0.0..1.0]


void Neuron::updateInputWeights(Layer &prevLayer)
{
	// The weights to be updated are in the Connection container
	// in the neurons in the preceding layer

	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		Neuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

		double newDeltaWeight =
			// Individual input, magnified by the gradient and train rate:
			eta
			* neuron.getOutputVal()
			* m_gradient
			// Also add momentum = a fraction of the previous delta weight;
			+ alpha
			* oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;

	// Sum our contributions of the errors at the nodes we feed.

	for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}

	return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
	// tanh - output range [-1.0..1.0]

	return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
	// tanh derivative
	return 1.0 - x * x;
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum = 0.0;

	// Sum the previous layer's outputs (which are our inputs)
	// Include the bias node from the previous layer.

	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		sum += prevLayer[n].getOutputVal() *
			prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}


// ****************** class Net ******************
class Net
{
public:
	Net(const vector<unsigned> &topology);
	void feedForward(const vector<double> &inputVals);
	void backProp(const vector<double> &targetVals);
	void getResults(vector<double> &resultVals) const;
	double getRecentAverageError(void) const { return m_recentAverageError; }

private:
	vector<Layer> m_layers; // m_layers[layerNum][neuronNum]
	double m_error;
	double m_recentAverageError;
	static double m_recentAverageSmoothingFactor;
};


double Net::m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over


void Net::getResults(vector<double> &resultVals) const
{
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

void Net::backProp(const vector<double> &targetVals)
{
	// Calculate overall net error (RMS of output neuron errors)

	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS

							 // Implement a recent average measurement

	m_recentAverageError =
		(m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
		/ (m_recentAverageSmoothingFactor + 1.0);

	// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}

	// Calculate hidden layer gradients

	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum) {
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum) {
		Layer &layer = m_layers[layerNum];
		Layer &prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n) {
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void Net::feedForward(const vector<double> &inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1);

	// Assign (latch) the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); ++i) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	// forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum) {
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have a new layer, now fill it with neurons, and
		// add a bias neuron in each layer.
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Made a Neuron!" << endl;
		}

		// Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
		m_layers.back().back().setOutputVal(1.0);
	}
}


void showVectorVals(string label, vector<double> &v)
{
	cout << label << " ";
	for (unsigned i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}

	cout << endl;
}

void normalize(vector<vector<double>> &set);

int main()
{
	//TrainingData trainData("trainingData.dat");
	readTrainingData();
	normalize(inputsSet);
	// e.g., { 3, 2, 1 }
	vector<unsigned> topology;
	topology.push_back(5);
	topology.push_back(7);
	topology.push_back(1);

	Net myNet(topology);

	vector<double> inputVals, targetVals, resultVals;
	int trainingPass = 0;
	for (int i = 0; i < inputsSet.size(); i++)
	{
		++trainingPass;
		cout << endl << "Pass " << trainingPass;
		/*
		// Get new input data and feed it forward:
		if (trainData.getNextInputs(inputVals) != topology[0]) {
			break;
		}*//*
		showVectorVals(": Inputs:", inputsSet[i]);
		myNet.feedForward(inputsSet[i]);

		// Collect the net's actual output results:
		myNet.getResults(resultVals);
		showVectorVals("Outputs:", resultVals);

		// Train the net what the outputs should have been:
		//trainData.getTargetOutputs(outputsSet[i]);
		showVectorVals("Targets:", outputsSet[i]);
		assert(outputsSet[i].size() == topology.back());

		myNet.backProp(outputsSet[i]);

		// Report how well the training is working, average over recent samples:
		cout << "Net recent average error: "
			<< myNet.getRecentAverageError() << endl;
	}

	cout << endl << "Done" << endl;
	system("pause");
}*/

vector<double> maxSet;
vector<double> minSet;

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
	double minf = -1;
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
			//double outputArr[5] = { 0, 0, 0, 0, 0 };
			while (getline(ss, element, ' '))
			{
				switch (count)
				{
				case 0:
					temp = atof(element.c_str());
					temp = temp / 4.0;
					/*
					if (temp == 0)
						outputArr[0] = 1;
					if (temp == 1)
						outputArr[1] = 1;
					if (temp == 2)
						outputArr[2] = 1;
					if (temp == 3)
						outputArr[3] = 1;
					if (temp == 4)
						outputArr[4] = 1;
						*/
					break;
				case 1:
					inputs.push_back(atof(element.c_str()));
					break;
				case 2:
					inputs.push_back(atof(element.c_str()));
					break;
				case 3:
					inputs.push_back(atoi(element.c_str()));
					break;
				case 4:
					inputs.push_back(atof(element.c_str()));
					break;
				case 5:
					inputs.push_back(atof(element.c_str()));
					break;
				};
				count++;
			}/*
			for (int i = 0; i < 5; i++)
			{
				outputs.push_back(outputArr[i]);
			}*/
			outputs.push_back(temp);
			outputsSet.push_back(outputs);
			inputsSet.push_back(inputs);
			inputs.clear();
		}
	}
}