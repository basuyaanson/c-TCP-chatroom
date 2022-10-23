#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h> //easyX�ϧήw
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#define NUM 1024 //�̤j�s���H��
SOCKET cSocket[NUM];
int count;//��e�s���H��
void s_thread(int idx); //�M�Τ�ݳq�H����������

int main()
{
	SetConsoleTitle(L"momotalk_server");
	printf("---------------------------------------------�w��ϥ�momotalk���A��!!!---------------------------------------------\n");
	count = 0;
	//�T�w��ĳ����
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("�ШD����ĳ�������O2.2����\n");
		WSACleanup();//�M�z��ĳ�����H��
		system("pause");
		return -1;
	}
	printf("�ШD����ĳ�����O2.2����!\n");

	//�Ы�socket
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket)
	{
		printf("�Ы�socketg����:%d\n", GetLastError);
		system("pause");
		return -2;
	}
	printf("�Ы�socket���\\n");
	
	//��J���A��IP
	char mip[32] = "";
	printf("�п�J���A��IP:");
	scanf("%s", mip);
	
	//�T�w���A����ĳ�M��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//�Msocket��ƲĤ@�ѼƫO���@�P
	addr.sin_addr.S_un.S_addr = inet_addr(mip);
	addr.sin_port = htons(10086);//�j�p�ݧ��ഫ

	//�j�w
	int r = bind(sSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("�j�w����: % d\n", GetLastError);
		system("pause");
		closesocket(sSocket); //�_�}�s�u
		WSACleanup();//�M�z��ĳ�����H��
		
		return -2;
	}
	printf("�j�w���\\n");

	//��ť
	r = listen(sSocket, 10);
	if (-1 == r)
	{
		printf("��ť����: % d\n", GetLastError);
		system("pause");
		closesocket(sSocket); //�_�}�s�u
		WSACleanup();//�M�z��ĳ�����H��
		
		return -2;
	}
	printf("��ť���\\n");
	printf("\n\n---------------------------------------------\n���A�����b�B�@��.....\n---------------------------------------------\n");
	system("color 70");
	
	//���ݥΤ�ݳs��
	for (int i = 0; i < NUM; i++)
	{
		cSocket[i] = accept(sSocket, NULL, NULL);
		if (SOCKET_ERROR == cSocket[i])
		{
			printf("���A���Y��: % d\n", GetLastError);
			system("pause");
			closesocket(sSocket); //�_�}�s�u
			WSACleanup();//�M�z��ĳ�����H��
			
			return -3;
		}
		printf("---------------------------------���Ȥ�s���W���A��:%d\n", i);
		count++;
		//�Ыؤ@�Ӱ�����M�Τ�ݳq�T
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)s_thread, (LPVOID)i, NULL, NULL);
	}
	while (1);
	return 0;
}

//�M�Τ�ݳq�H����������
void s_thread(int idx)
{
	//�q�H
	char buff[256] = { 0 };
	int r;//�����Τ�ݦr��
	int j;//�����Τ�ݼʺ�
	char temp[256];//�{�ɦr�Ŧ�,�ΨӱM�o���Ҧ��Τ�
	char name[256];//�Τ�ݼʺ�

	while (1)
	{
		//�q�Τ�ݱ����ƾ�
		r = recv(cSocket[idx], buff, 255, NULL);
		j = recv(cSocket[idx], name, 255, NULL); 
		if (r > 0)
		{
			buff[r] = 0;//�K�[�����Ÿ�	
			name[j] = 0;
			printf(">> (%d)_%s: %s\n", idx,name, buff);//��X 

			//�q���A���o�e�������Ҧ��Τ��
			memset(temp, 0, 256);
			sprintf(temp,"%d_%s: %s", idx, name, buff);
			for (int i = 0; i < count; i++)
			send(cSocket[i], temp, strlen(temp), NULL);
		}
	}
}
