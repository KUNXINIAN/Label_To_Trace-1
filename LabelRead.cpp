#include "LabelRead.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

LabelRead::LabelRead(LibRead *libReader):lib_(libReader)
{
}

LabelRead::~LabelRead()
{
}

void LabelRead::init()
{
	loc_.clear(); 
	line_data_.clear();
	log_data_.clear();
}

void LabelRead::labelFileRead(string path)
{
	ifstream inFile;
	inFile.open(path);
	string line_string;
	vector<vector<int>> strArray;
	while (getline(inFile, line_string))
	{
		string temp;
		vector<int> temp_vec;
		temp_vec = GetStringFromlog(line_string,true);
		strArray.push_back(temp_vec);
	}
	int profile_line_num = 0;
	for (auto line :strArray)
	{
		if (line[0]!=0 && line[0]!=1)
		{
			profile_line_num++;
			int l_r_flag = line[1];
			for (int j = 2; j<line.size(); j++)
			{
				left_right_foot_[line[j]] = l_r_flag;
			}
		}
		else
		{
			break;
		}
	}
	strArray.erase(strArray.begin(), strArray.begin() + profile_line_num);
	locFind(strArray);
	return;
}

void LabelRead::locFind(vector<vector<int>> line_data)
{
	//0:左 1：右
	vector<Location> loc;
	for (auto line:line_data)
	{
		int left_right_flag = line[0];
		if (line.size()>2)
		{
			vector<Location> steps =  lib_->getLabelLocation(line[1], line[2], line[3]);
			for (auto step:steps)
			{
				for (auto service:left_right_foot_)
				{
					if (service.second == left_right_flag)
					{
						service_loc_[service.first].push_back(step);
					}
				}
			}
		}
		else
		{
			int left_right_flag = line[0];
			Location step = lib_->getLabelLocation(line[1]);

			for (auto service : left_right_foot_)
			{
				if (service.second == left_right_flag)
				{
					service_loc_[service.first].push_back(step);
				}
			}
		}
	}
	return;
}

void LabelRead::traceWrite(string path)
{
	for (auto service:service_loc_)
	{
		int service_id = service.first;
		if (service_ins_[service_id].size()!=service_loc_[service_id].size())
		{
			cout << "Waring:  " << service_id << "号模块数据未对齐，惯导数据集大小与标签数据集大小不一致" << endl;
		}
		fstream data_write;
		data_write.close();
		data_write.open(path + '-' + to_string(service_id) + ".csv",  std::ios::out);
		int length = service_ins_[service_id].size() > service_loc_[service_id].size() ? service_loc_[service_id].size() : service_ins_[service_id].size();
		for (int index = 0; index < length; index++)
		{
			for (auto axis: service_ins_[service_id][index])
			{
				data_write << setprecision(16)<< axis << ",";
			}
			data_write << setprecision(16) << service_loc_[service_id][index][0] <<","<<service_loc_[service_id][index][1]<<","<< service_loc_[service_id][index][2]<<endl;
		}
		data_write.close();
	}
}

void LabelRead::logRead(string path)
{
	ifstream inFile;
	inFile.open(path, std::ios::in);
	string line_string;
	vector<vector<double>> log_data;
	while (getline(inFile,line_string))
	{
		vector<double> data = GetStringFromlog(line_string);
		if (!data.empty())
		{
			log_data.push_back(data);
		}	
	}
	logDataProcess(log_data);
	return;
}

vector<int> LabelRead::GetStringFromlog(std::string line, bool int_flag)
{
	int index_begin = 0, index_end = 0;
	int length = 0;
	vector<int> line_data;
	for (index_end; index_end < line.size(); index_end++)
	{
		if (line[index_end] == '#')
		{
			return line_data;
		}
		if (line[index_end] == ' ' || line[index_end] == '\t' || line[index_end] == ',')
		{
			string data = line.substr(index_begin, length);
			line_data.push_back(stoi(data));
			index_begin = index_end + 1;
			length = 0;
		}
		else if (index_end == line.size() - 1)
		{
			string data = line.substr(index_begin, length + 1);
			line_data.push_back(stoi(data));
		}
		else
		{
			length++;
		}
	}
	return line_data;
}

vector<double> LabelRead::GetStringFromlog(std::string line)
{
	int index_begin = 0, index_end = 0;
	int length = 0;
	vector<double> line_data;
	for (index_end; index_end < line.size(); index_end++)
	{
		if (line[index_end] == '#')
		{
			return line_data;
		}
		if (line[index_end] == ' ' || line[index_end] == '\t' || line[index_end] == ',')
		{
			string data = line.substr(index_begin, length);
			line_data.push_back(stof(data));
			index_begin = index_end + 1;
			length = 0;
		}
		else if (index_end == line.size() - 1)
		{
			string data = line.substr(index_begin, length + 1);
			line_data.push_back(stof(data));
		}
		else
		{
			length++;
		}
	}
	return line_data;
}

void LabelRead::logDataProcess(vector<vector<double>> log)
{
	//map<int, vector<double>> service_ins_;
	//2:service_id
	//6:x_dis
	//7:y_dis
	//8:z_dis
	//9:3933_status
	map<int, serviceStatus> flag;
	for (vector<double> data : log )
	{
		int service_id = data[2];
		int status_3933 = data[9];
		flag[service_id].activity_flag = true;
		if (status_3933 == 1 && !flag[service_id].first_flag)
		{
			flag[service_id].first_flag = true;
		}
		if (status_3933 == 2 && !flag[service_id].second_flag)
		{
			flag[service_id].second_flag = true;
		}
		if (status_3933 == 2 && flag[service_id].first_flag && 
			!flag[service_id].second_flag)
		{
			flag[service_id].second_back_flag = true;
		}
		if (status_3933 == 1 && flag[service_id].first_flag && 
			flag[service_id].second_flag && !flag[service_id].second_back_flag)
		{
			flag[service_id].first_back_flag = true;
		}
		vector<double> step;
		if (flag[service_id].first_flag && flag[service_id].second_flag 
			/*&& !flag[service_id].first_back_flag*/)
		{
			step.push_back(data[6]);
			step.push_back(data[7]);
			step.push_back(data[8]);
			service_ins_[service_id].push_back(step);
		}
	}
}
