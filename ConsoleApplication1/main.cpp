#include <stdio.h>
#include <windows.h>

int main()
{
	int time, dis;  //����Ļ��ʱ�䳤��
	char str[100];
	while (1)
	{
		printf("���������:");  //�ó���ȥ��
		scanf("%d", &dis);
		time=dis*1+24/2;   //��ϵ
		sprintf(str,"adb shell input swipe 200 500 200 500 %d",time);
		system(str);  //�����ַ�����?
	}
	return 0;
}


//system("cls");  //�����Ļ

//adb  
/*system("adb shell screencap -p /sdcard/screen.png");
system("adb pull /sdcard/screen.png");*/