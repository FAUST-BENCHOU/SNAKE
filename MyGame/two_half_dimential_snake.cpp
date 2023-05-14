#undef UNICODE
#undef _UNICODE		
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "cmd_console_tools.h"
#pragma warning(disable:4996)	//禁用安全检查 _tcscpy、_stprintf


/*--------------------------------属 性---------------------------*/
#define WIDTH_2 1170		/*	窗口大小  WIDTH * HIGHT 像素 尺寸需为 SIZE 的整数倍，  	 */
#define HIGHT 640	/*	蛇的行动在 A(SIZE,SIZE)到 B(WIDTH-FORM-SIZE,HIGHT-SIZE)之间的矩形内*/
#define FORM 250		/*	设置文字边框宽度	*/
#define SIZE 40			/*	每节蛇的尺寸		*/

#define COLOUR RGB(rand()%255,rand()%255,rand()%255)	/*	彩色 */

// 枚举值


// 全局变量
static int g_speed = 300;			//	速度(控制刷新的时间间隔)
static int g_status = 0;			//	游戏结束标志

// 全局常量
const char* IMAGE_PATH = "image";	//	贴图资源所在路径
const int SNAKE_LENGTH = 3;			//	初始，默认初始长度
const int SNAKE_TOWARD = 77;		//	初始，默认蛇头方向
const int MAX_SIZE = 100;			




/*---------------------------资源-------------------------------------*/
//蛇
typedef struct _snake
{
	int x[MAX_SIZE];		//坐标—X
	int y[MAX_SIZE];		//坐标—Y
	int len;				//当前长度
	int count;				//当前分数
	int direction;			//蛇头方向
}Snake;

//食物
typedef struct _food
{
	int x;					//坐标—X
	int y;					//坐标—Y
	int flg;				//标记食物是否已经出现
}Food;

// 图片对象
IMAGE img[11];		//墙，蛇头，身体，蛇尾，食物
IMAGE tips[13];		//其他界面图片

// 标记图片变量名
char name[11][15] = { "wall","snakeHead","body","food","food2","food3","angel","evil","cloud","fire","wall2" };
char tipsName[13][15] = { "snake","label","gameover","start","snake2","label2","anger","bad","sad","eye","eye2","scare","black"};

//加载图片
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
//变量
Snake snake_2;		//蛇
Food  food_2;			//食物
//函数
void PrintForm();		//文字提示框
void PrintForm_S();
void Init_Snake();		//初始化蛇
void Init_Food();		//初始化食物
void ShowSnake();		//贴图画蛇
void ShowSnake_S();		//魔化
void ShowFood();		//贴图画食物
void ShowFood_S();//贴图画食物(魔化)
void MoveSnake();		//蛇的移动
void KeyDown();			//控制
void EatFood();			//吃食物
void GameOver();		//游戏结束
int EndScreen();		//结束画面
void DrawWall();		//画墙
void DrawWall_S();



//右侧展示区域
void PrintForm()
{
	fillrectangle(0, 0, WIDTH_2, HIGHT - 1);	//边框

	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度为 48
	_tcscpy(f.lfFaceName, _T("幼圆"));    // 设置字体为“黑体”
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式

	putimage(WIDTH_2 - FORM, 0, &tips[0]);	//贴图——卡通贪吃蛇
	putimage(WIDTH_2 - FORM, 250, &tips[1]);	//提示框贴图，操作手册
	setbkmode(TRANSPARENT);					//文字透明方式
	settextcolor(LIGHTBLUE);				//文字颜色蓝
	TCHAR  str[2][50];
	_stprintf(str[0], _T("  得分: %3d 分"), snake_2.count);
	_stprintf(str[1], _T("  长度: %3d 节"), snake_2.len);
	for (int i = 0; i < 2; ++i)
	{
		outtextxy(WIDTH_2 - FORM + 10, i * 40 + 250, str[i]);	//指定位置输出字符串
	}

	TCHAR  str2[5][50];
	_stprintf(str2[0], _T(" 提示:"));
	_stprintf(str2[1], _T("  一枚果实10分，蛇最长  "));
	_stprintf(str2[2], _T("  100节，为通关吗? 撞墙,"));
	_stprintf(str2[3], _T("  或撞到自己游戏失败"));
	_stprintf(str2[4], _T("      "));

	settextcolor(BLACK);				//文字颜色
	f.lfHeight = 20;					// 设置字体高度为 20
	settextstyle(&f);					// 设置字体样式
	for (int i = 0; i < 5; i++)
	{
		outtextxy(WIDTH_2 - FORM + 10, 400 + i * 40, str2[i]);	//指定位置输出字符串
	}

}
void PrintForm_S()
{
	fillrectangle(0, 0, WIDTH_2, HIGHT - 1);	//边框

	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度为 48
	_tcscpy(f.lfFaceName, _T("幼圆"));    // 设置字体为“黑体”
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式

	putimage(WIDTH_2 - FORM, 0, &tips[4]);	//贴图——卡通贪吃蛇
	putimage(WIDTH_2 - FORM, 250, &tips[5]);	//提示框贴图，操作手册
	setbkmode(TRANSPARENT);					//文字透明方式
	settextcolor(LIGHTBLUE);				//文字颜色蓝

	TCHAR  str2[3][50];
	_stprintf(str2[0], _T(" 提示:"));
	_stprintf(str2[1], _T("  你什么  "));
	_stprintf(str2[2], _T("  也得不到  "));



	for (int i = 0; i < 3; i++)
	{
		outtextxy(WIDTH_2 - FORM + 30, 400 + i * 20, str2[i]);	//指定位置输出字符串
	}

}

void two_half_dimential_snake()
{
	char key;
	while (_kbhit()) { // 检查是否存在按键缓冲区中的字符
		key = _getch(); // 读取当前键入的字符
	}

	initgraph(WIDTH_2, HIGHT);	//初始化窗口大小
	loadResources();			//加载图片

	setlinecolor(BLACK);		//画线颜色
	setfillcolor(WHITE);		//填充颜色
	fillrectangle(-1, -1, WIDTH_2, HIGHT);	//边框

	putimage(0, 0, &tips[3]);
	Sleep(100);

	_getch();					// 按下任意键开始

	Init_Snake();				//初始化蛇
	Init_Food();				//初始化食物
	int tmp_count = 0;
	while (true)//强退
	{

		if (food_2.flg == 0)
		{
			Init_Food();		//初始化食物
		}

		BeginBatchDraw();
		cleardevice();			//刷新窗口

		if (snake_2.count < 20) {
			PrintForm();//打印表框
			DrawWall();				//画墙
			ShowFood();	//画食物
			ShowSnake();
		}
		else {
			g_speed = 150;
			PrintForm_S();
			DrawWall_S();
			for (int i = 0; i < tmp_count; i++) {
				int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//预留 FORM 像素显示信息
				int y = (HIGHT - SIZE) / SIZE;
				food_2.x = rand() % x * SIZE;		// *SIZE 保证食物地址为整数
				food_2.y = rand() % y * SIZE;
				ShowFood_S();	//画多种食物
			}
			ShowSnake_S();
			tmp_count++;

		}

		//画蛇
		EndBatchDraw();

		if (_kbhit())
		{
			KeyDown();			//玩家控制蛇移动
		}

		MoveSnake();			//蛇移动
		EatFood();				//吃食物

		GameOver();				//检查游戏是否结束，设置结束标志位
		if (EndScreen() == 1) {
			break;
		}
		if (tmp_count == 40) {
			g_status = 1;
			EndScreen();
			break;
		}
		Sleep(g_speed);			//控制速度
	}
	system("pause");
	initgraph(1500, 850);	//初始化窗口大小
	HWND hwnd = GetForegroundWindow(); // 获取当前窗口句柄
	int x = 0; // 左上角横坐标
	int y = 0; // 左上角纵坐标
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* 获取窗口信息 */
	/* 设置窗口信息 最大化 取消标题栏及边框 */
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	MoveWindow(hwnd, x, y, 1500, 850, TRUE); // 移动窗口到左上角

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
/*--------------------------蛇的功能函数--------------------------*/

//初始化蛇
void Init_Snake()
{
	//初始化前三节蛇
	snake_2.x[0] = SIZE * 3;
	snake_2.y[0] = SIZE;

	snake_2.x[1] = SIZE * 2;
	snake_2.y[1] = SIZE;

	snake_2.x[2] = SIZE;
	snake_2.y[2] = SIZE;

	/*	说明:
	*	1. 其余蛇默认坐标为 0。
	*		如果初始长度 SNAKE_LENGTH 设置为更长，也无需初始化其他坐标
	*	2. 因为前三节蛇位置固定。
	*		因此，游戏开始时的S NAKE_TOWARD 蛇头方向，只能向右或向下。
	*		如果，向设定其他蛇头初始方向，需要修改前 SNAKE_TOWARD 节蛇的坐标。
	*/

	snake_2.count = 0;				// 初始化分数
	snake_2.len = SNAKE_LENGTH;		// 初始化长度
	snake_2.direction = SNAKE_TOWARD;	// 初始化蛇头方向
}

//初始化食物
void Init_Food()
{
	srand((unsigned int)time(NULL));		//随机种子
	int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//预留 FORM 像素显示信息
	int y = (HIGHT - SIZE) / SIZE;


	//检查食物是否在蛇身上
	int i = 0;
	while (i < snake_2.len)
	{
		if (food_2.x == snake_2.x[i] && food_2.y == snake_2.y[i])	//食物在蛇身上
		{
			food_2.x = rand() % x * SIZE;		// *SIZE 保证食物地址为整数
			food_2.y = rand() % y * SIZE;
		}
		if (food_2.x == 0 || food_2.y == 0 || food_2.x == SIZE || food_2.y == SIZE || food_2.x == WIDTH_2 - FORM - SIZE * 2 || food_2.y == HIGHT - SIZE * 2)		//食物在上边界或左边界
		{
			food_2.x = rand() % x * SIZE;
			food_2.y = rand() % y * SIZE;
		}

		++i;
	}
	food_2.flg = 1;
}

//贴图画蛇
void ShowSnake()
{
	/*	画蛇身	*/
	int i;
	for (i = 1; i < snake_2.len; ++i)	//若画蛇尾，条件为 i < snake.len - 1
	{
		putimage(snake_2.x[i], snake_2.y[i], &img[2]);
	}

	/*	画蛇头	*/
	putimage(snake_2.x[0], snake_2.y[0], &img[1]);

}
int change_S = 0;
void ShowSnake_S()
{
	/*	画蛇身	*/
	int i;
	for (i = 1; i < snake_2.len; ++i)	//若画蛇尾，条件为 i < snake.len - 1
	{
		if (change_S % 2 == 0) {
			putimage(snake_2.x[i], snake_2.y[i], &img[8]);
		}
		else {
			putimage(snake_2.x[i], snake_2.y[i], &img[9]);
		}

	}

	/*	画蛇头	*/
	if (change_S % 2 == 0) {
		putimage(snake_2.x[0], snake_2.y[0], &img[6]);
	}
	else {
		putimage(snake_2.x[0], snake_2.y[0], &img[7]);
	}
	change_S++;
}

//贴图画食物
void ShowFood()
{
	putimage(food_2.x, food_2.y, &img[3]);
}
void ShowFood_S()
{
	putimage(food_2.x, food_2.y, &img[3]);
	int x = (WIDTH_2 - FORM - SIZE) / SIZE;	//预留 FORM 像素显示信息
	int y = (HIGHT - SIZE) / SIZE;
	food_2.x = rand() % x * SIZE;		// *SIZE 保证食物地址为整数
	food_2.y = rand() % y * SIZE;
	putimage(food_2.x, food_2.y, &img[4]);
	food_2.x = rand() % x * SIZE;		// *SIZE 保证食物地址为整数
	food_2.y = rand() % y * SIZE;
	putimage(food_2.x, food_2.y, &img[5]);
}


//蛇的移动
void MoveSnake()
{
	//把蛇的后一节坐标移动到前一节的坐标位置
	for (int i = snake_2.len; i > 0; --i)
	{
		snake_2.x[i] = snake_2.x[i - 1];
		snake_2.y[i] = snake_2.y[i - 1];
	}
	//单独移动蛇头，根据蛇的方向移动
	//enum Direction { right = 77, left = 75, down = 80, up = 72 };	//枚举方向值

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

// 按键处理
void KeyDown()
{
	// 暂停，退出
	char tmp = _getch();


	if (tmp == -32 || tmp == 0)
	{
		// ↑↓←→等按键，F1、F2等按键，有两个键值
		// 第二个是真实键值，在下面用 key = _getch(); 接收
	}
	else
	{
		return;
	}

	// 上下左右，加速减速
	char key = _getch();
	switch (key)
	{//enum Direction { right = 77, left = 75, down = 80, up = 72 };	//枚举方向值

	case 77:
		if (snake_2.direction != 75)	//不能相反方向移动
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

//吃食物
void EatFood()
{
	if (food_2.x == snake_2.x[0] && food_2.y == snake_2.y[0])
	{
		++snake_2.len;
		snake_2.count += 10;	//一个食物十分
		food_2.flg = 0;
	}

}

//游戏结束
void GameOver()
{

	//撞墙
	if (snake_2.x[0] < SIZE || snake_2.x[0] >= WIDTH_2 - FORM - SIZE ||		//横坐标出界
		snake_2.y[0]<SIZE || snake_2.y[0]>HIGHT - SIZE * 2)		//纵坐标出界
	{
		g_status = 2;
		return;
	}

	for (int i = 1; i < snake_2.len; i++)
	{
		//撞自己
		if (snake_2.x[i] == snake_2.x[0] && snake_2.y[i] == snake_2.y[0])
		{
			g_status = 3;
			return;
		}
	}

}

//结束画面
int EndScreen()
{
	if (g_status == 0) // 游戏没结束
	{
		return 0;
	}

	TCHAR s[100] = "", s2[50] = "";
	if (g_status == 1) {
		_stprintf(s, _T("GameOver!\n您的分数为%d分"), snake_2.count);
	}
	else if (g_status == 2)
	{
		_stprintf(s, _T("你撞墙了 GameOver!\n您的分数为%d分"), snake_2.count);
	}
	else if (g_status == 3)
	{
		_stprintf(s, _T("咬到自己了 GameOver!\n您的分数为%d分"), snake_2.count);
	}
	_stprintf(s2, _T("       按任意键结束....\n"));

	ShowSnake_S();			//画蛇，显示死亡状态
	setbkmode(TRANSPARENT);		//文字透明方式
	settextcolor(LIGHTBLUE);	//文字颜色

	// 结束文字分数提示
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2, s);		//指定位置输出字符串
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2 + SIZE, s2);	//指定位置输出字符串

	// 结束图片展示
	Sleep(1000);
	system("pause");
	putimage(0, 0, &tips[2]);		//贴图，结束界面
	settextcolor(RED);				//文字颜色红
	outtextxy(WIDTH_2 * 2 / 10 + SIZE, HIGHT / 2 + SIZE / 2, s);	//指定位置输出字符串
	return 1;

}

// 画墙
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
