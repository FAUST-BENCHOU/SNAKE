#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include "cmd_console_tools.h"
#include <fstream>
#include <GL/glut.h>
using namespace std;
/*设置画面满屏，介绍页面(简单复古)，一维贪吃蛇（左右移动）（单调的bgm）*/

void one_dimensional_snake() {
	cct_setfontsize("Terminal", 64, 32);
	cct_gotoxy(14, 2);//光标到中间
	cout << "************************";
	cct_gotoxy(16, 3);
	cout << "欢迎来到贪吃蛇游戏";
	cct_gotoxy(16, 4);
	cout << "我很开心你能来玩";
	cct_gotoxy(16, 5);
	cout << "规则：按下<--方向键让它来动";
	cct_gotoxy(16, 6);
	cout << "按下1开始游戏";
	cct_gotoxy(14, 7);
	cout << "************************" << endl;
	char num;
	for (;;)
	{
		num = _getch() - '0';
		if (num == 1)
			break;
	}
	cct_cls();
	cct_setfontsize("Terminal", 20, 10);
	int x = 96, y = 24, lenth = 7;
	cct_gotoxy(x, y);
	cout << "+------";
	cct_gotoxy(x - 5, y);
	cout << "*";//food
	int key = 72;//表示蛇移动的方向，72为按下“↑”所代表的数字
	for (;;) {
		if (_kbhit())//如果用户按下了键盘中的某个键
		{
			fflush(stdin);//清空缓冲区的字符
			//getch()读取方向键的时候，会返回两次，第一次调用返回0或者224，第二次调用返回的才是实际值
			key = _getch();//第一次调用返回的不是实际值
			key = _getch();//第二次调用返回实际值
		}
		else { continue; }
		if (key == 75) {
			x--;
			cct_gotoxy(x, y); cout << "+";
			cct_gotoxy(x + 1, y); cout << "-";
			cct_gotoxy(x + lenth, y); cout << " ";
		}
		if (x == 91 || x == 75 || x == 59) {
			cct_gotoxy(x - 16, y);
			cout << "*";//food
			lenth++;
			cct_gotoxy(x + lenth - 1, y); cout << "-";
		}
		if (x == 38) { break; }
	}
	cct_gotoxy(0, y + 1);
	cout << "游戏结束，等待3s退出游戏";
	Sleep(3500);
	cct_gotoxy(0, y + 2);
	cout << " D: lMyGamelx64 \Debug MyGame.exe（进程26196)已退出，代码为0。" << endl << "按任意键关闭此窗口.. .";
	Sleep(3500);
}