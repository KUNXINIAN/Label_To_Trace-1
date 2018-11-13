#pragma once
#include "LibRead.h"
#include <fstream>
#include <iostream>
#include <string>

struct serviceStatus
{
	bool activity_flag = false;
	bool start_flag = false;
};

class LabelRead
{
public:
	LabelRead(LibRead *lib);
	~LabelRead();
	void init();
	void stop();
	void labelFileRead(string path);
	void locFind(vector<vector<int>> line_data);
	void traceWrite(string path);
	void logRead(string path);
	vector<int> GetStringFromlog(std::string line, bool int_flag);
	vector<double> GetStringFromlog(std::string line);
	vector<Location> getMapLoc(int service_id);
	double getLength(int service_id);
	vector<int> getStartEndIndex(int service_id, vector<vector<double>> log);
	void getStepLength(string path);
	

private:
	LibRead *lib_;
	map<int, int> left_right_foot_;//0:×ó 1£ºÓÒ
	map<int, vector<Location>> service_loc_;
	map<int, vector<vector<double>>> service_ins_;
	map<int, vector<Location>> map_loc_;
	vector<Location> loc_;
	vector<vector<string>> line_data_;
	vector<vector<double>> log_data_;

	void logDataProcess(vector<vector<double>> log);

};


