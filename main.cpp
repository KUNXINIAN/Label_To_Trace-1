#include "LabelRead.h"
#include "LibRead.h"
#include <fstream>
#include <iostream>
#include <sstream>
#define LABEL_PATH "C:/Users/Silence/Desktop/Label.csv"
#define LOG_PATH "C:/Users/Silence/Desktop/"
#define DATA_PATH "C:/Users/Silence/Desktop/Data/"

int main()
{
	LibRead lib_manager;
	lib_manager.getLib(LABEL_PATH);
	LabelRead label_manager(&lib_manager);
	string folder_name;
	vector<string> folder_list;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "欢迎使用惯导轨迹数据采集脚本，请输入数据名称，输入一组数据后回车确认，输入end结束输入。" << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "请输入数据名称:" << endl;
	while (getline(cin, folder_name))
	{
		if (folder_name == "end")
		{
			break;
		}
		else
		{
			folder_list.push_back(folder_name);
		}
		cout << "请输入数据名称:" << endl;
	}
	
	for (auto folder:folder_list)
	{
		string label_name = LOG_PATH + folder + "/data.txt";
		string log_name = LOG_PATH + folder + "/ins.log";
		label_manager.init();
		label_manager.logRead(log_name);
		label_manager.labelFileRead(label_name);
		label_manager.traceWrite(DATA_PATH + folder);
	}
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "								 处理完成								      " << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	return 0;
}