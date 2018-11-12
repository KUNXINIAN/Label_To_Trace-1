clc;
clear;
close all;
for k=1:15
    filename=('E:\data\MapMatcingData\process_data\1104-13-117.csv');
    B=csvread(filename);%改为对应的CSV文件位子
    figure(2*k-1);
    for i=1:length(B)
        plot (B(i,2),B(i,1),'r*-');%画出惯性导航的数据的轨迹图
        text(B(i,2)+0.1,B(i,1),num2str(i));
        hold on;
        pause(0.1)
    end
    saveas(gcf,('E:\data\MapMatcingData\process_data\1104-13-117-ins'),'jpg');
    close;
    figure(2*k);
    for j=1:length(B)    
        plot (B(j,5),B(j,4),'b*-');%画出对应匹配像素坐标的轨迹图
        text(B(j,5)+0.1,B(j,4),num2str(j));
        pause(0.1)
        hold on;
    end
    hold on;
    saveas(gcf,('E:\data\MapMatcingData\process_data\1104-13-117-lable'),'jpg');
    close;
end

