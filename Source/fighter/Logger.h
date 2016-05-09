#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Logger
{
public:
	Logger();
	~Logger();

	void AddData(int Player, string Line);
	void Flush(int WinningPlayer);

	vector<string> Player1Data;
	vector<string> Player2Data;
};