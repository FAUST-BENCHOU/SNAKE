#undef UNICODE
#undef _UNICODE		
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <GL/glut.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include<sstream>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include "cmd_console_tools.h"
#include "one_dimential_snake.cpp"
#include "two_dimential_snake.cpp"
#include "two_half_dimential_snake.cpp"
#include "three_dimential_snake.cpp"
#include "four_dimential_snake.cpp"
#pragma warning(disable:4996)	//禁用安全检查 _tcscpy、_stprintf
#pragma comment(lib, "Winmm.lib")
using namespace cv;
using namespace std;
/*设置画面满屏，介绍页面(简单复古)，一维贪吃蛇（only左移）（单调的bgm）*/

POINT* g_pDst;		// 点集(目标)
POINT* g_pSrc;		// 点集(源)
int g_nWidth;		// 文字的宽度
int g_nHeight;		// 文字的高度
int g_nCount;		// 点集包含的点的数量


// 获取目标点集
void GetDstPoints()
{
	// 设置临时绘图对象
	IMAGE img;
	SetWorkingImage(&img);

	// 定义目标字符串
	TCHAR s[] = _T("SNAKE-Thanks for playing!");

	// 计算目标字符串的宽高，并调整临时绘图对象的尺寸
	setcolor(WHITE);
	setfont(100, 0, _T("Arial"));
	g_nWidth = textwidth(s);
	g_nHeight = textheight(s);
	Resize(&img, g_nWidth, g_nHeight);

	// 输出目标字符串至 img 对象
	outtextxy(0, 0, s);

	// 计算构成目标字符串的点的数量
	int x, y;
	g_nCount = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (getpixel(x, y) == WHITE)
				g_nCount++;

	// 计算目标数据
	g_pDst = new POINT[g_nCount];
	int i = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (getpixel(x, y) == WHITE)
			{
				g_pDst[i].x = x + (1540 - g_nWidth) / 2;
				g_pDst[i].y = y + (840 - g_nHeight) / 2;
				i++;
			}

	// 恢复对屏幕的绘图操作
	SetWorkingImage(NULL);
}


// 获取源点集
void GetSrcPoints()
{
	// 设置随机种子
	srand((unsigned int)time(NULL));

	// 设置随机的源数据
	g_pSrc = new POINT[g_nCount];
	for (int i = 0; i < g_nCount; i++)
	{
		g_pSrc[i].x = rand() % 1540;
		g_pSrc[i].y = rand() % 840;
	}
}


// 全屏模糊处理(忽略屏幕第一行和最后一行)
void Blur(DWORD* pMem)
{
	for (int i = 1540; i < 1540 * 839; i++)
	{
		pMem[i] = RGB(
			(GetRValue(pMem[i]) + GetRValue(pMem[i - 1540]) + GetRValue(pMem[i - 1]) + GetRValue(pMem[i + 1]) + GetRValue(pMem[i + 1540])) / 5,
			(GetGValue(pMem[i]) + GetGValue(pMem[i - 1540]) + GetGValue(pMem[i - 1]) + GetGValue(pMem[i + 1]) + GetGValue(pMem[i + 1540])) / 5,
			(GetBValue(pMem[i]) + GetBValue(pMem[i - 1540]) + GetBValue(pMem[i - 1]) + GetBValue(pMem[i + 1]) + GetBValue(pMem[i + 1540])) / 5);
	}
}


// 主函数
void end_snake()
{
	// 初始化
	initgraph(1540, 840);				// 创建绘图窗口看
	DWORD* pBuf = GetImageBuffer();		// 获取显示缓冲区指针
	GetDstPoints();						// 获取目标点集
	GetSrcPoints();						// 获取源点集

	HWND hWnd_thx = GetForegroundWindow(); //获取当前活动窗口句柄
	SetWindowPos(hWnd_thx, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER); //将窗口移动到屏幕左上角
	// 运算
	int x, y;
	for (int i = 2; i <= 256; i += 2)
	{
		COLORREF c = RGB(i - 1, i - 1, i - 1);
		Blur(pBuf);						// 全屏模糊处理

		for (int d = 0; d < g_nCount; d++)
		{
			x = g_pSrc[d].x + (g_pDst[d].x - g_pSrc[d].x) * i / 256;
			y = g_pSrc[d].y + (g_pDst[d].y - g_pSrc[d].y) * i / 256;
			pBuf[y * 1540 + x] = c;		// 直接操作显示缓冲区画点
		}

		Sleep(20);						// 延时
	}

	// 清理内存
	delete g_pDst;
	delete g_pSrc;

	// 按任意键退出
	_getch();
	closegraph();
}

void full_screen()
{
	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
	int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */

	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* 获取窗口信息 */
	/* 设置窗口信息 最大化 取消标题栏及边框 */
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}
void restore_screen()
{
	HWND hwnd = GetForegroundWindow();
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);
	/* 恢复窗口信息 去掉最大化 取消标题栏及边框 */
	SetWindowLong(hwnd, GWL_STYLE, l_WinStyle | WS_CAPTION | WS_THICKFRAME | WS_BORDER);
	ShowWindow(hwnd, SW_RESTORE);
}

void camera()
{
	// 1.创建视频采集对象;
	VideoCapture cap;

	// 2.打开默认相机;
	cap.open(0);

	// 3.判断相机是否打开成功;
	if (!cap.isOpened())
		return ;

	// 4.显示窗口命名;
	namedWindow("Video", WINDOW_NORMAL); //创建一个可调整大小的窗口
	resizeWindow("Video", 1540, 850); //设置窗口的初始大小
	// 获取窗口句柄
	HWND hWnd = GetForegroundWindow();

	// 设置窗口位置
	SetWindowPos(hWnd, NULL, 0, 0,1540 / 2, 850 / 2, SWP_NOZORDER);
	cap.set(CAP_PROP_FRAME_WIDTH, 1540); //设置帧的宽度
	cap.set(CAP_PROP_FRAME_HEIGHT, 850); //设置帧的高度
	auto start_time = std::chrono::steady_clock::now();//获取当前时间
	for (;;)
	{
		// 获取新的一帧;
		Mat frame;
		cap >> frame;
		if (frame.empty())
			return ;

		// 显示新的帧;
		imshow("Video", frame);
		auto current_time = std::chrono::steady_clock::now();
		// Calculate the elapsed time in seconds
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

		// Check if 5 seconds have elapsed
		if (elapsed_seconds >= 5)
		{
			// Exit the loop
			break;
		}
		// 按键退出显示;
		if (waitKey(30) >= 0) break;
	}
	// 5.释放视频采集对象;
	cap.release();
	destroyAllWindows();
    return;
}

HCRYPTPROV prov;

void mess1(LPVOID lp)
{
	MessageBoxA(NULL, "TRAP_CAUSE_UNKNOWN", "Windows错误报告", MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
}

void mess2(LPVOID lp)
{
	MessageBoxA(NULL, "TRAP_CAUSE_UNKNOWN", "Windows错误报告", MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
}

void mess3(LPVOID lp)
{
	MessageBoxA(NULL, "TRAP_CAUSE_UNKNOWN", "Windows错误报告", MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
}

//主函数
void messagebox()
{
	
	int NumberOfMessageBox = 100;
	HWND hwnd_mess[901];
	HWND ThisProgram = GetForegroundWindow();
	NumberOfMessageBox += 3;
	ShowWindow(ThisProgram, SW_HIDE);
	
	for (int i = 1; i <= NumberOfMessageBox / 3; i++)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mess1, 0, 0, 0);
		Sleep(90);
		hwnd_mess[i] = FindWindowA(NULL, "Windows错误报告");
		ShowWindow(hwnd_mess[i], SW_HIDE);
		MoveWindow(hwnd_mess[i], rand() % 1220, rand() % 1080, 220, 180, false);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mess2, 0, 0, 0);
		Sleep(90);
		hwnd_mess[i + 300] = FindWindowA(NULL, "Windows错误报告");
		ShowWindow(hwnd_mess[i + 300], SW_HIDE);
		MoveWindow(hwnd_mess[i + 300], rand() % 1220, rand() % 1080, 220, 180, false);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mess3, 0, 0, 0);
		Sleep(90);
		hwnd_mess[i + 600] = FindWindowA(NULL, "Windows错误报告");
		ShowWindow(hwnd_mess[i + 600], SW_HIDE);
		MoveWindow(hwnd_mess[i + 600], rand() % 1220, rand() % 1080, 220, 180, false);
	}
	Sleep(1000);
	for (int i = 1; i <= NumberOfMessageBox/3; i++)
	{
		ShowWindow(hwnd_mess[i], SW_SHOW);
		Sleep(15);
		ShowWindow(hwnd_mess[i + 300], SW_SHOW);
		Sleep(15);
		ShowWindow(hwnd_mess[i + 600], SW_SHOW);
	}
	Sleep(3000);
	for (int i = 1; i <= NumberOfMessageBox / 3; i++)
	{
		SendMessage(hwnd_mess[i], WM_CLOSE, 0, 0);
		Sleep(5);
		SendMessage(hwnd_mess[i + 300], WM_CLOSE, 0, 0);
		Sleep(5);
		SendMessage(hwnd_mess[i + 600], WM_CLOSE, 0, 0);
		Sleep(5);
	}
	
}

void mouse_fly() {
	int x = GetSystemMetrics(SM_CXSCREEN);//获取屏幕边界x
	int y = GetSystemMetrics(SM_CYSCREEN);//获取屏幕边界y
	srand(time(0));//使用随机变化值空间
	for(int i=0;i<500;i++){
		SetCursorPos(rand() % x, rand() % y);//依次将鼠标指针移到屏幕上的随机位置
		Sleep(10);
	}
}

void blue_screen() {

	system("mode 96,28");
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 15;
	cfi.dwFontSize.Y = 30;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"MS Gothic");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	system("color 1f");
	RECT rect;
	HWND hwnd = GetForegroundWindow();
	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, -30, -7, 8600, 1080, TRUE);
	full_screen();
	cout << "A problem has been detected and windows has been shut\
down to prevent damage\n\
to your computer.\n\n\
DRIVER_IROL_NOT_LESS_OR_EQUAL\n\n\
If this is the first time you've seen this Stop error screen, \n\
restart your computer.If this screen appears again, follow \n\
these steps:\n\n\
Check to make sure any new hardware or software is properly installed.\n\
If this is a new installation, ask your hardware or software manufacturer\n\
for any windows updates you might need.\n\n\
If problems continue. disable or remove any newly installed hardware\n\
or software. Disable BIOS memory option such as caching or shadowing.\n\
If you need to nsu Safe Mode to remove or disable components, restart\n\
your computer, press F8 to select Advanced Startup Options, and then\n\
select Safe Mode.\n\n\
Technical information:\n\
\"\"\"STOP:0x00000050（0xFD3094c2,0x00000001,0xFBFE7617,0x00000000）\n\n\
\"\"\"SPCMDON.SYS - Address FBFE7617 base at FBFE5000, DateStamp 3d6dd67c" << endl;
	
}

int main(int argc, char** argv) {
	full_screen();
	
	one_dimensional_snake();
	cct_cls();
	two_dimential_snake();
    Sleep(3500);
    cct_cls();
   cout << " D: lMyGamelx64 \Debug MyGame.exe（进程26196)已退出，代码为0。" << endl << "按任意键关闭此窗口.. .";
    Sleep(3500);
    
	cct_cls();

	blue_screen();
	mouse_fly();
	cct_cls();
	system("color 0f");
	cct_setfontsize("Terminal", 20, 10);

	restore_screen();
	two_half_dimential_snake();
	
	HWND hWnd2 = FindWindow(NULL, "MyGame"); // 通过窗口标题寻找窗口句柄
	if (hWnd2 != NULL)
	{
		ShowWindow(hWnd2, SW_MINIMIZE); // 最小化窗口
		Sleep(1000); // 等待1秒，确保窗口已最小化
		RECT rect;
		GetWindowRect(hWnd2, &rect); // 获取窗口位置和大小信息
		MoveWindow(hWnd2, rect.left, rect.top, 1, 1, TRUE); // 将窗口大小修改为1x1像素
	}

	mciSendString(_T("open Following.mp3 alias mymusic"), NULL, 0, NULL);

	// 播放音乐
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);

	cct_setconsoleborder(1, 1);
	cct_cls();

    camera();
	Sleep(3500);
	
	ofstream fout;
	fout.open("three_dim_in.txt");
	fout << "You only have 80 seconds to go through the next scene,be careful." << endl<<endl<<"In that accursed temple,"<<endl<<"A man is trapped, and snakes are everywhere."<<endl<<"He tries to escape, but finds"<<endl<<"Each door and path leads to the same way."<<endl<<endl
		<<"Whenever he walks down a corridor,"<<endl<<"He hears the hissing of snakes beneath the ground."<<endl<<"In each room, there are snake shapes,"<<endl<<"Staring at him with evil eyes, waiting for their chance."<<endl
		<<"This python, more terrible than the others,"<<endl<<endl<<"This python, more terrible than the others,"<<endl<<"Has shining eyes that seem very intelligent."<<endl<<"The man knows he must kill this python,"<<endl<<"Or he will never escape from this cursed temple."<<endl;
	WinExec("notepad.exe three_dim_in.txt", SW_SHOW);
	// 延时一段时间，等待记事本窗口打开
	Sleep(22000);
	// 查找记事本窗口句柄
	HWND hWnd = FindWindow("Notepad", NULL);
	// 如果找到了窗口，则发送关闭消息
	if (hWnd != NULL) {
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	else {
		cout << "未能找到记事本窗口" << std::endl;
	}

	glutInit(&argc, argv);
	three_dimential_snake();
	
	ofstream fout_2;
	fout_2.open("four_dim_in.txt");
	fout_2 << "You only have 10 seconds to go through the next scene,be careful." << endl << endl << "In what dimension do you reside?" << endl << endl << "Where do you exist in time and space?" << endl << endl << "In what class do you belong?"
		<< endl << endl << "Are you facing forward or in reverse?" << endl << endl << "Do you stand before or behind?" << endl << endl << "To the left or to the right are you inclined?" << endl << endl << "Within or without, where do you abide?"<<endl;
	WinExec("notepad.exe four_dim_in.txt", SW_SHOW);
	// 延时一段时间，等待记事本窗口打开
	Sleep(15000);
	// 查找记事本窗口句柄
	HWND hWnd_fourdim = FindWindow("Notepad", NULL);
	// 如果找到了窗口，则发送关闭消息
	if (hWnd_fourdim != NULL) {
		SendMessage(hWnd_fourdim, WM_CLOSE, 0, 0);
	}
	else {
		cout << "未能找到记事本窗口" << std::endl;
	}
	
	glutInit(&argc, argv);
	four_dimential_snake();

	Sleep(3000);

	ofstream fout_end;
	fout_end.open("end.txt");
	fout_end <<"Has it ended?" <<endl<<endl<<endl<<"Do you remember what I just said to you?"<< endl << endl << endl << "Within or without, where do you abide?"<<endl;
	WinExec("notepad.exe end.txt", SW_SHOW);
	// 延时一段时间，等待记事本窗口打开
	Sleep(15000);
	// 查找记事本窗口句柄
	HWND hWnd_end = FindWindow("Notepad", NULL);
	// 如果找到了窗口，则发送关闭消息
	if (hWnd_end != NULL) {
		SendMessage(hWnd_end, WM_CLOSE, 0, 0);
	}
	else {
		cout << "未能找到记事本窗口" << std::endl;
	}
	Sleep(3000);
	restore_screen();
	cct_setconsoleborder(1540, 850);
	char input[100] = { 0 };
	cout << "感谢您的游玩，作者做这个游戏做的要死要活呜呜呜呜呜" << endl
		<< "接下来是一个小彩蛋~~~你只要配合就好啦";
	Sleep(5000);
	cct_cls();
	
	cout << "现在，您将10s的时间读完这段话，有15s的时间输入这个游戏的名字：SNAKE,否则我将"<<endl << endl<<"通过定义的const char* file_path = 'C:\\Programme Files'"<<endl<<endl
		<<"和while(1){remove(file_path);}来反复尝试删除你电脑里的这个文件夹，" << endl << endl<<"当然，如果您没有这个文件夹那就没有必要担心，有的话也没有关系"<<endl << endl
		<<"输入就可以了，考验你的反应，毕竟这是一个一次性游戏。"<<endl << endl;
	Sleep(10000);
	for (int i = 0; i < 3; i++) {
		cout << "请输入SNAKE:";
		cin >> input;
	}
	cout << "请输入HUNGRY-SNAKE:";
	cin >> input;
	cout << "请输入HUNGRY-SNAKE-GAME:";
	cin >> input;
	cout << "请输入HUNGRY-SNAKE-GAMEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE:"<<endl;
	cin >> input;
	Sleep(3500);
	cct_cls();
	messagebox();
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	Sleep(2000);

	ofstream fout_ques;
	fout_ques.open("ques.txt");
	fout_ques << "Now,who are inside?" << endl ;
	WinExec("notepad.exe ques.txt", SW_SHOW);

	// 延时一段时间，等待记事本窗口打开
	Sleep(5000);
	// 查找记事本窗口句柄
	HWND hWnd_ques = FindWindow("Notepad", NULL);
	// 如果找到了窗口，则发送关闭消息
	if (hWnd_ques != NULL) {
		SendMessage(hWnd_ques, WM_CLOSE, 0, 0);
	}
	else {
		cout << "未能找到记事本窗口" << std::endl;
	}

	end_snake();
}
 


/*完成游戏后需要有过渡，下一个界面（稍微复杂美观），二维贪吃蛇（略微欢快的bgm）*/
/*报错*/
/*恢复为二维贪吃蛇，但是场景十分抽象*/
/*画面崩坏*/

/*恢复正常，可爱的画面(二维彩色)，且元素更高级*/
/*加入关机和向桌面发送文件的元素*/
/*控制台发出询问消息（人机互动）*/

/*boss:3d*/


