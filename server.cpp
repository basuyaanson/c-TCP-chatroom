#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h> //easyX圖形庫
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#define NUM 1024 //最大連接人數
SOCKET cSocket[NUM];
int count;//當前連接人數
void s_thread(int idx); //和用戶端通信的執行緒函數

int main()
{
	SetConsoleTitle(L"momotalk_server");
	printf("---------------------------------------------歡迎使用momotalk伺服器!!!---------------------------------------------\n");
	count = 0;
	//初始化Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("初始化Winsock失敗\n");
		WSACleanup();//清理Winsock
		system("pause");
		return -1;
	}
	printf("Winsock初始化成功!\n");

	//創建socket
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket)
	{
		printf("創建socketg失敗:%d\n", GetLastError);
		system("pause");
		return -2;
	}
	printf("創建socket成功\n");
	
	//輸入伺服器IP
	char mip[32] = "";
	printf("請輸入伺服器IP:");
	scanf("%s", mip);
	
	//確定伺服器IP地址
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//使用IPv4
	addr.sin_addr.S_un.S_addr = inet_addr(mip);
	addr.sin_port = htons(10086);//通訊埠

	//綁定
	int r = bind(sSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("綁定失敗: % d\n", GetLastError);
		system("pause");
		closesocket(sSocket); //斷開連線
		WSACleanup();//清理Winsock
		
		return -2;
	}
	printf("綁定成功\n");

	//監聽
	r = listen(sSocket, 10);
	if (-1 == r)
	{
		printf("監聽失敗: % d\n", GetLastError);
		system("pause");
		closesocket(sSocket); //斷開連線
		WSACleanup();
		
		return -2;
	}
	printf("監聽成功\n");
	printf("\n\n---------------------------------------------\n伺服器正在運作中.....\n---------------------------------------------\n");
	system("color 70");
	
	//等待用戶端連接
	for (int i = 0; i < NUM; i++)
	{
		cSocket[i] = accept(sSocket, NULL, NULL);
		if (SOCKET_ERROR == cSocket[i])
		{
			printf("伺服器崩潰: % d\n", GetLastError);
			system("pause");
			closesocket(sSocket); //斷開連線
			WSACleanup();
			
			return -3;
		}
		printf("---------------------------------有客戶連接上伺服器:%d\n", i);
		count++;
		//創建一個執行緒和用戶端通訊
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)s_thread, (LPVOID)i, NULL, NULL);
	}
	while (1);
	return 0;
}

//和用戶端通信的執行緒函數
void s_thread(int idx)
{
	//通信
	char buff[256] = { 0 };
	int r;//接收用戶端字串
	int j;//接收用戶端暱稱
	char temp[256];//臨時字符串,用來專發給所有用戶
	char name[256];//用戶端暱稱

	while (1)
	{
		//從用戶端接收數據
		r = recv(cSocket[idx], buff, 255, NULL);
		j = recv(cSocket[idx], name, 255, NULL); 
		if (r > 0)
		{
			buff[r] = 0;//添加結束符號	
			name[j] = 0;
			printf(">> (%d)_%s: %s\n", idx,name, buff);//輸出 

			//從伺服器發送消息給所有用戶端
			memset(temp, 0, 256);
			sprintf(temp,"%d_%s: %s", idx, name, buff);
			for (int i = 0; i < count; i++)
			send(cSocket[i], temp, strlen(temp), NULL);
		}
	}
}
