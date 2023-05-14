#include<stdio.h>  
#include<time.h>  
#include<windows.h>  
#include<stdlib.h>  

#include <iostream>
#include <conio.h>
#include "cmd_console_tools.h"
using namespace std;
#define U 1  
#define D 2  
#define L 3  
#define R 4       //蛇的状态，U：上 ；D：下；L:左 R：右  
#define LENTH 54 
#define WIDTH 26

typedef struct SNAKE { //蛇身的一个节点  
	int x;
	int y;
	struct SNAKE* next;
} snake;

//全局变量//  
int score = 0, add = 10;//总得分与每次吃食物得分。  
int status, sleeptime = 200;//每次运行的时间间隔  
snake* head, * food;//蛇头指针，食物指针  
snake* q;//遍历蛇的时候用到的指针  
int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。  
int judge = 1;
//声明全部函数//  
void creatMap();
void initsnake();
int biteself();
void createfood();
void cantcrosswall();
void snakemove();

void gamecircle1();
void gamecircle2();
void gamecircle3();

void welcometogame1();
void welcometogame2();

void endgame1();
void endgame2();

void final_end();

void gamestart1();
void gamestart2();
void gamestart3();

void creatMap() { //创建地图  
	int i;
	for (i = 0; i < LENTH + 2; i += 2) { //打印上下边框  
		cct_gotoxy(i, 0);
		printf("■");
		cct_gotoxy(i, WIDTH);
		printf("■");
	}
	for (i = 1; i < WIDTH; i++) { //打印左右边框  
		cct_gotoxy(0, i);
		printf("■");
		cct_gotoxy(LENTH, i);
		printf("■");
	}
}

void initsnake() { //初始化蛇身  
	snake* tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置//  
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++) {
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL) { //从头到尾，输出蛇身  
		cct_gotoxy(tail->x, tail->y);
		printf("■");
		tail = tail->next;
	}
}

int biteself() { //判断是否咬到了自己  
	snake* self;
	self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {
			return 1;
		}
		self = self->next;
	}
	return 0;
}

void createfood() { //随机出现食物  
	snake* food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0) {  //保证其为偶数，使得食物能与蛇头对其  
		food_1->x = rand() % (LENTH - 2) + 2;
	}
	food_1->y = rand() % (WIDTH - 2) + 1;
	q = head;
	while (q->next == NULL) {
		if (q->x == food_1->x && q->y == food_1->y) { //判断蛇身是否与食物重合  
			free(food_1);
			createfood();
		}
		q = q->next;
	}
	cct_gotoxy(food_1->x, food_1->y);
	food = food_1;
	printf("■");
}

void cantcrosswall() { //不能穿墙  
	if (head->x == 0 || head->x == LENTH || head->y == 0 || head->y == WIDTH) {
		endgamestatus = 1;
		judge = 0;
	}
}

void snakemove() { //蛇前进,上U,下D,左L,右R  
	snake* nexthead;
	cantcrosswall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U) {
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y) { //如果下一个有食物//  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                                           //如果没有食物//  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			cct_gotoxy(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == D) {
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y) { //有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                           //没有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			cct_gotoxy(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L) {
		nexthead->x = head->x - 2;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y) { //有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                            //没有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			cct_gotoxy(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == R) {
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y) { //有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                                     //没有食物  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("■");
				q = q->next;
			}
			cct_gotoxy(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (biteself() == 1) {     //判断是否会咬到自己  
		endgamestatus = 2;
		judge = 0;
	}
}


void gamecircle1() { //控制游戏  

	cct_gotoxy(64, 15);
	printf("不要穿墙，不要咬到自己\n");
	cct_gotoxy(64, 16);
	printf("用↑.↓.←.→分别控制蛇的移动.");
	cct_gotoxy(64, 20);
	status = R;
	while (1) {
		cct_gotoxy(64, 10);
		printf("得分：%d  ", score);
		if (score == 30) {
			sleeptime = 10;
		}

		if (GetAsyncKeyState(VK_UP) && status != D) {
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U) {
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R) {
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L) {
			status = R;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		Sleep(sleeptime);

		snakemove();
		if (judge == 0) {
			break;
		}
	}
}
void gamecircle2() { //控制游戏  
	score = 0;
	cct_gotoxy(60, 7);
	printf("1.不要穿墙，不要咬到自己\n");
	cct_gotoxy(60, 8);
	printf("2.不得使用宿舍中的插头，无论它们看起来是否完好。\n");
	cct_gotoxy(60, 9);
	printf("3.晾衣架上不能有衣服，包括鞋子。\n");
	cct_gotoxy(60, 10);
	printf("4.晾衣架上不能有衣服，包括面罩。\n");
	cct_gotoxy(60, 11);
	printf("5.笔记本电脑是大功率电器。笔记本电脑是大功率电器。笔记本电脑是大功率电器。\n");
	cct_gotoxy(60, 12);
	printf("6.不要使用大功率电器。\n");
	cct_gotoxy(60, 13);
	printf("7.当你遇到“他们”中的“年轻人”时，不要违反第6条规则。\n");
	cct_gotoxy(60, 14);
	printf("8.回答他们的问题。\n");

	cct_gotoxy(60, 16);
	printf("用↑.↓.←.→分别控制移动控制移动控制控制控制控制控制控制\n控制控制控制控制控制控制\n控制控制    控制控制控制控制控制控 制控 制控制控制控制\n\n\n      控制控制控制");

	char kind_of_shape[6]={'@','$','#','&','*','?'};
	for (int i = 0; i <= 100; i++) {
		cct_gotoxy(rand() % LENTH, rand() % WIDTH);
		int kind = (rand() % 6 - 1);
		printf("%c", kind_of_shape[kind]);
	}



	cct_gotoxy(60, 20);
	status = R;
	int count = 0;
	while (1) {
		cct_gotoxy(64, 18);
		printf("得分：%d  ", 99999999-score);
		if (score == 30) {
			sleeptime = 10;
		}

		if (GetAsyncKeyState(VK_UP) && status != D) {
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U) {
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R) {
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L) {
			status = R;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		Sleep(sleeptime);
		snakemove();
		
		if (judge == 0) {
			break;
		}
	}
}
void gamecircle3() { //控制游戏  

	const int MAX_X = 54, MAX_Y = 26;
	int x, y, fg_color, bg_color, num;
	char ch;

	srand((unsigned int)(time(0))); //生成种子

	for (num = 0; num < 150; num++) {
		x = rand() % (MAX_X + 1);
		y = rand() % (MAX_Y + 1);
		fg_color = rand() % 16;
		bg_color = rand() % 16;
		ch = 33 + rand() % 94;   //ASCII码表中33-126为可见图形字符

		/* 显示一个1 */
		cct_showch(x, y, ch, bg_color, fg_color, 2);

		/* 延时0.05秒 */
		Sleep(10);
	}  //end of for

	cct_gotoxy(60, 20);
	status = R;
	int count = 0;
	
	while (1) {
		for (int j = 60; j <= 92; j = j + 8) {
			for (int i = 1; i <= 20; i++) {
				cct_gotoxy(j, i);
				bg_color = COLOR_BLACK;
				fg_color = COLOR_RED;
				cct_showstr(j, i, "得分：0", bg_color, fg_color, 8);
			}
		}
		if (score == 10) {
			sleeptime = 20;
		}

		if (GetAsyncKeyState(VK_UP) && status != D) {
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U) {
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R) {
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L) {
			status = R;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		Sleep(sleeptime);
		snakemove();

		if (judge == 0) {
			break;
		}
	}
}

void welcometogame1() { //开始界面  
	cct_gotoxy(15, 10);
	printf("===================================");
	cct_gotoxy(15, 11);
	printf("欢迎来到这个贪食蛇游戏！");
	cct_gotoxy(15, 12);
	printf("用↑.↓.←.→分别控制蛇的移动\n");
	cct_gotoxy(15, 13);
	printf("吃的多的是赢家！");
	cct_gotoxy(15, 14);
	printf("===================================\n\n");
	system("pause");
	system("cls");
}
void welcometogame2() { //开始界面  
	cct_gotoxy(15, 10);
	printf("===========   = ========= =     ========   ===============  == == ==========   =============================");
	cct_gotoxy(15, 11);
	printf("欢#$迎%#)($#贪%食蛇:>::lig(&yuhb\n");
	cct_gotoxy(15, 12);
	printf("移动移移 动动移S  动F移动移动S  移SD移 SDFDG+^732动SAS动移动移动 移移动动移  动移动\n");
	cct_gotoxy(15, 13);
	printf("赢家！BUYAObuy不要输不会输 &￥&输GS*&输输 输输输输输输输输输输输  ？》”：输输输");
	cct_gotoxy(15, 14);
	printf("===   ======== ===========  ==============    ============    =================    ==== = =====  ====================\n\n");
	system("pause");  
	system("cls");
}

void endgame1() { //结束游戏  


	system("cls");
	cct_gotoxy(24, 12);
	if (endgamestatus == 1) {
		printf("对不起，您撞到墙了。游戏结束.");
	}
	else if (endgamestatus == 2) {
		printf("对不起，您咬到自己了。游戏结束.");
	}
	cct_gotoxy(24, 13);
	printf("您的得分是%d,您难道不想再来一局吗？输入y重来吧！\n", score);
	std::cin.clear(); //清除错误状态
	std::cin.ignore(100, '\n'); //忽略输入缓冲区中的所有字符，直到遇到换行符
	while (true) {
		if (_kbhit()) {  // 如果有键盘输入
			int key_code = _getch();  // 获取键码
			if (key_code == 'y' || key_code == 'Y') {
				break;  // 结束检测
			}
			else {
				cout << "输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y输入y" << endl;
			}
		}
	}
}
void endgame2() { //结束游戏  

	system("cls");
	cct_gotoxy(7, 6);
	cout << "LOSTLOSTLOSTLOS TLOSTL  OSTLOSTLOSTLOSTLOSTLO  STLOSTLOSTLOSTLOSTLOSTLO  STLOSTLOSTLOSTL  OSTLOSTLOSTL OSTLOSTL   OSTLOSTLOSTLOSTLOSTLOS     TLOSTLOSTLOSTLOSTLOST\n      LOSTLOSTLOSTLOSTLOST LOST LOSTL  OSTLO STLOSTLOSTLOST\n                           LOSTLOSTLOSTLOSTLOSTLOST LO S   TL   OSTLOSTLOSTLOSTLOST\nL        OSTL                  OSTL OSTLOSTLOSTLOSTLOSTLOST                   LOSTLOSTLOSTLOST";
	cct_gotoxy(24, 12);
	
	if (endgamestatus == 1) {
		printf("对不起，为什么为什么您撞到墙了为什么为什么为什么。为什么游戏为什么结束.");
	}
	else if (endgamestatus == 2) {
		printf("对不起，为什么为什么您咬到自己了为什么为什么为什么。为什么游戏为什么结束.");
	}
	cct_gotoxy(14, 13);
	printf("再来一局再来一局再来一局再来一局再来一局再来一局再来一局再来一局再来一局   再来一局再来一局再来一局\n再来一局再来一局   再来一局再来一局   再来一局再来一局再来一局再来一局\n再来一局再   来一局再来一局再来一局再来     一局再来一局再来一  局再来一局\n再来   一局  再来一局再来一局再来一局再来一局再来一局   再来一局再来一局输入y重   来 输入  y重 来  输入 y重来,\n输入y  重来输入y  重来输入y重来   输入y重来\n输入y重来输入y   重来输入y重来     输入y重来 输入y重来输入y重来输入y重来\n\n\n");
	std::cin.clear(); //清除错误状态
	std::cin.ignore(100, '\n'); //忽略输入缓冲区中的所有字符，直到遇到换行符
	while (true) {
		if (_kbhit()) {  // 如果有键盘输入
			int key_code = _getch();  // 获取键码
			if (key_code == 'y'||key_code=='Y') {
				break;  // 结束检测
			}
			else {
				cout << " 是y is y 是yis yis y是yi是ys 是y yis yv 是y is yis yis yis yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" << endl;
			}
		}
	}
}

void final_end() {
	int fg_color, bg_color;
	for (int j = 1; j <= 70; j = j + 10) {
		for (int i = 1; i <= 30; i++) {
			cct_gotoxy(j, i);
			bg_color = COLOR_BLACK;
			fg_color = COLOR_RED;
			cct_showstr(j, i, " 您还是输了 ", bg_color, fg_color, 8);
			Sleep(10);
		}
	}
}

void gamestart1() { //游戏初始化  

	welcometogame1();
	creatMap();
	initsnake();
	createfood();
}
void gamestart2() { //游戏初始化  

	welcometogame2();
	creatMap();
	initsnake();
	createfood();
}
void gamestart3() { //游戏初始化  

	creatMap();
	initsnake();
	createfood();
}


void two_dimential_snake() {

	cct_setfontsize("Terminal", 30, 15);
	gamestart1();
	gamecircle1();
	endgame1();
	

	cct_cls();
	head=NULL, food=NULL;
	q=NULL;  
	endgamestatus = 0;   
	judge = 1;
	sleeptime = 200;
	gamestart2();
	gamecircle2();
	endgame2();
	

	cct_cls();
	head = NULL, food = NULL;
	q = NULL;
	endgamestatus = 0;
	judge = 1;
	sleeptime = 20;
	score = 0;
	gamestart3();
	gamecircle3();

	final_end();
}