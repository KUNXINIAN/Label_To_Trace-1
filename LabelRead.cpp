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
	//不同的行动时需要重新初始化
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
		//首先读取第一个字符，若是控制字段，则 profile_line_num ++
		//控制段应该写在data.txt的最前面
		if (line[0]==999)
		{
			//999保留字段是用来说明左右脚分别都有哪些模块
			profile_line_num++;
			int l_r_flag = line[1];
			for (int j = 2; j<line.size(); j++)
			{
				left_right_foot_[line[j]] = l_r_flag;
			}
		}
		//else if ( )
		//{
			//其他控制字符在这里添加
		//}
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
	//本模块是通过标签序列寻找标签坐标，并将标签坐标添加进service_loc_哈希表
	//0:左 1：右
	vector<Location> loc;
	for (auto line:line_data)
	{
		int left_right_flag = line[0];
		if (line.size()>2)
		{
			//单标签时的坐标寻找
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
			//多标签时的坐标寻找
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

//void LabelRead::traceWrite(string path)
//{
//	for (auto service:service_loc_)
//	{
//		int service_id = service.first;
//		if (service_ins_[service_id].size()!=service_loc_[service_id].size())
//		{
//			cout << "Waring:  " << service_id << "号模块数据未对齐，惯导数据集大小与标签数据集大小不一致" << endl;
//		}
//		fstream data_write;
//		data_write.close();
//		data_write.open(path + '-' + to_string(service_id) + ".csv",  std::ios::out);
//		int length = service_ins_[service_id].size() > service_loc_[service_id].size() ? service_loc_[service_id].size() : service_ins_[service_id].size();
//		for (int index = 0; index < length; index++)
//		{
//			for (auto axis: service_ins_[service_id][index])
//			{
//				data_write << setprecision(16)<< axis << ",";
//			}
//			data_write << setprecision(16) << service_loc_[service_id][index][0] <<","<<service_loc_[service_id][index][1]<<","<< service_loc_[service_id][index][2]<<endl;
//		}
//		data_write.close();
//	}
//}

void LabelRead::traceWrite(string path)
{
	//测试数据的写入模块
	//最开始的思路是以标签序列为准绳，对齐惯导数据，后发现惯导数据集长度远大于标签序列
	//所以改变思路，将标签序列均分，使两者数据长度相同。
	for (auto service : service_loc_)
	{
		int service_id = service.first;
		vector<Location> loc = getMapLoc(service_id);
		if (fabs(loc.size() - service_ins_[service_id].size())>2)
		{
			throw("Waring:" + to_string(service_id) + "号模块数据未对齐，惯导数据集大小与标签数据集大小不一致");
		}
		fstream data_write;
		data_write.close();
		data_write.open(path + '-' + to_string(service_id) + ".csv",  std::ios::out);
		int length = service_ins_[service_id].size() > loc.size() ? loc.size() : service_ins_[service_id].size();
		for (int index = 0; index < length; index++)
		{
			for (auto axis: service_ins_[service_id][index])
			{
				data_write << setprecision(16)<< axis << ",";
			}
			data_write << setprecision(16) << loc[index][0] <<","<< loc[index][1]<<","<< loc[index][2]<<endl;
		}
		data_write.close();
	}

}

vector<Location> LabelRead::getMapLoc(int service_id)
{
	int size = service_ins_[service_id].size();
	vector<Location> loc_list = service_loc_[service_id];
	vector<Location> coordinate_list;
	if (size < loc_list.size())
		return coordinate_list;
	if (loc_list.size() < 2)
		return coordinate_list;

	double total_length = getLength(service_id);
	double step = total_length / (double)size;
	Location cur_loc = loc_list[0];
	coordinate_list.push_back(cur_loc);
	int next_loc_index = 1;
	//相似三角形边长之比等于相似比
	while (size > 0)
	{
		double dis = cur_loc.calDist(loc_list[next_loc_index]);
		double cur_step = step;
		//当步长大于接下来两个标签点间的距离时，需往第三个标签点延伸
		while (next_loc_index<loc_list.size()-1 && dis < cur_step) {
			cur_step -= dis;
			dis = loc_list[next_loc_index].calDist(loc_list[next_loc_index + 1]);
			cur_loc = loc_list[next_loc_index];
			next_loc_index++;
		}
		double ratio = cur_step / dis;
		cur_loc = (loc_list[next_loc_index] - cur_loc)*ratio + cur_loc;
		coordinate_list.push_back(cur_loc);
		size--;
	}
	return coordinate_list;
}

double LabelRead::getLength(int service_id)
{
	vector<Location> loc_list = service_loc_[service_id];
	double sum = 0;
	if (loc_list.size() < 1)
		return 0.0;
	for (int index = 1;index<loc_list.size();index++)
	{
		sum += loc_list[index].calDist(loc_list[index - 1]);
	}
	return sum;
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
	//从log中逐行读取数据
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
		//读取ins.log的基本原则是，从2号起始位开始记录，到返回时第二次到达1号位结束。
		int service_id = data[2];
		//3933即是1号位2号位的发射装置
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
