#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <graphics.h> //easyX圖形庫
#include<vector>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")//靜態庫
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
using namespace std;
SOCKET sSocket;
void scanfAndSend();//執行緒函數
void gif();//動態圖
void buttom();//按鈕事件
constexpr auto swidth = 1200;//聊天室長寬
constexpr auto sheight = 600;
int si = 0;//全局變量，用來防止音樂多次播放
//按鈕點擊判定
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
	printf("---------------------------------------------歡迎使用momotalk!---------------------------------------------\n");
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
	printf("初始化Winsock成功!\n");

	//創建socket
	sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket)
	{
		printf("創建socketg失敗,錯誤原因:%d\n", GetLastError);
		system("pause");
		return -2;
	}
	printf("創建socket成功\n\n");

	//輸入伺服器IP
	char mip[32]="";
	printf("請輸入伺服器IP:");
	scanf("%s",mip);
	
	//確定IP地址
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//使用IPv4
	addr.sin_addr.S_un.S_addr = inet_addr(mip);
	addr.sin_port = htons(10086);//通訊埠

	//連接伺服器
	int r = connect(sSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{
		printf("連接伺服器失敗,錯誤原因: %d\n", GetLastError);
		system("pause");
		return -2;
	}

	//創建多執行緒
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)scanfAndSend, NULL, NULL, NULL);
	char buff[256];//用戶端字符串
	int n = 4;//行數,用來判斷清屏時機
	char name [256];//用戶端暱稱
	while (1)
	{
		r = recv(sSocket, buff, 255, NULL);//伺服器接收循環	
		if (r > 0)
		{
			buff[r] = 0;//結束符號		
			settextcolor(BLACK);
			outtextxy(0, n * 20, buff);//讓訊息從上往下					
			n++;
			if (n > 25)
			{
				n = 4;
				cleardevice();//當聊天室到第21行,清屏
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)buttom, NULL, NULL, NULL);
			}			
		}		
	}
	while (1);//停頓
	return 0;
}


//執行緒函數
void scanfAndSend()
{
	printf("----------------連接成功----------------\n ");
	
	char buff[256];\\字串
	char name[256];\\暱稱
	printf("請輸入您的暱稱: ");
	scanf("%s", name);
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)gif, NULL, NULL, NULL);
	
	printf("\n----------------歡迎來到momotalk----------------\n ");
	printf("請輸入要發送的內容:\n ");
	while (1)
	{			
		memset(buff, 0, 256);//清空數據	
		scanf("%s", buff);
		send(sSocket, buff, strlen(buff), NULL);//發送給伺服器
		send(sSocket, name, strlen(name), NULL);
	}
}

//gif圖
void gif()
{	
	initgraph(swidth, sheight, 1);
	SetWindowText(GetHWnd(),"momotalk");
	setbkcolor(DARKGRAY);
	cleardevice();
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)buttom, NULL, NULL, NULL);
	vector<IMAGE> run;
	TCHAR imgname[256];
	
	for (int i = 0; i < 234; i++)\\遍歷載入圖片，使用vector儲存
	{
		_stprintf_s(imgname, "./gif/(%d).jpg", i);
		IMAGE g1;
		loadimage(&g1, imgname, 950,600);
		run.push_back(g1);
	}

	IMAGE imgshow;
	int i = 0;
	while (1)\\遍歷打印圖片
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

//按鈕
void buttom()
{
	music();
	settextcolor(WHITE);
	outtextxy(0, 0, "--------------------歡迎使用momotalk-----------------------------");
	outtextxy(0, 20,"注意:單行文字請盡量不超過10個字");
	outtextxy(0, 40,"每隔20行會進行一次清屏");
	outtextxy(0, 60,"--------------------------------------------------------------------------");
	while (true)
	 {
		settextcolor(WHITE);
		outtextxy(0,520 , "--------------------------------------------------------------------");
		LPCTSTR tplay = _T("繼續音樂播放"); LPCTSTR tpause = _T("暫停音樂播放");
		LPCTSTR texit = _T("退出聊天室");
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
