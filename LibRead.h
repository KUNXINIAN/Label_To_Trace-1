#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include "types.h"
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;



class LibRead
{
public:
	LibRead();
	~LibRead();
	void getLib(string path);
	vector<double> GetStringFromlog(std::string line);
	Location getLabelLocation(int label);
	vector<Location> getLabelLocation(int label_1, int label_2, int step_num);

private:
	map<int,Location> Lib_;
	const double EPS = 1e-6;
};

