#include "LibRead.h"
LibRead::LibRead()
{
}

LibRead::~LibRead()
{
}

void LibRead::getLib(string path)
{
	ifstream inFile;
	inFile.open(path);
	string line_string;
	while (getline(inFile,line_string))
	{
		vector<double> axis = GetStringFromlog(line_string);
		//Lib_是一个哈希表，通过查表的方式实现坐标的查找
		Lib_[(int)axis[0]] = { axis[1],axis[2],axis[3] };
	}
}

vector<double> LibRead::GetStringFromlog(std::string line)
{
	//读取每一行的输入，解析出每一个元素的值
	int index_begin = 0, index_end = 0;
	int length = 0;
	vector<double> line_data;
	for (index_end; index_end < line.size(); index_end++)
	{
		if (line[index_end] == '#')
		{
			continue;
		}
		if (line[index_end] == ' ' || line[index_end] == '\t'|| line[index_end] == ',')
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

Location LibRead::getLabelLocation(int label)
{
	//通过标签序号查找坐标，若查找不到该标签的序号，则会返回（-1，-1，-1）
	if (fabs(label - (double)(int)label<EPS))
	{
		if (Lib_.find(label)!=Lib_.end())
		{
			return Lib_[label];
		}
		else
		{
			cout <<label<<" "<< "Label didn't find" << endl;
		}
	}
	else
	{
		cout <<"Single"<<" "<< label << " " << "illegal input" << endl;
	}
	return Location(-1,-1,-1);
}

vector<Location> LibRead::getLabelLocation(int label_1, int label_2, int step_num)
{
	//根据标签获取坐标函数的重载，支持同时输入两个标签，以及一个步数的输入，
	//通过两个标签的坐标差取等分得到最终的坐标
	Location loc_pre = getLabelLocation(label_1);
	Location loc_end = getLabelLocation(label_2);
	Location dis = (loc_pre-loc_end)/step_num;


	vector<Location> trace;
	for (int i = 1; i <= step_num; i++)
	{
		Location loc = loc_pre+dis*i;
		trace.push_back(loc);
	}

	return trace;
}
