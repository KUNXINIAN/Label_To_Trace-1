# **Label_To_Trace**  
*获取轨迹标签数据、惯导数据，比对标签坐标库，对齐时间戳，合成地图匹配测试文件*  
1. 主文件：main.cpp  
2. 标签坐标读取类：LibRead  
3. 轨迹标签读取列：Labelead  
4. 标签测量文件：Label.csv  
5. 使用方法：  
  5.1 **更改main.cpp中的文件位置宏定义**  
  5.2 **在ins.log文件夹中新建一个txt文件，命名为data.txt**  
  5.3 **第一行输入999（*保留功能码*） 0/1（*左脚0，右脚1*） 模块序号**  
  5.4 **手动听写测试中的标签序列**  
  5.5 **运行main.cpp，在对话框中输入ins.log所在文件夹名（*行动名*）**  
  5.6 **在宏定义中指定的输出文件夹位置找到输出的测试集文件**  
