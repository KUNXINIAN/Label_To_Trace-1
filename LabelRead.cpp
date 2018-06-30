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
	//��ͬ���ж�ʱ��Ҫ���³�ʼ��
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
		//���ȶ�ȡ��һ���ַ������ǿ����ֶΣ��� profile_line_num ++
		//���ƶ�Ӧ��д��data.txt����ǰ��
		if (line[0]==999)
		{
			//999�����ֶ�������˵�����ҽŷֱ�����Щģ��
			profile_line_num++;
			int l_r_flag = line[1];
			for (int j = 2; j<line.size(); j++)
			{
				left_right_foot_[line[j]] = l_r_flag;
			}
		}
		//else if ( )
		//{
			//���������ַ����������
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
	//��ģ����ͨ����ǩ����Ѱ�ұ�ǩ���꣬������ǩ������ӽ�service_loc_��ϣ��
	//0:�� 1����
	vector<Location> loc;
	for (auto line:line_data)
	{
		int left_right_flag = line[0];
		if (line.size()>2)
		{
			//����ǩʱ������Ѱ��
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
			//���ǩʱ������Ѱ��
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
//			cout << "Waring:  " << service_id << "��ģ������δ���룬�ߵ����ݼ���С���ǩ���ݼ���С��һ��" << endl;
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
	//�������ݵ�д��ģ��
	//�ʼ��˼·���Ա�ǩ����Ϊ׼��������ߵ����ݣ����ֹߵ����ݼ�����Զ���ڱ�ǩ����
	//���Ըı�˼·������ǩ���о��֣�ʹ�������ݳ�����ͬ��
	for (auto service : service_loc_)
	{
		int service_id = service.first;
		vector<Location> loc = getMapLoc(service_id);
		if (fabs(loc.size() - service_ins_[service_id].size())>2)
		{
			throw("Waring:" + to_string(service_id) + "��ģ������δ���룬�ߵ����ݼ���С���ǩ���ݼ���С��һ��");
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
	//���������α߳�֮�ȵ������Ʊ�
	while (size > 0)
	{
		double dis = cur_loc.calDist(loc_list[next_loc_index]);
		double cur_step = step;
		//���������ڽ�����������ǩ���ľ���ʱ��������������ǩ������
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
	//��log�����ж�ȡ����
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
		//��ȡins.log�Ļ���ԭ���ǣ���2����ʼλ��ʼ��¼��������ʱ�ڶ��ε���1��λ������
		int service_id = data[2];
		//3933����1��λ2��λ�ķ���װ��
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
