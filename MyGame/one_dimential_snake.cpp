#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>//windows���ͷ�ļ�
#include <time.h>
#include <conio.h>//����̨�������ͷ�ļ�
#include "cmd_console_tools.h"
#include <fstream>
#include <GL/glut.h>
using namespace std;
/*���û�������������ҳ��(�򵥸���)��һά̰���ߣ������ƶ�����������bgm��*/

void one_dimensional_snake() {
	cct_setfontsize("Terminal", 64, 32);
	cct_gotoxy(14, 2);//��굽�м�
	cout << "************************";
	cct_gotoxy(16, 3);
	cout << "��ӭ����̰������Ϸ";
	cct_gotoxy(16, 4);
	cout << "�Һܿ�����������";
	cct_gotoxy(16, 5);
	cout << "���򣺰���<--�������������";
	cct_gotoxy(16, 6);
	cout << "����1��ʼ��Ϸ";
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
	int key = 72;//��ʾ���ƶ��ķ���72Ϊ���¡����������������
	for (;;) {
		if (_kbhit())//����û������˼����е�ĳ����
		{
			fflush(stdin);//��ջ��������ַ�
			//getch()��ȡ�������ʱ�򣬻᷵�����Σ���һ�ε��÷���0����224���ڶ��ε��÷��صĲ���ʵ��ֵ
			key = _getch();//��һ�ε��÷��صĲ���ʵ��ֵ
			key = _getch();//�ڶ��ε��÷���ʵ��ֵ
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
	cout << "��Ϸ�������ȴ�3s�˳���Ϸ";
	Sleep(3500);
	cct_gotoxy(0, y + 2);
	cout << " D: lMyGamelx64 \Debug MyGame.exe������26196)���˳�������Ϊ0��" << endl << "��������رմ˴���.. .";
	Sleep(3500);
}