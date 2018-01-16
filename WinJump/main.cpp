#include <windows.h> //ͷ�ļ�
#include "atlimage.h"  //png ->bmp
#include <math.h>

#define WINDOW_WIDTH 	(1080/3)
#define WINDOW_HEIGHT 	(1920/3)


HDC		g_hdc,g_memdc;  //���ڻ�ͼ��� �ڴ��� 
HDC		g_hscrdc;
HWND	hScreenWnd;
HBITMAP bmp;		 //��λͼ
HBITMAP screenBmp;   //��ͼ
CImage  image;
int		flag=0;
int		time;
int		preX,preY,nowX,nowY;
int		screenW;
int		screenH;


BOOL GameInit(HWND hwnd);
VOID GamePaint(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

//������
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prehInstance,LPSTR lpCmdLine,int nShowCmd) //������ 
{
	//1.��Ҫʲô���ӵĴ���
	WNDCLASSEX wnd={0}; //����һ������
	wnd.cbSize=sizeof(WNDCLASSEX);     //����Ӧ��ռ���ڴ�
	wnd.hInstance=hInstance;           //����ĵ�ǰʵ�����
	wnd.style=CS_HREDRAW|CS_VREDRAW; //������Ըı�ˮƽ�ʹ�ֱ����
	wnd.hbrBackground=(HBRUSH)5;		 //������ɫ
	wnd.hCursor=LoadCursor(NULL,IDC_ARROW);					 //�����ʽ  
	wnd.hIcon=NULL;//LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));					 //�����ͼ��
	wnd.cbClsExtra=0;          //����ĸ����ڴ�
	wnd.cbWndExtra=0;          //���ڵĸ����ڴ�
	wnd.lpszMenuName=NULL;	 //�˵�
	wnd.lpszClassName=L"FOR OUR DREAMS"; //���ڵ�������
	wnd.lpfnWndProc=WndProc;   //���ڵĹ��̺���

	//2.ע�ᴰ��
	RegisterClassEx(&wnd); //��鴰���Ƿ����

	//3.����
	HWND hWnd=CreateWindow(L"FOR OUR DREAMS",L"PlaneDemo",
		WS_OVERLAPPEDWINDOW,300,200,WINDOW_WIDTH,WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL);

	//4.��ʾ�͸���
	ShowWindow(hWnd,nShowCmd);
	UpdateWindow(hWnd);

	if(!GameInit(hWnd))
		return -1;

	//��Ϣѭ��
	MSG msg={0};
	while(msg.message!=WM_QUIT) //ֻҪ�ͻ������˳������ָ��
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))//��Ϣ����ȥ�����Ϣ
		{
			//ջ��������?�����   �����������ð�? ����Ϸ
			TranslateMessage(&msg);  //������Ϣ  �����ֵ--->�ַ���Ϣ
			DispatchMessage(&msg);   //������Ϣ ���͸�ϵͳ 
		}
		else
		{
			GamePaint(hWnd);
		}
	}
	//��6���������ע��
	UnregisterClass(L"Hola",wnd.hInstance);  //����׼��������ע��������
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	char str[100];
	switch (message)
	{
	case WM_CREATE:
		hScreenWnd=(HWND)0x00340258;

		//��ͼ
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

			//����adb
			//sprintf(str,"adb shell input swipe 200 500 200 500 %d",(int)(dis*3*1.35+20));
			//system(str);

			//system("adb shell screencap -p/sdcard/screen.png");
			//system("adb pull/sdcard/screen.png");
			flag = 0;
			//������Ҫ�������Ϸ����������ˢ��
			/*
				1.��������.
				2.����������.
				3.ÿһ�δ�����λ��.
				4. 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32  ---->>�е㶼��32  3��100
				5. 1 32 32 32 32 32 32 32 32 32 32 32 32
				6.�Ƚϼ� �� 300
				�Զ����
			*/
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//��Ϸ�ĳ�ʼ��
BOOL GameInit(HWND hwnd)
{
	g_hdc=GetDC(hwnd);
	  //���ڻ�ͼ��� �ڴ��� 
		
	g_hscrdc=GetDC(hScreenWnd);
	screenW=GetDeviceCaps(g_hscrdc,HORZRES);
	screenH=GetDeviceCaps(g_hscrdc,VERTRES);
	bmp=CreateCompatibleBitmap(g_hscrdc,screenW,screenH);
	g_memdc=CreateCompatibleDC(g_hscrdc);


	   //��ͼ
	
	return TRUE;
}

//��Ϸ�Ļ���
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


