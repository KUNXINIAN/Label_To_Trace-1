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
	���������������ɵ�ͼƥ���׼ѵ��������Ϊ���¼���ģ�飺
	main.cpp:������
	LibRead.cpp:��ȡ�ⶨ�ı�ǩ���������ļ�label.csv
	LabelRead.cpp:��ȡ�����ߵ�ģ�����߳���ins.log���Լ���¼�ı�ǩ�����ļ�data.txt
	author��Chengmo
	date��2018/6/13
*/

int main()
{
	int a = INT_MAX;
	LibRead lib_manager;
	//��ȡlabel.csv
	lib_manager.getLib(LABEL_PATH);
	LabelRead label_manager(&lib_manager);
	string folder_name;
	vector<string> folder_list;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "��ӭʹ�ùߵ��켣���ݲɼ��ű����������������ƣ�����һ�����ݺ�س�ȷ�ϣ�����end�������롣" << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "��������������:" << endl;
	//�����ж����ƣ�����֧�ֶ���ж�ͬʱ¼��
	while (getline(cin, folder_name)){
		if (folder_name == "end"){
			break;
		}
		else{
			folder_list.push_back(folder_name);
		}
		cout << "��������������:" << endl;
	}

	for (auto folder:folder_list){
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "������"<< folder <<"�ж�ִ���˵�������ȫƴ��" << endl;
		string name;
		cin >> name;
		string label_name = LOG_PATH + folder + "/data.txt";
		string log_name = LOG_PATH + folder + "/ins.log";
		label_manager.init();
		//��ȡdata.txt������ǩ�����ļ�
		label_manager.labelFileRead(label_name);
		//��ȡins.log��ԭʼ�ߵ������ļ�
		label_manager.logRead(log_name);

		//дѵ�����ļ���ÿ���ж�ÿ���ߵ�ģ�飬���е������ļ�
		//label_manager.traceWrite(DATA_PATH + folder + "-" +name);

		//д�����ļ�����ⲽ���Ƿ����
		label_manager.getStepLength(DATA_PATH + folder + "-" + name);
	}
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "								 �������								      " << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	return 0;
}