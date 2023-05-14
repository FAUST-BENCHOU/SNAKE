#undef UNICODE
#undef _UNICODE		
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "cmd_console_tools.h"
#pragma warning(disable:4996)	//���ð�ȫ��� _tcscpy��_stprintf


/*--------------------------------�� ��---------------------------*/
#define WIDTH_2 1170		/*	���ڴ�С  WIDTH * HIGHT ���� �ߴ���Ϊ SIZE ����������  	 */
#define HIGHT 640	/*	�ߵ��ж��� A(SIZE,SIZE)�� B(WIDTH-FORM-SIZE,HIGHT-SIZE)֮��ľ�����*/
#define FORM 250		/*	�������ֱ߿���	*/
#define SIZE 40			/*	ÿ���ߵĳߴ�		*/

#define COLOUR RGB(rand()%255,rand()%255,rand()%255)	/*	��ɫ */

// ö��ֵ


// ȫ�ֱ���
static int g_speed = 300;			//	�ٶ�(����ˢ�µ�ʱ����)
static int g_status = 0;			//	��Ϸ������־

// ȫ�ֳ���
const char* IMAGE_PATH = "image";	//	��ͼ��Դ����·��
const int SNAKE_LENGTH = 3;			//	��ʼ��Ĭ�ϳ�ʼ����
const int SNAKE_TOWARD = 77;		//	��ʼ��Ĭ����ͷ����
const int MAX_SIZE = 100;			




/*---------------------------��Դ-------------------------------------*/
//��
typedef struct _snake
{
	int x[MAX_SIZE];		//���ꡪX
	int y[MAX_SIZE];		//���ꡪY
	int len;				//��ǰ����
	int count;				//��ǰ����
	int direction;			//��ͷ����
}Snake;

//ʳ��
typedef struct _food
{
	int x;					//���ꡪX
	int y;					//���ꡪY
	int flg;				//���ʳ���Ƿ��Ѿ�����
}Food;

// ͼƬ����
IMAGE img[11];		//ǽ����ͷ�����壬��β��ʳ��
IMAGE tips[13];		//��������ͼƬ

// ���ͼƬ������
char name[11][15] = { "wall","snakeHead","body","food","food2","food3","angel","evil","cloud","fire","wall2" };
char tipsName[13][15] = { "snake","label","gameover","start","snake2","label2","anger","bad","sad","eye","eye2","scare","black"};

//����ͼƬ
void loadResources()
{
	for (int i = 0; i < 11; i++)
	{
		char filename[30] = "";
		sprintf_s(filename, "%s/%s.png", IMAGE_PATH, name[i]);
		loadimage(img + i, filename);
	}
	for (int i = 0; i < 13; i++)
	{
		char filename[30] = "";
		sprintf_s(filename, "%s/%s.png", IMAGE_PATH, tipsName[i]);
		loadimage(tips + i, filename);
	}
}

/************************************************************/
//����
Snake snake_2;		//��
Food  food_2;			//ʳ��
//����
void PrintForm();		//������ʾ��
void PrintForm_S();
void Init_Snake();		//��ʼ����
void Init_Food();		//��ʼ��ʳ��
void ShowSnake();		//��ͼ����
void ShowSnake_S();		//ħ��
void ShowFood();		//��ͼ��ʳ��
void ShowFood_S();//��ͼ��ʳ��(ħ��)
void MoveSnake();		//�ߵ��ƶ�
void KeyDown();			//����
void EatFood();			//��ʳ��
void GameOver();		//��Ϸ����
int EndScreen();		//��������
void DrawWall();		//��ǽ
void DrawWall_S();



//�Ҳ�չʾ����
void PrintForm()
{
	fillrectangle(0, 0, WIDTH_2, HIGHT - 1);	//�߿�

	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 30;                      // ��������߶�Ϊ 48
	_tcscpy(f.lfFaceName, _T("��Բ"));    // ��������Ϊ�����塱
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f);                     // ����������ʽ

	putimage(WIDTH_2 - FORM, 0, &tips[0]);	//��ͼ������̰ͨ����
	putimage(WIDTH_2 - FORM, 250, &tips[1]);	//��ʾ����ͼ�������ֲ�
	setbkmode(TRANSPARENT);					//����͸����ʽ
	settextcolor(LIGHTBLUE);				//������ɫ��
	TCHAR  str[2][50];
	_stprintf(str[0], _T("  �÷�: %3d ��"), snake_2.count);
	_stprintf(str[1], _T("  ����: %3d ��"), snake_2.len);
	for (int i = 0; i < 2; ++i)
	{
		outtextxy(WIDTH_2 - FORM + 10, i * 40 + 250, str[i]);	//ָ��λ������ַ���
	}

	TCHAR  str2[5][50];
	_stprintf(str2[0], _T(" ��ʾ:"));
	_stprintf(str2[1], _T("  һö��ʵ10�֣����  "));
	_stprintf(str2[2], _T("  100�ڣ�Ϊͨ����? ײǽ,"));
	_stprintf(str2[3], _T("  ��ײ���Լ���Ϸʧ��"));
	_stprintf(str2[4], _T("      "));

	settextcolor(BLACK);				//������ɫ
	f.lfHeight = 20;					// ��������߶�Ϊ 20
	settextstyle(&f);					// ����������ʽ
	for (int i = 0; i < 5; i++)
	{
		outtextxy(WIDTH_2 - FORM + 10, 400 + i * 40, str2[i]);	//ָ��λ������ַ���
	}

}
void PrintForm_S()
{
	fillrectangle(0, 0, WIDTH_2, HIGHT - 1);	//�߿�

	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 30;                      // ��������߶�Ϊ 48
	_tcscpy(f.lfFaceName, _T("��Բ"));    // ��������Ϊ�����塱
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f);                     // ����������ʽ

	putimage(WIDTH_2 - FORM, 0, &tips[4]);	//��ͼ������̰ͨ����
	putimage(WIDTH_2 - FORM, 250, &tips[5]);	//��ʾ����ͼ�������ֲ�
	setbkmode(TRANSPARENT);					//����͸����ʽ
	settextcolor(LIGHTBLUE);				//������ɫ��

	TCHAR  str2[3][50];
	_stprintf(str2[0], _T(" ��ʾ:"));
	_stprintf(str2[1], _T("  ��ʲô  "));
	_stprintf(str2[2], _T("  Ҳ�ò���  "));



	for (int i = 0; i < 3; i++)
	{
		outtextxy(WIDTH_2 - FORM + 30, 400 + i * 20, str2[i]);	//ָ��λ������ַ���
	}

}

void two_half_dimential_snake()
{
	char key;
	while (_kbhit()) { // ����Ƿ���ڰ����������е��ַ�
		key = _getch(); // ��ȡ��ǰ������ַ�
	}

	initgraph(WIDTH_2, HIGHT);	//��ʼ�����ڴ�С
	loadResources();			//����ͼƬ

	setlinecolor(BLACK);		//������ɫ
	setfillcolor(WHITE);		//�����ɫ
	fillrectangle(-1, -1, WIDTH_2, HIGHT);	//�߿�

	putimage(0, 0, &tips[3]);
	Sleep(100);

	_getch();					// �����������ʼ

	Init_Snake();				//��ʼ����
	Init_Food();				//��ʼ��ʳ��
	int tmp_count = 0;
	while (true)//ǿ��
	{

		if (food_2.flg == 0)
		{
			Init_Food();		//��ʼ��ʳ��
		}

		BeginBatchDraw();
		cleardevice();			//ˢ�´���

		if (snake_2.count < 20) {
			PrintForm();//��ӡ���
			DrawWall();				//��ǽ
			ShowFood();	//��ʳ��
			ShowSnake();
		}
		else {
			g_speed = 150;
			PrintForm_S();
			DrawWall_S();
			for (int i = 0; i < tmp_count; i++) {
				int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//Ԥ�� FORM ������ʾ��Ϣ
				int y = (HIGHT - SIZE) / SIZE;
				food_2.x = rand() % x * SIZE;		// *SIZE ��֤ʳ���ַΪ����
				food_2.y = rand() % y * SIZE;
				ShowFood_S();	//������ʳ��
			}
			ShowSnake_S();
			tmp_count++;

		}

		//����
		EndBatchDraw();

		if (_kbhit())
		{
			KeyDown();			//��ҿ������ƶ�
		}

		MoveSnake();			//���ƶ�
		EatFood();				//��ʳ��

		GameOver();				//�����Ϸ�Ƿ���������ý�����־λ
		if (EndScreen() == 1) {
			break;
		}
		if (tmp_count == 40) {
			g_status = 1;
			EndScreen();
			break;
		}
		Sleep(g_speed);			//�����ٶ�
	}
	system("pause");
	initgraph(1500, 850);	//��ʼ�����ڴ�С
	HWND hwnd = GetForegroundWindow(); // ��ȡ��ǰ���ھ��
	int x = 0; // ���ϽǺ�����
	int y = 0; // ���Ͻ�������
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* ��ȡ������Ϣ */
	/* ���ô�����Ϣ ��� ȡ�����������߿� */
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	MoveWindow(hwnd, x, y, 1500, 850, TRUE); // �ƶ����ڵ����Ͻ�

	for (int i = 6; i < 12; i++) {
		putimage(0, 0, &tips[12]);
		if (i == 9 || i == 10){
			putimage(0, 150, &tips[i]);
		}
		else {
			putimage(0, 0, &tips[i]);
		}
		Sleep(450);
	}
	
	Sleep(500);
	closegraph();
	Sleep(500);
	return;
}
/*--------------------------�ߵĹ��ܺ���--------------------------*/

//��ʼ����
void Init_Snake()
{
	//��ʼ��ǰ������
	snake_2.x[0] = SIZE * 3;
	snake_2.y[0] = SIZE;

	snake_2.x[1] = SIZE * 2;
	snake_2.y[1] = SIZE;

	snake_2.x[2] = SIZE;
	snake_2.y[2] = SIZE;

	/*	˵��:
	*	1. ������Ĭ������Ϊ 0��
	*		�����ʼ���� SNAKE_LENGTH ����Ϊ������Ҳ�����ʼ����������
	*	2. ��Ϊǰ������λ�ù̶���
	*		��ˣ���Ϸ��ʼʱ��S NAKE_TOWARD ��ͷ����ֻ�����һ����¡�
	*		��������趨������ͷ��ʼ������Ҫ�޸�ǰ SNAKE_TOWARD ���ߵ����ꡣ
	*/

	snake_2.count = 0;				// ��ʼ������
	snake_2.len = SNAKE_LENGTH;		// ��ʼ������
	snake_2.direction = SNAKE_TOWARD;	// ��ʼ����ͷ����
}

//��ʼ��ʳ��
void Init_Food()
{
	srand((unsigned int)time(NULL));		//�������
	int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//Ԥ�� FORM ������ʾ��Ϣ
	int y = (HIGHT - SIZE) / SIZE;


	//���ʳ���Ƿ���������
	int i = 0;
	while (i < snake_2.len)
	{
		if (food_2.x == snake_2.x[i] && food_2.y == snake_2.y[i])	//ʳ����������
		{
			food_2.x = rand() % x * SIZE;		// *SIZE ��֤ʳ���ַΪ����
			food_2.y = rand() % y * SIZE;
		}
		if (food_2.x == 0 || food_2.y == 0 || food_2.x == SIZE || food_2.y == SIZE || food_2.x == WIDTH_2 - FORM - SIZE * 2 || food_2.y == HIGHT - SIZE * 2)		//ʳ�����ϱ߽����߽�
		{
			food_2.x = rand() % x * SIZE;
			food_2.y = rand() % y * SIZE;
		}

		++i;
	}
	food_2.flg = 1;
}

//��ͼ����
void ShowSnake()
{
	/*	������	*/
	int i;
	for (i = 1; i < snake_2.len; ++i)	//������β������Ϊ i < snake.len - 1
	{
		putimage(snake_2.x[i], snake_2.y[i], &img[2]);
	}

	/*	����ͷ	*/
	putimage(snake_2.x[0], snake_2.y[0], &img[1]);

}
int change_S = 0;
void ShowSnake_S()
{
	/*	������	*/
	int i;
	for (i = 1; i < snake_2.len; ++i)	//������β������Ϊ i < snake.len - 1
	{
		if (change_S % 2 == 0) {
			putimage(snake_2.x[i], snake_2.y[i], &img[8]);
		}
		else {
			putimage(snake_2.x[i], snake_2.y[i], &img[9]);
		}

	}

	/*	����ͷ	*/
	if (change_S % 2 == 0) {
		putimage(snake_2.x[0], snake_2.y[0], &img[6]);
	}
	else {
		putimage(snake_2.x[0], snake_2.y[0], &img[7]);
	}
	change_S++;
}

//��ͼ��ʳ��
void ShowFood()
{
	putimage(food_2.x, food_2.y, &img[3]);
}
void ShowFood_S()
{
	putimage(food_2.x, food_2.y, &img[3]);
	int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//Ԥ�� FORM ������ʾ��Ϣ
	int y = (HIGHT - SIZE) / SIZE;
	food_2.x = rand() % x * SIZE;		// *SIZE ��֤ʳ���ַΪ����
	food_2.y = rand() % y * SIZE;
	putimage(food_2.x, food_2.y, &img[4]);
	food_2.x = rand() % x * SIZE;		// *SIZE ��֤ʳ���ַΪ����
	food_2.y = rand() % y * SIZE;
	putimage(food_2.x, food_2.y, &img[5]);
}


//�ߵ��ƶ�
void MoveSnake()
{
	//���ߵĺ�һ�������ƶ���ǰһ�ڵ�����λ��
	for (int i = snake_2.len; i > 0; --i)
	{
		snake_2.x[i] = snake_2.x[i - 1];
		snake_2.y[i] = snake_2.y[i - 1];
	}
	//�����ƶ���ͷ�������ߵķ����ƶ�
	//enum Direction { right = 77, left = 75, down = 80, up = 72 };	//ö�ٷ���ֵ

	switch (snake_2.direction)
	{
	case 77:
		snake_2.x[0] += SIZE;
		break;
	case 75:
		snake_2.x[0] -= SIZE;
		break;
	case 72:
		snake_2.y[0] -= SIZE;
		break;
	case 80:
		snake_2.y[0] += SIZE;
		break;
	default:
		break;
	}
}

// ��������
void KeyDown()
{
	// ��ͣ���˳�
	char tmp = _getch();


	if (tmp == -32 || tmp == 0)
	{
		// ���������Ȱ�����F1��F2�Ȱ�������������ֵ
		// �ڶ�������ʵ��ֵ���������� key = _getch(); ����
	}
	else
	{
		return;
	}

	// �������ң����ټ���
	char key = _getch();
	switch (key)
	{//enum Direction { right = 77, left = 75, down = 80, up = 72 };	//ö�ٷ���ֵ

	case 77:
		if (snake_2.direction != 75)	//�����෴�����ƶ�
			snake_2.direction = 77;
		break;
	case 75:
		if (snake_2.direction != 77)
			snake_2.direction = 75;
		break;
	case 72:
		if (snake_2.direction != 80)
			snake_2.direction = 72;
		break;
	case 80:
		if (snake_2.direction != 72)
			snake_2.direction = 80;
		break;
	default:
		break;

	}

}

//��ʳ��
void EatFood()
{
	if (food_2.x == snake_2.x[0] && food_2.y == snake_2.y[0])
	{
		++snake_2.len;
		snake_2.count += 10;	//һ��ʳ��ʮ��
		food_2.flg = 0;
	}

}

//��Ϸ����
void GameOver()
{

	//ײǽ
	if (snake_2.x[0] < SIZE || snake_2.x[0] >= WIDTH_2 - FORM - SIZE ||		//���������
		snake_2.y[0]<SIZE || snake_2.y[0]>HIGHT - SIZE * 2)		//���������
	{
		g_status = 2;
		return;
	}

	for (int i = 1; i < snake_2.len; i++)
	{
		//ײ�Լ�
		if (snake_2.x[i] == snake_2.x[0] && snake_2.y[i] == snake_2.y[0])
		{
			g_status = 3;
			return;
		}
	}

}

//��������
int EndScreen()
{
	if (g_status == 0) // ��Ϸû����
	{
		return 0;
	}

	TCHAR s[100] = "", s2[50] = "";
	if (g_status == 1) {
		_stprintf(s, _T("GameOver!\n���ķ���Ϊ%d��"), snake_2.count);
	}
	else if (g_status == 2)
	{
		_stprintf(s, _T("��ײǽ�� GameOver!\n���ķ���Ϊ%d��"), snake_2.count);
	}
	else if (g_status == 3)
	{
		_stprintf(s, _T("ҧ���Լ��� GameOver!\n���ķ���Ϊ%d��"), snake_2.count);
	}
	_stprintf(s2, _T("       �����������....\n"));

	ShowSnake_S();			//���ߣ���ʾ����״̬
	setbkmode(TRANSPARENT);		//����͸����ʽ
	settextcolor(LIGHTBLUE);	//������ɫ

	// �������ַ�����ʾ
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2, s);		//ָ��λ������ַ���
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2 + SIZE, s2);	//ָ��λ������ַ���

	// ����ͼƬչʾ
	Sleep(1000);
	system("pause");
	putimage(0, 0, &tips[2]);		//��ͼ����������
	settextcolor(RED);				//������ɫ��
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2 + SIZE / 2, s);	//ָ��λ������ַ���
	return 1;

}

// ��ǽ
void  DrawWall()
{
	for (int i = 0; i < WIDTH_2 - FORM; i += SIZE)
	{
		putimage(i, 0, &img[0]);
	}
	for (int i = 0; i < WIDTH_2 - FORM; i += SIZE)
	{
		putimage(i, HIGHT - SIZE, &img[0]);
	}
	for (int j = 0; j < WIDTH_2 - FORM; j += SIZE)
	{
		putimage(0, j, &img[0]);
	}
	for (int j = 0; j < WIDTH_2 - FORM; j += SIZE)
	{
		putimage(WIDTH_2 - FORM - SIZE, j, &img[0]);
	}
}
void  DrawWall_S()
{
	for (int i = 0; i < WIDTH_2 - FORM; i += SIZE)
	{
		putimage(i, 0, &img[10]);
	}
	for (int i = 0; i < WIDTH_2 - FORM; i += SIZE)
	{
		putimage(i, HIGHT - SIZE, &img[10]);
	}
	for (int j = 0; j < WIDTH_2 - FORM; j += SIZE)
	{
		putimage(0, j, &img[10]);
	}
	for (int j = 0; j < WIDTH_2 - FORM; j += SIZE)
	{
		putimage(WIDTH_2 - FORM - SIZE, j, &img[10]);
	}
}
