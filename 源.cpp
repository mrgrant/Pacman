#include <graphics.h>
#include <time.h>
#include <conio.h>
#pragma comment(lib,"Winmm.lib")

#define M 640                       //开屏大小
#define N 480     
#define rank 1                      //游戏等级       
#define doublescore_time 20
#define speedup_time 10
#define enemyslow_time 10
#define winscore 300

int speed = 0;
int  g_sum = 0;                   //统计吃豆的个数
int  g_score = 0;                   //分数统计
bool g_win = true;                //成败的标志
IMAGE image[12];                     //装载图片
enum { LEFT = 1, RIGHT, UP, DOWN };     //方向的控制
struct figure						//人物属性
{
	int x;				   //人物图纸坐标      
	int y;
	int x1;                //人物方向移动
	int y1;
	int x2;				   //人物物理坐标
	int y2;
	int dir;
}you, enemy[5];

/*附加功能*/
int doublescoreshow = 0;
int doublescore_t = 0;
int speedupshow = 0;
int speedup_t = 0;
int enemyslowshow = 0;
int enemyslow_t = 0;


/*0豆子,1可以移动地方,2墙壁,3自己,4敌人,9双倍，10加速*/

int a[15][20] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,2,2,0,0,2,0,0,0,0,0,0,0,4,0,0,0,2,
2,0,0,0,2,0,0,2,0,0,0,2,2,2,2,2,0,0,0,2,
2,4,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,0,2,0,0,2,0,3,2,2,0,0,0,0,2,2,0,2,
2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,0,0,0,0,0,0,2,0,0,0,2,2,2,0,0,0,2,
2,0,0,0,0,0,2,0,0,2,0,0,0,2,0,0,4,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,0,2,0,0,0,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,2,2,0,0,0,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,2,0,0,2,
2,0,2,2,0,2,2,0,0,0,0,0,0,4,0,2,0,0,0,2,
2,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };/*数组就是地图*/

int b[15][20] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,2,2,0,0,2,0,0,0,0,0,0,0,4,0,0,0,2,
2,0,0,0,2,0,0,2,0,0,0,2,2,2,2,2,0,0,0,2,
2,4,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,0,2,0,0,2,0,3,2,2,0,0,0,0,2,2,0,2,
2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
2,0,2,0,0,0,0,0,0,2,0,0,0,2,2,2,0,0,0,2,
2,0,0,0,0,0,2,0,0,2,0,0,0,2,0,0,4,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,0,2,0,0,0,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,2,2,0,0,0,0,0,2,
2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,2,0,0,2,
2,0,2,2,0,2,2,0,0,0,0,0,0,4,0,2,0,0,0,2,
2,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };/*数组就是地图*/

										  ////////////////////////////函数声明/////////////////////////////////
void Init_begin();									//初始化开始界面
void Initia();										//初始化界面
void Init_score();									//记分板
void Init_loss();									//失败的界面
void Init_win();									//成功的界面
void Init_leave();									//离开的界面
void Init_copy();									//图形复制
void enemymove();					   				//敌人运动
void randdire(struct figure *sth);			        //使方向随即改变
bool judgemeet();									//相遇判断
void youmove(int k, int speed);					//对自己进行移动
void eatbean();								    	//判断是否吃小黄点
void play();										//开始游戏
int  sjs(int a, int b);								//产生a，b之间的数
void enemymeetroad(int i);							//敌人与路
void enemymeetbean(int i);					    	//敌人遇豆子
void load_image();									//加载图片
void load_music();
void Game_levels();                                 //游戏等级设定            
void beanrecreate();

/*附加功能*/

void props();//道具函数
void doublescore();
void speedup();
void enemyslow();
													/////////////////////////////////////////////////////////////////////

void Init_begin() 									//初始化开始界面
{
	floodfill(1, 1, WHITE);
	setbkcolor(WHITE);
	char init[20] = "吃豆子游戏";
	int x = M / 2 - 250, y = 100;
	setcolor(LIGHTGREEN);
	settextstyle(100, 0, "微软雅黑", 0, 0, FW_DEMIBOLD, 1, false, 0);
	outtextxy(x, y, init);
	Sleep(400);


	RECT r = { x + 50, 250, 540, 460 };
	settextstyle(24, 0, "微软雅黑");
	setcolor(GREEN);
	outtextxy(x + 50, 250, "吃豆子游戏规则：");
	settextstyle(18, 0, "微软雅黑");
	drawtext("\n\n1：游戏中，操作者可以通过方向键（上、下、左、右）或字母键（‘w’、‘s’、‘a’、‘d’）来调整方向\
		\n2：游戏中途可以按‘空格键’来暂停游戏\
		\n3：按‘ESC’推出游戏",
		&r, DT_WORDBREAK);


	settextstyle(24, 0, "微软雅黑");
	setcolor(DARKGRAY);
	outtextxy(M / 2 - 30, y + 300, "按 回车键");
	setcolor(BLACK);
	outtextxy(M / 2 - 30, y + 330, "开始游戏");
	char put;
	put = getch();
	while (put != 13)
		put = getch();
}

/*0豆子,1可以移动地方,2墙壁,3自己,4敌人,5左,6右,7上,8下,9双倍,10加速,11减速*/
void Initia() 										//初始化界面
{
	int i;
	cleardevice();
	setbkcolor(RGB(135, 206, 250));

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 20; j++)
			putimage(100 + j * 20, 100 + i * 20, &image[a[i][j]]);

	you.x = 9;            you.y = 5;					//敌人和自己的初始图纸坐标 
	enemy[0].x = 15; enemy[0].y = 2;
	enemy[1].x = 1;  enemy[1].y = 4;
	enemy[2].x = 16; enemy[2].y = 8;
	enemy[3].x = 13; enemy[3].y = 12;
	enemy[4].x = 7;  enemy[4].y = 13;

	you.x2 = you.x * 20 + 100;						//图纸坐标转化为屏幕坐标
	you.y2 = you.y * 20 + 100;
	for (i = 0; i<5; i++)
	{
		enemy[i].x2 = enemy[i].x * 20 + 100;
		enemy[i].y2 = enemy[i].y * 20 + 100;
		enemy[i].x1 = 0;	enemy[i].y1 = 1;
	}
	props();
	mciSendString(_T("play bgm form 0"), NULL, 0, NULL);
	Init_score();
}

void load_image()                                  //加载图片
{
	loadimage(&image[0], _T("res\\0.jpg"));
	loadimage(&image[1], _T("res\\1.jpg"));
	loadimage(&image[2], _T("res\\2.jpg"));
	loadimage(&image[3], _T("res\\3.jpg"));
	loadimage(&image[4], _T("res\\4.jpg"));
	loadimage(&image[5], _T("res\\5.jpg"));
	loadimage(&image[6], _T("res\\6.jpg"));
	loadimage(&image[7], _T("res\\7.jpg"));
	loadimage(&image[8], _T("res\\8.jpg"));
	loadimage(&image[9], _T("res\\9.jpg"));
	loadimage(&image[10], _T("res\\10.jpg"));
	loadimage(&image[11], _T("res\\11.jpg"));
}

void load_music()
{
	mciSendString(_T("open rec\\background.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open rec\\score.wav alias scorem"), NULL, 0, NULL);
	mciSendString(_T("open rec\\death.wav alias deathm"), NULL, 0, NULL);
	mciSendString(_T("open rec\\speedup.mp3 alias speedm"), NULL, 0, NULL);
	mciSendString(_T("open rec\\slowdown.wav alias slowm"), NULL, 0, NULL);
	mciSendString(_T("open rec\\double.wav alias doublescorem"), NULL, 0, NULL);
}

void Init_score() 							       //记分板
{
	setbkcolor(RGB(72, 12, 12));
	char infor[10];
	itoa(g_score, infor, 10);
	setcolor(CYAN);
	settextstyle(28, 0, "Arial");
	outtextxy(M - 140, 90, "当前分数为");
	setfillstyle(RGB(72, 12, 12));
	bar(M - 40, 140, M - 20, 170);
	outtextxy(M - 80, 140, infor);
}

int sjs(int a, int b)                               //产生a，b之间的数                        
{
	return a + rand() % (b - a + 1);
}

void Init_copy()                                   //图形复制
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 20; j++)
			a[i][j] = b[i][j];
}

void enemymeetbean(int i)  				    	  //敌人遇豆子
{
	int x = enemy[i].x2;
	int y = enemy[i].y2;
	for (int j = 0; j<20; j++)
	{
		if (a[enemy[i].y][enemy[i].x] == 0)
		{
			putimage(x, y, &image[0]);
			enemy[i].x2 += enemy[i].x1;
			enemy[i].y2 += enemy[i].y1;
			putimage(enemy[i].x2, enemy[i].y2, &image[4]);
		}
		else if(a[enemy[i].y][enemy[i].x] == 9)
		{
			putimage(x, y, &image[9]);
			enemy[i].x2 += enemy[i].x1;
			enemy[i].y2 += enemy[i].y1;
			putimage(enemy[i].x2, enemy[i].y2, &image[4]);
		}
		else if (a[enemy[i].y][enemy[i].x] == 10)
		{
			putimage(x, y, &image[10]);
			enemy[i].x2 += enemy[i].x1;
			enemy[i].y2 += enemy[i].y1;
			putimage(enemy[i].x2, enemy[i].y2, &image[4]);
		}
		else if (a[enemy[i].y][enemy[i].x] == 11)
		{
			putimage(x, y, &image[11]);
			enemy[i].x2 += enemy[i].x1;
			enemy[i].y2 += enemy[i].y1;
			putimage(enemy[i].x2, enemy[i].y2, &image[4]);
		}
		
	}
}

void enemymeetroad(int i)                        //敌人遇路
{
	int x = enemy[i].x2;
	int y = enemy[i].y2;
	for (int j = 0; j < 20; j++)
	{
		putimage(x, y, &image[1]);
		enemy[i].x2 += enemy[i].x1;
		enemy[i].y2 += enemy[i].y1;
		putimage(enemy[i].x2, enemy[i].y2, &image[4]);
	}
}

void enemymove() 					   	         //敌人运动
{
	int i;
	Game_levels();
	if (enemyslow_t)
	{
		Sleep(200);
		enemyslow_t--;
	}
	for (int i = 0; i < 5; i++)                     //判断是否与墙壁相遇，如果相遇，随即改变方向
	{
		if (a[enemy[i].y + enemy[i].y1][enemy[i].x + enemy[i].x1] == 2 || abs(enemy[i].y1) == abs(enemy[i].x1))
			randdire(&enemy[i]);
	}
	for (i = 0; i < 5; i++)
	{
		if ((a[enemy[i].y][enemy[i].x] == 0)||(a[enemy[i].y][enemy[i].x] == 9)|| (a[enemy[i].y][enemy[i].x] == 10)|| (a[enemy[i].y][enemy[i].x] == 11))
			enemymeetbean(i);
		else
			enemymeetroad(i);
		enemy[i].x += enemy[i].x1;
		enemy[i].y += enemy[i].y1;
		if (judgemeet()) break;
	}
	Sleep(300);
}

void Game_levels()                                  //游戏等级设定                   
{
	if (sjs(1, rank) == 1)
	{
		int t = rand() % 5;
		enemy[t].x1 = enemy[t].x - you.x > 0 ? -1 : 1;
		enemy[t].y1 = enemy[t].y - you.y > 0 ? -1 : 1;
	}
	for (int i = 0; i < 5; i++)
	{
		if (enemy[i].x == you.x && enemy[i].y + 1 == you.y)
		{
			enemy[i].x1 = 0;     enemy[i].y1 = 1;
		}
		if (enemy[i].x == you.x && enemy[i].y - 1 == you.y)
		{
			enemy[i].x1 = 0;     enemy[i].y1 = -1;
		}
		if (enemy[i].x + 1 == you.x && enemy[i].y == you.y)
		{
			enemy[i].x1 = 1;     enemy[i].y1 = 0;
		}
		if (enemy[i].x - 1 == you.x && enemy[i].y == you.y)
		{
			enemy[i].x1 = -1;    enemy[i].y1 = 1;
		}
	}
}

void props()
{
	int i;
	i = rand() % 20;
	if (i == 5)
	{
		int t = rand() % 3;
		switch (t) 
		{
		case 0:
			doublescoreshow = 1;
			doublescore();
			break;
		case 1:
			speedupshow = 1;
			speedup();
			break;
		case 2:
			enemyslowshow = 1;
			enemyslow();
			break;
		}
	}
}

void doublescore()
{
	int x, y;
	while (doublescoreshow)
	{
		x = rand() % 15;
		y = rand() % 20;
		if (a[x][y] == 0)
		{
			a[x][y] = 9;
			putimage(100 + y * 20, 100 + x * 20, &image[9]);
			doublescoreshow = 0;
		}
	}
}

void speedup()
{
	int x, y;
	while (speedupshow)
	{
		x = rand() % 15;
		y = rand() % 20;
		if (a[x][y] == 0)
		{
			a[x][y] = 10;
			putimage(100 + y * 20, 100 + x * 20, &image[10]);
			speedupshow = 0;
		}
	}
}

void enemyslow()
{
	int x, y;
	while (enemyslowshow)
	{
		x = rand() % 15;
		y = rand() % 20;
		if (a[x][y] == 0)
		{
			a[x][y] = 11;
			putimage(100 + y * 20, 100 + x * 20, &image[11]);
			enemyslowshow = 0;
		}
	}
}


void randdire(struct figure *sth)            //使方向随即改变 
{
	int b = 1;
	int x, y;
	while (b)
	{
		x = sjs(-1, 1);
		y = sjs(-1, 1);
		if (a[sth->y + y][sth->x + x] != 2 && abs(y) != abs(x))
		{
			b = 0;
			sth->x1 = x;
			sth->y1 = y;
		}
		else b = 1;
	}
}

bool judgemeet() 		                      //相遇判断
{
	for (int i = 0; i < 5; i++)
	{
		if (enemy[i].x == you.x && enemy[i].y == you.y)
		{
			mciSendString(_T("play deathm from 0"), NULL, 0, NULL);
			Sleep(500);
			mciSendString(_T("stop deathm"), NULL, 0, NULL);
			g_win = false;
			Init_loss();
			return true;
			break;
		}
	}
	return false;
}

void eatbean() 								//判断是否吃小黄点
{
	if (a[you.y][you.x] == 9) 
	{
		mciSendString(_T("play doublescorem form 0"), NULL, 0, NULL);
		doublescore_t = doublescore_time;
		Sleep(30);
		mciSendString(_T("stop doublescorem"), NULL, 0, NULL);
	}
	if (a[you.y][you.x] == 10) 
	{
		mciSendString(_T("play speedm form 0"), NULL, 0, NULL);
		speedup_t = speedup_time;
		Sleep(30);
		mciSendString(_T("stop speedm"), NULL, 0, NULL);
	}
	if (a[you.y][you.x] == 11)
	{
		mciSendString(_T("play slowm form 0"), NULL, 0, NULL);
		enemyslow_t = enemyslow_time;
		Sleep(30);
		mciSendString(_T("stop slowm"), NULL, 0, NULL);
	}
	if (doublescore_t==0)
	{
		if (a[you.y][you.x] == 0)
			{
				mciSendString(_T("play scorem form 0"), NULL, 0, NULL);
				a[you.y][you.x] = 1;
				g_sum++;
				g_score += 10;
				Init_score();
				a[you.y][you.x] = 1;
				mciSendString(_T("stop scorem"), NULL, 0, NULL);
			}
	}else 
	{
		if (a[you.y][you.x] == 0)
		{
			mciSendString(_T("play scorem form 0"), NULL, 0, NULL);
			a[you.y][you.x] = 1;
			g_sum++;
			g_score += 20;
			Init_score();
			a[you.y][you.x] = 1;
			doublescore_t--;
			mciSendString(_T("stop scorem"), NULL, 0, NULL);
		}
	}
	
}

void youmove(int k, int speed)                        //对自己进行移动
{
	int speedloop;
	switch (k)
		{
		case LEFT:
			you.x1 = -1; you.y1 = 0;you.dir = LEFT;
			break;
		case RIGHT:
			you.x1 = 1; you.y1 = 0;you.dir = RIGHT;
			break;
		case DOWN:
			you.x1 = 0; you.y1 = 1;you.dir = DOWN;
			break;
		case UP:
			you.x1 = 0; you.y1 = -1;you.dir = UP;
			break;
		}
	
	if (speedup_t)
	{
		speedloop = 2;
		speedup_t--;
	}
	else
		speedloop = 1;

	for (int k = 0;k < speedloop;k++)
	{
		if (a[you.y + you.y1][you.x + you.x1] != 2)
			{
				int x = you.x2;
				int y = you.y2;
				for (int j = 0; j < 10; j++)
				{
					putimage(x, y, &image[1]);
					you.x2 += you.x1 * 2;
					you.y2 += you.y1 * 2;
					putimage(you.x2, you.y2, &image[you.dir+4]);
				}
				you.x += you.x1;
				you.y += you.y1;
				eatbean();
			}
	}
	
}

void Init_loss()                            //失败的界面
{
	mciSendString(_T("stop bgm"), NULL, 0, NULL);
	cleardevice();
	char str[] = "Ooops, 挑战失败 >_< ";
	LOGFONT f;
	getfont(&f);
	f.lfHeight = 48;
	strcpy(f.lfFaceName, "楷体");
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	setcolor(LIGHTBLUE);
	settextstyle(&f);
	outtextxy(80, 200, str);

	char tryagain[] = "是否想继续挑战？（Y/N）";
	f.lfHeight = 28;
	settextstyle(&f);
	setcolor(BROWN);
	outtextxy(150, 300, tryagain);

	char yesorno;
	int keep = 1;
	while (keep)
	{
		yesorno = getch();
		if (yesorno == 'Y' || yesorno == 'y' || yesorno == 'N' || yesorno == 'n')
			keep = 0;
	}

	if (yesorno == 'Y' || yesorno == 'y')
	{
		g_win = true;
		g_score = 0;
		g_sum = 0;
		cleardevice();
		Init_copy();
		Initia();
	}
	else  Init_leave();
}

void Init_leave()
{
	cleardevice();
	mciSendString(_T("close bgm"), NULL, 0, NULL);
	mciSendString(_T("close scorem"), NULL, 0, NULL);
	mciSendString(_T("close deathm"), NULL, 0, NULL);
	char str[] = "下次继续努力吧!";
	LOGFONT f;
	getfont(&f);
	f.lfHeight = 48;
	strcpy(f.lfFaceName, "楷体");
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	setcolor(LIGHTGREEN);
	settextstyle(&f);
	outtextxy(150, 200, str);
	Sleep(2000);
	exit(0);
}

void Init_win()                             //成功的界面                        
{
	cleardevice();
	char str[] = "恭喜你通过了游戏！！！";
	LOGFONT f;
	getfont(&f);
	f.lfHeight = 48;
	strcpy(f.lfFaceName, "楷体");
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	setcolor(LIGHTGREEN);
	settextstyle(&f);
	outtextxy(50, 200, str);
	Sleep(4000);

	char tryagain[] = "是否想再次挑战？（Y/N）";
	f.lfHeight = 28;
	settextstyle(&f);
	setcolor(BROWN);
	outtextxy(70, 300, tryagain);

	char yesorno;
	yesorno = getch();
	if (yesorno == 'y' || yesorno == 'Y')
	{
		g_win = true;
		g_score = 0;
		g_sum = 0;
		cleardevice();
		Init_copy();
		Initia();
	}
	else  exit(0);
}

void stop()                                 //游戏暂停
{
	char key = 'a';
	char stop[] = "游戏暂停中";
	setcolor(BLACK);
	settextstyle(28, 0, "楷体");
	outtextxy(230, 50, stop);
	Sleep(100);
	while (key != ' ')
		key = getch();
	setcolor(RGB(72, 12, 12));
	outtextxy(230, 50, stop);
}

void beanrecreate()
{
	int i, j;
	int t;
	for (i = 0;i < 15;i++) {
		for (j = 0;j < 20;j++) {
			if ((a[i][j] == 1)&&(!((i==you.y)&&(j==you.x)))) {
				t = rand() % 20;//设定豆豆重新出现的概率
				if (t == 0 || t == 1)
				{
					a[i][j] = t;
					putimage(100 + j * 20, 100 + i * 20, &image[t]);
				}
				
			}
				
		}
	}
}

void play()
{
	setbkcolor(RGB(72, 12, 12));
	char key;             //获取字母
	int t;                //记录方向
	while (g_win)
	{
		props();
		beanrecreate();
		enemymove();
		judgemeet();
		while (kbhit())
		{
			key = getch();
			switch (key)
			{
			case 'a':
			case  75:      	t = LEFT;    youmove(t, speed); 			break;
			case 'd':
			case  77:   	t = RIGHT; ;  youmove(t, speed);   		break;
			case 's':
			case  80:       t = DOWN;      youmove(t, speed);    		break;
			case 'w':
			case  72:		t = UP;    	youmove(t, speed);  	 	break;
			case  27:		Init_leave();	break;
			case  32:		stop();			break;
			default:                       break;
			}
		}
		if (g_sum == winscore)        Init_win();
	}
}

void main()
{
	srand((unsigned)time(NULL));
	initgraph(M, N);
	load_image();
	load_music();
	Init_begin();
	setbkcolor(RGB(72, 12, 12));

	Initia();
	play();
	closegraph();
}