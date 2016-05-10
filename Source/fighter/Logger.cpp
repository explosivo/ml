#include "fighter.h"
#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{

}

void Logger::AddData(int Player, string Line)
{
	if (Player == 1)
	{
		Player1Data.push_back(Line);
	}
	else if (Player == 2)
	{
		Player2Data.push_back(Line);
	}
}

void Logger::Flush(int WinningPlayer)
{
	ofstream outFile;
	outFile.open("trainingData.dat", ios::out | ios::app);
	if (WinningPlayer == 1)
	{
		for (int i = 0; i < Player1Data.size(); i++)
		{
			outFile << Player1Data[i];
			outFile << "\n";
		}
	}
	else if (WinningPlayer == 2)
	{
		for (int i = 0; i < Player2Data.size(); i++)
		{
			outFile << Player2Data[i];
			outFile << "\n";
		}
	}
	Player1Data.clear();
	Player2Data.clear();
	outFile.close();
}