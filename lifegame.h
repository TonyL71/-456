#pragma once
#ifndef LIFEGAME_H
#define LIFEGAME_H
//项目—>属性->高级->使用多字节字符集
#include<iostream>
//Easy X
#include<graphics.h>
//这个库用来获取键盘
#include <conio.h>
//这个库用来实现多线程
#include<thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
	//定义一个SPEED来由用户决定演变速度
int SPEED;
std::thread getAndSetThread;
std::mutex field_mutex;
std::condition_variable cv;
std::atomic_bool is_continue(true);
#define cube_size 20
#define side 40
//媒体播放
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
	bool is_running;
	bool is_begin;
	bool field[side][side];
	bool ass[side][side];//用来记录一次演变后的细胞状态
	//更新一个格子的细胞状态
	int d_x[3] = { -1,0,1 };
	int d_y[3] = { -1,0,1 };
	bool update(int x, int y) {
		int sum = 0;//这里包括自己在内的9个格子
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
			{
				int dx = (x + d_x[i]);
				int dy = (y + d_y[j]);//遍历每一个周边的格子
				if (field[dx][dy]) {
					sum += 1;
				}
			}
		}
		if (field[x][y] == true) {
			if (sum < 3 || sum>4) { return false; }
			else { return true; }
		}
		else {
			if (sum == 3) { return  true; }
			else { return false; }
		}
	}
	//绘制地图
	void drawmap() {
		setfillcolor(BLACK);
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				if (field[i][j]) { fillrectangle(i * cube_size, j * cube_size, (i + 1) * cube_size, (j + 1) * cube_size); }
				else { clearrectangle(i * cube_size + 1, j * cube_size + 1, (i + 1) * cube_size - 1, (j + 1) * cube_size - 1); }
				//clear会连着边界绿线一起清理掉，故把边界向内缩一个像素
			}
		}
	}
	//演化函数
	void evolve() {
		//遍历每一个格子，根据每个格子周围的状态决定接下来细胞的状态是死是活
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				ass[i][j] = update(i, j);
			}
		}
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				field[i][j] = ass[i][j];
			}
		}
	}
	//（演变过程中）循环不断获取鼠标信息并根据鼠标信息设置map
	void getAndSet(bool (& field)[side][side]) {
		MOUSEMSG msg;
		while (is_continue) {
			//获取鼠标信息
			msg = GetMouseMsg();
			//根据鼠标信息设置map
			if (msg.mkLButton) {//鼠标左键按下 设置为存活
				//输出每一次鼠标点击的坐标
				cout << "\t x=" << msg.x / cube_size << "\t y=" << msg.y / cube_size << endl;
				field[msg.x / cube_size][msg.y / cube_size] = true;
			}
			else if (msg.mkRButton) {//鼠标右键点击，设置为不存活
				field[msg.x / cube_size][msg.y / cube_size] = false;
			}
			//这里注释掉drawmap函数是因为在主循环中不断画图已经足够，再增加这个就只会徒增功耗
					//drawmap();
		}
	}
	//定义一个新的函数，用来初始化活细胞并开始演变
	void newgetAndSet(bool(&field)[side][side]) {
		MOUSEMSG msg;
		msg = GetMouseMsg();
		//根据鼠标信息设置map
		if (msg.mkLButton) {//鼠标左键按下 设置为存活
			//输出每一次鼠标点击的坐标
			cout << "\t x=" << msg.x / cube_size << "\t y=" << msg.y / cube_size << endl;
			if ((side - 1) == (msg.y / cube_size) &&
				(side - 1) == (msg.x / cube_size)) {//点击右下角那个点开始演化
				is_begin = false;
				is_running = true;
				//播放音乐：卡农（音乐文件已经放在根目录里面了）
				mciSendString("open canno.MP3", NULL, NULL, NULL);
				mciSendString("play canno.mp3", NULL, NULL, NULL);
			}
			field[msg.x / cube_size][msg.y / cube_size] = true;
		}
		else if (msg.mkRButton) {//鼠标右键点击，设置为不存活
			field[msg.x / cube_size][msg.y / cube_size] = false;

		}
	}
	void play() {
		//初始化
		is_running = false;
		is_continue = true;
		is_begin = true;
		initgraph(cube_size * side, cube_size * side);
		setbkcolor(WHITE);
		setlinecolor(GREEN);
		cleardevice();
		field[side][side] = false;
		ass[side][side] = false;
		for (int i = 0; i < side; i++) {
			line(i * cube_size, 0, i * cube_size, side * cube_size);
			line(0, i * cube_size, side * cube_size, i * cube_size);
		}
		cout << "生命游戏，首先选择演变速度，用鼠标左键设置活细胞，右键设置细胞为死细胞" << endl;
		cout << "鼠标左键点击最右下角的细胞开始演化" << endl;
		cout << "演化过程中也可以用鼠标设置细胞状态" << endl;
		cout << "请选择演变速度：" << endl;
		cout << "输入1、2、3表示低速、中速和高速" << endl;
		int x;
		cin >> x;
		switch (x) {
		case 1:SPEED = 460;
			cout << "你选择了低速，可以开始设置细胞初始状态了" << endl; break;
		case 2:SPEED = 100;
			cout << "你选择了中速，可以开始设置细胞初始状态了" << endl; break;
		case 3:SPEED = 20;
			cout << "你选择了高速，可以开始设置细胞初始状态了" << endl; break;
		}
		cout << "演化过程中可以按Q、q键退出游戏" << endl;
		//开始游戏之前的设置初始化活细胞环节
		while (is_begin) {
			drawmap();
			newgetAndSet(field);
		}
		//启动一个新的线程
		getAndSetThread = std::thread(getAndSet, std::ref(field));
		BeginBatchDraw();
		//主循环
		while (is_running) {
			//绘图部分
			Sleep(SPEED);
			drawmap();
			//键鼠部分
			//getAndSet(field);
			//演化
			evolve();
			//双缓冲，提高帧率
			FlushBatchDraw();
			//多线程
			cv.notify_one();
			//按Q/q键可以退出程序（表现为暂停演化）
			if (_kbhit()) {
				char ch = _getch();
				if (ch == 'Q' || ch == 'q') {

					//关闭音乐
					mciSendString("close canno.mp3", NULL, NULL, NULL);
					MessageBox(GetHWnd(), "演变停止", "游戏结束", 0);
					cleardevice();
					closegraph();
					is_running = false;
					return;
				}
			}
		}
		//并行函数getAndSet
		getAndSet(field);
		getAndSetThread.join();
		EndBatchDraw();
		closegraph();
		//防止闪退
		getchar();
		//return 0;
	}
#endif
