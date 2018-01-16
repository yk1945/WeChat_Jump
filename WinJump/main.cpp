#include <windows.h> //头文件
#include "atlimage.h"  //png ->bmp
#include <math.h>

#define WINDOW_WIDTH 	(1080/3)
#define WINDOW_HEIGHT 	(1920/3)


HDC		g_hdc,g_memdc;  //窗口绘图句柄 内存句柄 
HDC		g_hscrdc;
HWND	hScreenWnd;
HBITMAP bmp;		 //空位图
HBITMAP screenBmp;   //截图
CImage  image;
int		flag=0;
int		time;
int		preX,preY,nowX,nowY;
int		screenW;
int		screenH;


BOOL GameInit(HWND hwnd);
VOID GamePaint(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

//主函数
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prehInstance,LPSTR lpCmdLine,int nShowCmd) //主函数 
{
	//1.需要什么样子的窗口
	WNDCLASSEX wnd={0}; //定义一个窗口
	wnd.cbSize=sizeof(WNDCLASSEX);     //窗口应该占用内存
	wnd.hInstance=hInstance;           //程序的当前实例句柄
	wnd.style=CS_HREDRAW|CS_VREDRAW; //窗体可以改变水平和垂直方向
	wnd.hbrBackground=(HBRUSH)5;		 //背景颜色
	wnd.hCursor=LoadCursor(NULL,IDC_ARROW);					 //鼠标样式  
	wnd.hIcon=NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));					 //程序的图标
	wnd.cbClsExtra=0;          //程序的附加内存
	wnd.cbWndExtra=0;          //窗口的附加内存
	wnd.lpszMenuName=NULL;	 //菜单
	wnd.lpszClassName=L"FOR OUR DREAMS"; //窗口的类名称
	wnd.lpfnWndProc=WndProc;   //窗口的过程函数

	//2.注册窗口
	RegisterClassEx(&wnd); //检查窗口是否可用

	//3.创建
	HWND hWnd=CreateWindow(L"FOR OUR DREAMS",L"PlaneDemo",
		WS_OVERLAPPEDWINDOW,300,200,WINDOW_WIDTH,WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL);

	//4.显示和更新
	ShowWindow(hWnd,nShowCmd);
	UpdateWindow(hWnd);

	if(!GameInit(hWnd))
		return -1;

	//消息循环
	MSG msg={0};
	while(msg.message!=WM_QUIT) //只要客户发出退出程序的指令
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))//消息队列去获得消息
		{
			//栈在哪里用?代码块   队列在哪里用啊? 打游戏
			TranslateMessage(&msg);  //翻译消息  虚拟键值--->字符消息
			DispatchMessage(&msg);   //发送消息 发送给系统 
		}
		else
		{
			GamePaint(hWnd);
		}
	}
	//【6】窗口类的注销
	UnregisterClass(L"Hola",wnd.hInstance);  //程序准备结束，注销窗口类
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	char str[100];
	switch (message)
	{
	case WM_CREATE:
		hScreenWnd=(HWND)0x00340258;

		//截图
		//system("adb shell screencap -p/sdcard/screen.png");
		//system("adb pull/sdcard/screen.png");
		break;

	case WM_LBUTTONDOWN:
		if(flag==0)
		{
			preX=LOWORD(lParam);
			preY=HIWORD(lParam);
			flag=1;
		}
		else
		{
			nowX=LOWORD(lParam);
			nowY=HIWORD(lParam);
			int dis;
			dis=sqrt((fabs(double(nowX-preX))*fabs(double(nowX-preX)))+
				(fabs(double(nowY-preY))*fabs(double(nowY-preY))));

			POINT pt;
			pt.x=rand()%200+100;
			pt.y=rand()%250+300;
			ClientToScreen(hScreenWnd,&pt);
			SetCursorPos(pt.x,pt.y);
			SetForegroundWindow(hScreenWnd);

			Sleep(500);
			SendMessage(hScreenWnd,WM_LBUTTONDOWN,0,0);
			Sleep((int)(dis*3*1.35+20));
			SendMessage(hScreenWnd,WM_LBUTTONUP,0,0);

			//不用adb
			//sprintf(str,"adb shell input swipe 200 500 200 500 %d",(int)(dis*3*1.35+20));
			//system(str);

			//system("adb shell screencap -p/sdcard/screen.png");
			//system("adb pull/sdcard/screen.png");
			flag = 0;
			//尽量不要让这个游戏觉得我们再刷分
			/*
				1.反复截屏.
				2.触屏的命令.
				3.每一次触屏的位置.
				4. 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32  ---->>中点都是32  3次100
				5. 1 32 32 32 32 32 32 32 32 32 32 32 32
				6.比较简单 手 300
				自动外挂
			*/
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//游戏的初始化
BOOL GameInit(HWND hwnd)
{
	g_hdc=GetDC(hwnd);
	  //窗口绘图句柄 内存句柄 
		
	g_hscrdc=GetDC(hScreenWnd);
	screenW=GetDeviceCaps(g_hscrdc,HORZRES);
	screenH=GetDeviceCaps(g_hscrdc,VERTRES);
	bmp=CreateCompatibleBitmap(g_hscrdc,screenW,screenH);
	g_memdc=CreateCompatibleDC(g_hscrdc);


	   //截图
	
	return TRUE;
}

//游戏的绘制
VOID GamePaint(HWND hwnd)
{
	SelectObject(g_memdc,bmp);
	BitBlt(g_memdc,0,0,screenW,screenH,g_hscrdc,0,0,SRCCOPY);
	//image.Load(L"D:\\Windows 7 Documents\\Desktop\\WinJump\\Debug\\screen.png");
	//image.Save(L"D:\\Windows 7 Documents\\Desktop\\WinJump\\Debug\\screen.bmp");

	//image.Load(L"screen.png");
	//image.Save(L"screen.bmp");
	//image  screen.png-->screen.bmp
	//screenBmp=(HBITMAP)LoadImage(NULL,
		//L"D:\\Windows 7 Documents\\Desktop\\WinJump\\Debug\\screen.bmp",
		//IMAGE_BITMAP,WINDOW_WIDTH-10,WINDOW_HEIGHT-10,LR_LOADFROMFILE);
	/*screenBmp=(HBITMAP)LoadImage(NULL,
		L"scree.bmp",
		IMAGE_BITMAP,WINDOW_WIDTH,WINDOW_HEIGHT,LR_LOADFROMFILE);*/
	//SelectObject(g_memdc,screenBmp);
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_memdc,0,0,SRCCOPY);
}


