#pragma once
#ifndef FIVE_H
#define FIVE_H
#include <stdio.h>
#include <graphics.h>
#define MAP_ROW 15
#define MAP_COL 15

int Map[MAP_ROW][MAP_COL];

//游戏初始化
void GameInit();
//游戏绘制
void GameDraw();	
//数据更新
void GameUpdate();
//返回游戏结果	比赛结束返回1 仍比赛中返回0 
bool Juge();
//判断
void Game_Juge();	

int flag;	// 1白棋 或 -1黑棋
//COORD是一个结构体，有两个成员变量分别表示当前坐标X，Y
COORD currentPos, waitPos;

void play2()
{
	//在周围额外开辟一圈用来存放标志
	initgraph(420 + 60, 420 + 60, 0);
	GameInit();
	while (1)
	{
		GameDraw();
		GameUpdate();
	}
	closegraph();
	//return 0;
}

void GameInit()
{
	//初始化当前坐标和待下坐标
	ZeroMemory(&Map, sizeof(Map));
	ZeroMemory(&currentPos, sizeof(COORD));
	ZeroMemory(&waitPos, sizeof(COORD));
	//黑棋先下
	Map[7][7] = -1;	
	//白子后下
	flag = 1;		
	currentPos.X = 7;
	currentPos.Y = 7;
}

void GameDraw()
{
	BeginBatchDraw();
	/*******************		绘制棋盘			********************/
	char arr[20];
	//设置窗口背景颜色
	setbkcolor(WHITE);		
	//设置文字背景透明
	setbkmode(1);		
	//线样式设置
	setlinestyle(PS_SOLID, 2);
	//设置线颜色
	setlinecolor(BLACK);
	//设置文字颜色
	settextcolor(BLACK);	
	// 填充颜色设置
	setfillcolor(RGB(255, 205, 150));  
	cleardevice();						//清屏
	//绘制无边框的正方形
	solidrectangle(0, 0, 480, 480);		
	//绘制行
	for (int row = 1; row <= 15; row++)
	{
		//+64是为了使用大写字母的ASCII码
		outtextxy(10, row * 30 - 15, 64 + row);
		line(30, row * 30, 420 + 30, row * 30);
	}
	//绘制列
	for (int col = 1; col <= 15; col++)
	{
		wsprintf(arr, "%d", col);
		outtextxy(col * 30 - 5, 5, arr);
		line(col * 30, 30, col * 30, 420 + 30);
	}
	// 填充颜色设置为黑色
	setfillcolor(BLACK);   
	//绘制五个实心圆
	solidcircle(4 * 30, 4 * 30, 5);
	solidcircle(12 * 30, 4 * 30, 5);
	solidcircle(4 * 30, 12 * 30, 5);
	solidcircle(12 * 30, 12 * 30, 5);
	solidcircle(8 * 30, 8 * 30, 5);

	/*******************		绘制棋子			********************/
	for (int y = 0; y < MAP_ROW; y++)
	{
		for (int x = 0; x < MAP_COL; x++)
		{
			if (Map[y][x] == 1)
			{
				//绘制白子
				// 填充颜色设置为白色
				setfillcolor(WHITE);   
				solidcircle((x + 1) * 30, (y + 1) * 30, 10);
			}
			if (Map[y][x] == -1)
			{
				//绘制黑子
				 // 填充颜色设置为黑色
				setfillcolor(BLACK);  
				solidcircle((x + 1) * 30, (y + 1) * 30, 10);
			}
		}
	}
	/*******************		绘制状态			********************/
	//设置线颜色为红色
	setlinecolor(RED);				
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			//currentPos绘制
			//画横线
			line(currentPos.X * 30 + 30 - 15, currentPos.Y * 30 + 30 - 15 + y * 30,
				currentPos.X * 30 + 30 - 5, currentPos.Y * 30 + 30 - 15 + y * 30);
			line(currentPos.X * 30 + 30 + 5, currentPos.Y * 30 + 30 - 15 + y * 30,
				currentPos.X * 30 + 30 + 15, currentPos.Y * 30 + 30 - 15 + y * 30);
			//画竖线
			line(currentPos.X * 30 + 30 - 15 + 30 * y, currentPos.Y * 30 + 30 - 15,
				currentPos.X * 30 + 30 - 15 + 30 * y, currentPos.Y * 30 + 30 - 5);
			line(currentPos.X * 30 + 30 - 15 + 30 * y, currentPos.Y * 30 + 30 + 5,
				currentPos.X * 30 + 30 - 15 + 30 * y, currentPos.Y * 30 + 30 + 15);
			//waitPos绘制
			line(waitPos.X * 30 + 30 - 15, waitPos.Y * 30 + 30 - 15 + y * 30,
				waitPos.X * 30 + 30 - 5, waitPos.Y * 30 + 30 - 15 + y * 30);
			line(waitPos.X * 30 + 30 + 5, waitPos.Y * 30 + 30 - 15 + y * 30,
				waitPos.X * 30 + 30 + 15, waitPos.Y * 30 + 30 - 15 + y * 30);
			//画竖线
			line(waitPos.X * 30 + 30 - 15 + 30 * y, waitPos.Y * 30 + 30 - 15,
				waitPos.X * 30 + 30 - 15 + 30 * y, waitPos.Y * 30 + 30 - 5);
			line(waitPos.X * 30 + 30 - 15 + 30 * y, waitPos.Y * 30 + 30 + 5,
				waitPos.X * 30 + 30 - 15 + 30 * y, waitPos.Y * 30 + 30 + 15);
		}
	}
	EndBatchDraw();
}

void GameUpdate()
{
	//鼠标操作
	MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
	int x = msg.x;
	int y = msg.y;
	switch (msg.uMsg)
	{
	case WM_MOUSEMOVE:
		// 鼠标移动 确保鼠标在棋盘区域
		if (x >= 30 && x <= 450 && y >= 30 && y <= 450)
		{
			waitPos.X = (x - 30 + 15) / 30;
			waitPos.Y = (y - 30 + 15) / 30;
		}
		break;
	case WM_LBUTTONUP:
		//确保鼠标在棋盘区域
		if (x >= 15 && x <= 465 && y >= 15 && y <= 465)
		{
			//此处无子
			if (Map[(y - 30 + 15) / 30][(x - 30 + 15) / 30] == 0)
			{
				//下子，改变currentPos
				currentPos.X = (x - 30 + 15) / 30;
				currentPos.Y = (y - 30 + 15) / 30;
				//map赋值
				Map[currentPos.Y][currentPos.X] = flag;
				flag = -flag;
			}
		}
		GameDraw();
		Game_Juge();
		break;	// 按鼠标右键退出程序
	}
}
//Jude函数返回一个布尔值，true表示获胜
bool Juge()
{
	int begin = 0, end = 0;
	/*========左右========*/
	for (int index = 1; index <= 5; index++)
	{
		//往左找
		if (currentPos.X - index >= 0 && Map[currentPos.Y][currentPos.X - index] != Map[currentPos.Y][currentPos.X])
		{
			begin = index;
			break;
		}
		else if (currentPos.X - index < 0)
		{
			begin = index;
			break;
		}
	}
	for (int index = 1; index <= 5; index++)
	{
		//往右找
		if (currentPos.X + index < 15 && Map[currentPos.Y][currentPos.X + index] != Map[currentPos.Y][currentPos.X])
		{
			end = index;
			break;
		}
		else if (currentPos.X + index >= 15)
		{
			end = index;
			break;
		}
	}
	if (begin + end >= 6)
	{
		return true;
	}


	/*========上下========*/
	for (int index = 1; index <= 5; index++)
	{
		//往上找
		if (currentPos.Y - index >= 0 && Map[currentPos.Y - index][currentPos.X] != Map[currentPos.Y][currentPos.X])
		{
			begin = index;
			break;
		}
		else if (currentPos.Y - index < 0)
		{
			begin = index;
			break;
		}
	}

	for (int index = 1; index <= 5; index++)
	{
		//往下找
		if (currentPos.Y + index < 15 && Map[currentPos.Y + index][currentPos.X] != Map[currentPos.Y][currentPos.X])
		{
			end = index;
			break;
		}
		else if (currentPos.Y + index >= 15)
		{
			end = index;
			break;
		}
	}
	if (begin + end >= 6)
	{
		return true;
	}

	/*========\========*/
	for (int index = 1; index <= 5; index++)
	{
		//		\上找
		if (currentPos.Y - index >= 0 && currentPos.X - index >= 0 &&
			Map[currentPos.Y - index][currentPos.X - index] != Map[currentPos.Y][currentPos.X])
		{
			begin = index;
			break;
		}
		else if (currentPos.Y - index < 0 || currentPos.X - index < 0)
		{
			begin = index;
			break;
		}
	}
	for (int index = 1; index <= 5; index++)
	{
		//		\下找
		if (currentPos.Y + index < 15 && currentPos.X + index < 15 &&
			Map[currentPos.Y + index][currentPos.X + index] != Map[currentPos.Y][currentPos.X])
		{
			end = index;
			break;
		}
		else if (currentPos.Y + index >= 15 || currentPos.X + index >= 15)
		{
			end = index;
			break;
		}

	}
	if (begin + end >= 6)
	{
		return true;
	}
	/*========/========*/
	for (int index = 1; index <= 5; index++)
	{
		//		/上找
		if (currentPos.Y - index >= 0 && currentPos.X + index < 15 &&
			Map[currentPos.Y - index][currentPos.X + index] != Map[currentPos.Y][currentPos.X])
		{
			begin = index;
			break;
		}
		else if (currentPos.Y - index < 0 || currentPos.X + index >= 15)
		{
			begin = index;
			break;
		}
	}
	for (int index = 1; index <= 5; index++)
	{
		//		/下找
		if (currentPos.Y + index < 15 && currentPos.X - index >= 0 &&
			Map[currentPos.Y + index][currentPos.X - index] != Map[currentPos.Y][currentPos.X])
		{
			end = index;
			break;
		}
		else if (currentPos.Y + index >= 15 || currentPos.X - index < 0)
		{
			end = index;
			break;
		}
	}
	if (begin + end >= 6)
	{
		return true;
	}
	return false;
}

void Game_Juge()
{
	/****************		判断输赢			******************/
	if (Juge())
	{
		if (Map[currentPos.Y][currentPos.X] == -1)
		{
			MessageBox(GetHWnd(), "黑棋胜", "比赛结束", 0);
			GameInit();
			return;
		}
		if (Map[currentPos.Y][currentPos.X] == 1)
		{
			MessageBox(GetHWnd(), "白棋胜", "比赛结束", 0);
			GameInit();
			return;
		}
	}
}
#endif
