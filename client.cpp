#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <graphics.h> //easyX�ϧήw
#include<vector>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")//�R�A�w
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;
SOCKET sSocket;
void scanfAndSend();//��������
void gif();//gif
void buttom();//���s�ƥ�
constexpr auto swidth = 1200;
constexpr auto sheight = 600;
int si = 0;
//���s�I���P�w
bool pointinrect(int x, int y, RECT& r)
{
	return(r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}
void music()
{
	if (si < 1)
	{
	mciSendString("open ./music/1.mp3", 0, 0, 0);
	mciSendString("play ./music/1.mp3 repeat", 0, 0, 0);
	si++;
	}
	mciSendString("resume ./music/1.mp3 repeat", 0, 0, 0);
}
void pausemu()
{	
	mciSendString("pause ./music/1.mp3", 0, 0, 0);
}
void resume()
{
	mciSendString("resume ./music/1.mp3", 0, 0, 0);
}

int main()
{
	SetConsoleTitle("momotalk_client");
	printf("---------------------------------------------�w��ϥ�momotalk!---------------------------------------------\n");
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
	sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket)
	{
		printf("�Ы�socketg����,���~��]:%d\n", GetLastError);
		system("pause");
		return -2;
	}
	printf("�Ы�socket���\\n\n");

	//��J���A��IP
	char mip[32]="";
	printf("�п�J���A��IP:");
	scanf("%s",mip);
	
	//�T�w���A����ĳ�M��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//�Msocket��ƲĤ@�ѼƫO���@�P
	addr.sin_addr.S_un.S_addr = inet_addr(mip);
	addr.sin_port = htons(10086);//�j�p�ݧ��ഫ

	//�s�����A��
	int r = connect(sSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("�s�����A������,���~��]: %d\n", GetLastError);
		system("pause");
		return -2;
	}

	//�Ыئh�����
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)scanfAndSend, NULL, NULL, NULL);
	char buff[256];//�Τ�ݦr�Ŧ�
	int n = 4;//���,�ΨӧP�_�M�̮ɾ�
	char name [256];//�Τ�ݼʺ�
	while (1)
	{
		r = recv(sSocket, buff, 255, NULL);//���A�������`��	
		if (r > 0)
		{
			buff[r] = 0;//�����Ÿ�		
			settextcolor(BLACK);
			outtextxy(0, n * 20, buff);//���T���q�W���U					
			n++;
			if (n > 25)
			{
				n = 4;
				cleardevice();//���ѫǨ��21��,�M��
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)buttom, NULL, NULL, NULL);
			}			
		}		
	}
	while (1);//���y
	return 0;
}


//��������
void scanfAndSend()
{
	printf("----------------�s�����\----------------\n ");
	//�q�H
	char buff[256];
	char name[256];
	printf("�п�J�z���ʺ�: ");
	scanf("%s", name);
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)gif, NULL, NULL, NULL);
	
	printf("\n----------------�w��Ө�momotalk----------------\n ");
	printf("�п�J�n�o�e�����e:\n ");
	while (1)
	{			
		memset(buff, 0, 256);//�M�żƾ�	
		scanf("%s", buff);
		send(sSocket, buff, strlen(buff), NULL);//�o�e�����A��
		send(sSocket, name, strlen(name), NULL);
	}
}

//gif��
void gif()
{	
	initgraph(swidth, sheight, 1);
	SetWindowText(GetHWnd(),"momotalk");
	setbkcolor(DARKGRAY);
	cleardevice();
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)buttom, NULL, NULL, NULL);
	vector<IMAGE> run;
	TCHAR imgname[256];
	
	for (int i = 0; i < 234; i++)
	{
		_stprintf_s(imgname, "./gif/(%d).jpg", i);
		IMAGE g1;
		loadimage(&g1, imgname, 950,600);
		run.push_back(g1);
	}

	IMAGE imgshow;
	int i = 0;
	while (1)
	{
		if (i == 231)
		{
			i = 1;
		}

		imgshow = run[i];
		putimage(300, 0, &imgshow);
		i++;
		Sleep(100);
	}
}

//���s
void buttom()
{
	music();
	settextcolor(WHITE);
	outtextxy(0, 0, "--------------------�w��ϥ�momotalk-----------------------------");
	outtextxy(0, 20,"�`�N:����r�кɶq���W�L10�Ӧr");
	outtextxy(0, 40,"�C�j20��|�i��@���M��");
	outtextxy(0, 60,"--------------------------------------------------------------------------");
	while (true)
	 {
		settextcolor(WHITE);
		outtextxy(0,520 , "--------------------------------------------------------------------");
		LPCTSTR tplay = _T("�~�򭵼ּ���"); LPCTSTR tpause = _T("�Ȱ����ּ���");
		LPCTSTR texit = _T("�h�X��ѫ�");
		RECT tplayr, tpauser, texitr;
		tplayr.left = 20;
		tplayr.right = tplayr.left + 50;
		tplayr.top = 540;
		tplayr.bottom = 570;

		texitr.left = 150;
		texitr.right = texitr.left + 50;
		texitr.top = 560;
		texitr.bottom = 580;

		tpauser.left = 20;
		tpauser.right = texitr.left + 50;
		tpauser.top = 570;
		tpauser.bottom = 600;

		outtextxy(tplayr.left, tplayr.top, tplay);
		outtextxy(texitr.left, texitr.top, texit);
		outtextxy(tpauser.left, tpauser.top, tpause);
	
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (pointinrect(mess.x, mess.y, tplayr))
			{
				resume();
			}
			else if (pointinrect(mess.x, mess.y, texitr))
			{
				exit(0);
			}
			else if (pointinrect(mess.x, mess.y, tpauser))
			{
				pausemu();
			}
		}
	}
}
