#include "LabelRead.h"
#include "LibRead.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#define LABEL_PATH "E:/data/MapMatcingData/Label.csv"
#define LOG_PATH "E:/data/MapMatcingData/origin_data/"
#define DATA_PATH "E:/data/MapMatcingData/process_data/"

/*
	本程序作用是生成地图匹配标准训练集，分为以下几个模块：
	main.cpp:主程序
	LibRead.cpp:读取测定的标签像素坐标文件label.csv
	LabelRead.cpp:读取穿戴惯导模块所走出的ins.log，以及记录的标签序列文件data.txt
	author：Chengmo
	date：2018/6/13
*/

int main()
{
	int a = INT_MAX;
	LibRead lib_manager;
	//读取label.csv
	lib_manager.getLib(LABEL_PATH);
	LabelRead label_manager(&lib_manager);
	string folder_name;
	vector<string> folder_list;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "欢迎使用惯导轨迹数据采集脚本，请输入数据名称，输入一组数据后回车确认，输入end结束输入。" << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "请输入数据名称:" << endl;
	//输入行动名称，可以支持多个行动同时录入
	while (getline(cin, folder_name)){
		if (folder_name == "end"){
			break;
		}
		else{
			folder_list.push_back(folder_name);
		}
		cout << "请输入数据名称:" << endl;
	}

	for (auto folder:folder_list){
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "请输入"<< folder <<"行动执行人的姓名（全拼）" << endl;
		string name;
		cin >> name;
		string label_name = LOG_PATH + folder + "/data.txt";
		string log_name = LOG_PATH + folder + "/ins.log";
		label_manager.init();
		//读取data.txt，即标签序列文件
		label_manager.labelFileRead(label_name);
		//读取ins.log，原始惯导数据文件
		label_manager.logRead(log_name);

		//写训练集文件，每个行动每个惯导模块，都有单独的文件
		//label_manager.traceWrite(DATA_PATH + folder + "-" +name);

		//写步长文件，检测步长是否均匀
		label_manager.getStepLength(DATA_PATH + folder + "-" + name);
	}
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "								 处理完成								      " << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	return 0;
}