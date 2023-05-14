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
#define R 4       //�ߵ�״̬��U���� ��D���£�L:�� R����  
#define LENTH 54 
#define WIDTH 26

typedef struct SNAKE { //�����һ���ڵ�  
	int x;
	int y;
	struct SNAKE* next;
} snake;

//ȫ�ֱ���//  
int score = 0, add = 10;//�ܵ÷���ÿ�γ�ʳ��÷֡�  
int status, sleeptime = 200;//ÿ�����е�ʱ����  
snake* head, * food;//��ͷָ�룬ʳ��ָ��  
snake* q;//�����ߵ�ʱ���õ���ָ��  
int endgamestatus = 0; //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��  
int judge = 1;
//����ȫ������//  
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

void creatMap() { //������ͼ  
	int i;
	for (i = 0; i < LENTH + 2; i += 2) { //��ӡ���±߿�  
		cct_gotoxy(i, 0);
		printf("��");
		cct_gotoxy(i, WIDTH);
		printf("��");
	}
	for (i = 1; i < WIDTH; i++) { //��ӡ���ұ߿�  
		cct_gotoxy(0, i);
		printf("��");
		cct_gotoxy(LENTH, i);
		printf("��");
	}
}

void initsnake() { //��ʼ������  
	snake* tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��//  
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
	while (tail != NULL) { //��ͷ��β���������  
		cct_gotoxy(tail->x, tail->y);
		printf("��");
		tail = tail->next;
	}
}

int biteself() { //�ж��Ƿ�ҧ�����Լ�  
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

void createfood() { //�������ʳ��  
	snake* food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0) {  //��֤��Ϊż����ʹ��ʳ��������ͷ����  
		food_1->x = rand() % (LENTH - 2) + 2;
	}
	food_1->y = rand() % (WIDTH - 2) + 1;
	q = head;
	while (q->next == NULL) {
		if (q->x == food_1->x && q->y == food_1->y) { //�ж������Ƿ���ʳ���غ�  
			free(food_1);
			createfood();
		}
		q = q->next;
	}
	cct_gotoxy(food_1->x, food_1->y);
	food = food_1;
	printf("��");
}

void cantcrosswall() { //���ܴ�ǽ  
	if (head->x == 0 || head->x == LENTH || head->y == 0 || head->y == WIDTH) {
		endgamestatus = 1;
		judge = 0;
	}
}

void snakemove() { //��ǰ��,��U,��D,��L,��R  
	snake* nexthead;
	cantcrosswall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U) {
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y) { //�����һ����ʳ��//  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                                           //���û��ʳ��//  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
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
		if (nexthead->x == food->x && nexthead->y == food->y) { //��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                           //û��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
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
		if (nexthead->x == food->x && nexthead->y == food->y) { //��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                            //û��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
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
		if (nexthead->x == food->x && nexthead->y == food->y) { //��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {                                     //û��ʳ��  
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL) {
				cct_gotoxy(q->x, q->y);
				printf("��");
				q = q->next;
			}
			cct_gotoxy(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (biteself() == 1) {     //�ж��Ƿ��ҧ���Լ�  
		endgamestatus = 2;
		judge = 0;
	}
}


void gamecircle1() { //������Ϸ  

	cct_gotoxy(64, 15);
	printf("��Ҫ��ǽ����Ҫҧ���Լ�\n");
	cct_gotoxy(64, 16);
	printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
	cct_gotoxy(64, 20);
	status = R;
	while (1) {
		cct_gotoxy(64, 10);
		printf("�÷֣�%d  ", score);
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
void gamecircle2() { //������Ϸ  
	score = 0;
	cct_gotoxy(60, 7);
	printf("1.��Ҫ��ǽ����Ҫҧ���Լ�\n");
	cct_gotoxy(60, 8);
	printf("2.����ʹ�������еĲ�ͷ���������ǿ������Ƿ���á�\n");
	cct_gotoxy(60, 9);
	printf("3.���¼��ϲ������·�������Ь�ӡ�\n");
	cct_gotoxy(60, 10);
	printf("4.���¼��ϲ������·����������֡�\n");
	cct_gotoxy(60, 11);
	printf("5.�ʼǱ������Ǵ��ʵ������ʼǱ������Ǵ��ʵ������ʼǱ������Ǵ��ʵ�����\n");
	cct_gotoxy(60, 12);
	printf("6.��Ҫʹ�ô��ʵ�����\n");
	cct_gotoxy(60, 13);
	printf("7.�������������ǡ��еġ������ˡ�ʱ����ҪΥ����6������\n");
	cct_gotoxy(60, 14);
	printf("8.�ش����ǵ����⡣\n");

	cct_gotoxy(60, 16);
	printf("�á�.��.��.���ֱ�����ƶ������ƶ����ƿ��ƿ��ƿ��ƿ��ƿ���\n���ƿ��ƿ��ƿ��ƿ��ƿ���\n���ƿ���    ���ƿ��ƿ��ƿ��ƿ��ƿ� �ƿ� �ƿ��ƿ��ƿ���\n\n\n      ���ƿ��ƿ���");

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
		printf("�÷֣�%d  ", 99999999-score);
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
void gamecircle3() { //������Ϸ  

	const int MAX_X = 54, MAX_Y = 26;
	int x, y, fg_color, bg_color, num;
	char ch;

	srand((unsigned int)(time(0))); //��������

	for (num = 0; num < 150; num++) {
		x = rand() % (MAX_X + 1);
		y = rand() % (MAX_Y + 1);
		fg_color = rand() % 16;
		bg_color = rand() % 16;
		ch = 33 + rand() % 94;   //ASCII�����33-126Ϊ�ɼ�ͼ���ַ�

		/* ��ʾһ��1 */
		cct_showch(x, y, ch, bg_color, fg_color, 2);

		/* ��ʱ0.05�� */
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
				cct_showstr(j, i, "�÷֣�0", bg_color, fg_color, 8);
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

void welcometogame1() { //��ʼ����  
	cct_gotoxy(15, 10);
	printf("===================================");
	cct_gotoxy(15, 11);
	printf("��ӭ�������̰ʳ����Ϸ��");
	cct_gotoxy(15, 12);
	printf("�á�.��.��.���ֱ�����ߵ��ƶ�\n");
	cct_gotoxy(15, 13);
	printf("�ԵĶ����Ӯ�ң�");
	cct_gotoxy(15, 14);
	printf("===================================\n\n");
	system("pause");
	system("cls");
}
void welcometogame2() { //��ʼ����  
	cct_gotoxy(15, 10);
	printf("===========   = ========= =     ========   ===============  == == ==========   =============================");
	cct_gotoxy(15, 11);
	printf("��#$ӭ%#)($#̰%ʳ��:>::lig(&yuhb\n");
	cct_gotoxy(15, 12);
	printf("�ƶ����� ������S  ��F�ƶ��ƶ�S  ��SD�� SDFDG+^732��SAS���ƶ��ƶ� ���ƶ�����  ���ƶ�\n");
	cct_gotoxy(15, 13);
	printf("Ӯ�ң�BUYAObuy��Ҫ�䲻���� &��&��GS*&���� ����������������������  ��������������");
	cct_gotoxy(15, 14);
	printf("===   ======== ===========  ==============    ============    =================    ==== = =====  ====================\n\n");
	system("pause");  
	system("cls");
}

void endgame1() { //������Ϸ  


	system("cls");
	cct_gotoxy(24, 12);
	if (endgamestatus == 1) {
		printf("�Բ�����ײ��ǽ�ˡ���Ϸ����.");
	}
	else if (endgamestatus == 2) {
		printf("�Բ�����ҧ���Լ��ˡ���Ϸ����.");
	}
	cct_gotoxy(24, 13);
	printf("���ĵ÷���%d,���ѵ���������һ��������y�����ɣ�\n", score);
	std::cin.clear(); //�������״̬
	std::cin.ignore(100, '\n'); //�������뻺�����е������ַ���ֱ���������з�
	while (true) {
		if (_kbhit()) {  // ����м�������
			int key_code = _getch();  // ��ȡ����
			if (key_code == 'y' || key_code == 'Y') {
				break;  // �������
			}
			else {
				cout << "����y����y����y����y����y����y����y����y����y����y����y����y����y����y����y����y����y����y����y" << endl;
			}
		}
	}
}
void endgame2() { //������Ϸ  

	system("cls");
	cct_gotoxy(7, 6);
	cout << "LOSTLOSTLOSTLOS TLOSTL  OSTLOSTLOSTLOSTLOSTLO  STLOSTLOSTLOSTLOSTLOSTLO  STLOSTLOSTLOSTL  OSTLOSTLOSTL OSTLOSTL   OSTLOSTLOSTLOSTLOSTLOS     TLOSTLOSTLOSTLOSTLOST\n      LOSTLOSTLOSTLOSTLOST LOST LOSTL  OSTLO STLOSTLOSTLOST\n                           LOSTLOSTLOSTLOSTLOSTLOST LO S   TL   OSTLOSTLOSTLOSTLOST\nL        OSTL                  OSTL OSTLOSTLOSTLOSTLOSTLOST                   LOSTLOSTLOSTLOST";
	cct_gotoxy(24, 12);
	
	if (endgamestatus == 1) {
		printf("�Բ���ΪʲôΪʲô��ײ��ǽ��ΪʲôΪʲôΪʲô��Ϊʲô��ϷΪʲô����.");
	}
	else if (endgamestatus == 2) {
		printf("�Բ���ΪʲôΪʲô��ҧ���Լ���ΪʲôΪʲôΪʲô��Ϊʲô��ϷΪʲô����.");
	}
	cct_gotoxy(14, 13);
	printf("����һ������һ������һ������һ������һ������һ������һ������һ������һ��   ����һ������һ������һ��\n����һ������һ��   ����һ������һ��   ����һ������һ������һ������һ��\n����һ����   ��һ������һ������һ������     һ������һ������һ  ������һ��\n����   һ��  ����һ������һ������һ������һ������һ��   ����һ������һ������y��   �� ����  y�� ��  ���� y����,\n����y  ��������y  ��������y����   ����y����\n����y��������y   ��������y����     ����y���� ����y��������y��������y����\n\n\n");
	std::cin.clear(); //�������״̬
	std::cin.ignore(100, '\n'); //�������뻺�����е������ַ���ֱ���������з�
	while (true) {
		if (_kbhit()) {  // ����м�������
			int key_code = _getch();  // ��ȡ����
			if (key_code == 'y'||key_code=='Y') {
				break;  // �������
			}
			else {
				cout << " ��y is y ��yis yis y��yi��ys ��y yis yv ��y is yis yis yis yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy" << endl;
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
			cct_showstr(j, i, " ���������� ", bg_color, fg_color, 8);
			Sleep(10);
		}
	}
}

void gamestart1() { //��Ϸ��ʼ��  

	welcometogame1();
	creatMap();
	initsnake();
	createfood();
}
void gamestart2() { //��Ϸ��ʼ��  

	welcometogame2();
	creatMap();
	initsnake();
	createfood();
}
void gamestart3() { //��Ϸ��ʼ��  

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