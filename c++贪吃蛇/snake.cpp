// snake.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "Graphics.h"
#include "stdlib.h"
#include "time.h"


#define UP 296
#define RIGHT 301
#define DOWN 304
#define LEFT 299
#define ESC 27
#define MAXSTAR 400


struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};


struct{
	struct{
		int x;
		int y;
	}body[30];
	int node;
	int derection;
	int life;
}snake;


struct{
	int x;
	int y;
	int life;
	int color;
}food;

struct{
	char name[5];
	int score;
}rank[11];

void initsnake();
void showsnake();
void showfood();
void showscore();
void game(int,int,int,int,int,int);
void eatfood();
int judgedie();
void gameover();
int startmenu();
void list();
int savedata();

int score,speed;
STAR star[MAXSTAR];

int main(int argc, char* argv[])
{
	int n,a,b,c,d,e,f;
	initgraph(640,480);
	setbkcolor(BLACK);
	while(1){
		n=startmenu();
		if(n==1){
			FILE *fp;
			fp=fopen("D:\\data.txt","r");
			if(fp==NULL)
			{
				printf("暂无记录\n");
				break;
			}
			else{
				fscanf(fp,"%d%d%d%d%d%d",&a,&b,&c,&d,&e,&f);
				fclose(fp);
				remove("D:\\data.txt");
				game(a,b,c,d,e,f);
				gameover();
			}
		}
		if(n==2){
			game(100,0,300,230,2,2);
			gameover();
		}
		if(n==3){
			list();
		}
		if(n==4)	return 0;
	}
}


void initsnake(int a,int b,int c,int d)
{
	int i;
	snake.body[0].x=a;
	snake.body[0].y=b;
	snake.node=c;
	snake.derection=d;
	snake.life=1;
	for(i=1;i<snake.node;i++)
	{
		snake.body[i].x=snake.body[i-1].x+10;
		snake.body[i].y=snake.body[i-1].y;
	}
}


void showsnake()
{
	int i;
	setfillcolor(YELLOW);setfillstyle(0);
	switch(snake.derection){
		case 1:solidellipse(snake.body[0].x,snake.body[0].y,snake.body[0].x+10,snake.body[0].y+20);break;
		case 2:solidellipse(snake.body[0].x,snake.body[0].y,snake.body[0].x+20,snake.body[0].y+10);break;
		case 3:solidellipse(snake.body[0].x+10,snake.body[0].y,snake.body[0].x-10,snake.body[0].y+10);break;
		case 4:solidellipse(snake.body[0].x,snake.body[0].y+10,snake.body[0].x+10,snake.body[0].y-10);break;
	}
	for(i=1;i<snake.node;i++)
	{
		solidroundrect(snake.body[i].x,snake.body[i].y,snake.body[i].x+10,snake.body[i].y+10,5,5);
	}
}


void showfood()
{
	srand((unsigned)time(NULL));
	if(food.life==0)
	{
		food.color=rand()%101;
		food.x=(rand()%54+5)*10+5;
		food.y=(rand()%38+5)*10+5;
		setcolor(MAGENTA);setlinestyle(0,1);
		circle(food.x,food.y,5);
		if(food.color<=10){
			setfillcolor(LIGHTRED);setfillstyle(0);
			floodfill(food.x,food.y,MAGENTA);
		}
		else if(food.color<=20){
			setfillcolor(LIGHTGREEN);setfillstyle(0);
			floodfill(food.x,food.y,MAGENTA);
		}
		else if(food.color<=25){
			setfillcolor(WHITE);setfillstyle(0);
			floodfill(food.x,food.y,MAGENTA);
		}
		else{
			setfillcolor(YELLOW);setfillstyle(0);
			floodfill(food.x,food.y,MAGENTA);
		}
		food.life=1;
	}
}


void showscore()
{
	char s[20];
	setcolor(WHITE);
	settextstyle(20, 0, _T("宋体"));
	sprintf(s,"score:%d",score);
	outtextxy(40,10,s);
}


void game(int a,int b,int c,int d,int e,int f)
{
	int key,i,n;
	speed=a;
	score=b;
	cleardevice();
	setcolor(LIGHTGREEN);
	setlinestyle(0,10);
	rectangle(40,40,600,440);
	initsnake(c,d,e,f);
	showsnake();
	food.life=0;
	showfood();
	showscore();
	while(1)
	{
		Sleep(speed);
		if(judgedie()==0) return;
		setcolor(BLACK);setlinestyle(0,3);
		setfillcolor(BLACK);setfillstyle(0);
		solidrectangle(snake.body[snake.node-1].x,snake.body[snake.node-1].y,
			snake.body[snake.node-1].x+10,snake.body[snake.node-1].y+10);
		if(kbhit()!=0){ 
			if((key=getch())==224)	key=key+getch();
			if(key==UP&&snake.derection!=4)				snake.derection=1;		  
			if(key==LEFT&&snake.derection!=3)			snake.derection=2;
			if(key==RIGHT&&snake.derection!=2)			snake.derection=3;
			if(key==DOWN&&snake.derection!=1)			snake.derection=4;
			if(key==ESC){
				n=savedata();
				if(n==1){
					FILE *fp;
					fp=fopen("D:\\data.txt","w");
					fprintf(fp,"%5d%5d%5d%5d%5d%5d\n",speed,score,snake.body[0].x,snake.body[0].y,
						snake.node,snake.derection);
					fclose(fp);
					exit(0);
				}
				if(n==2){
					cleardevice();
					showscore();
					setcolor(MAGENTA);setlinestyle(0,1);
					circle(food.x,food.y,5);
					setfillcolor(YELLOW);setfillstyle(0);
					floodfill(food.x,food.y,MAGENTA);
					setcolor(LIGHTGREEN);
					setlinestyle(0,10);
					rectangle(40,40,600,440);
				}
				if(n==3) break;
			}
		}
		for(i=snake.node;i>=1;i--){
			snake.body[i].x=snake.body[i-1].x;
			snake.body[i].y=snake.body[i-1].y;
		}
		if(snake.derection==1)	snake.body[0].y-=10;
		if(snake.derection==2)	snake.body[0].x-=10;
		if(snake.derection==3)	snake.body[0].x+=10;
		if(snake.derection==4)	snake.body[0].y+=10;
		showsnake();
		eatfood();
	}
}


void eatfood()
{
	if(snake.body[0].x+5==food.x&&snake.body[0].y+5==food.y){
		if(food.color<=10)
			speed-=10;
		else if(food.color<=20)
			score+=10;
		else if(food.color<=25)
			speed+=10;
		score+=10;
		snake.node+=1;
		speed-=10;
		snake.body[snake.node-1].x=-10;
		snake.body[snake.node-1].y=-10;
		showscore();
		food.life=0;
		showfood();
	}
}


int judgedie()
{
	int i;
	if(snake.body[0].x<50||snake.body[0].x>580||snake.body[0].y<50||snake.body[0].y>420)
		return 0;
	for(i=3;i<snake.node;i++)
		if(snake.body[0].x==snake.body[i].x&&snake.body[0].y==snake.body[i].y)
			return 0;
	return 1;
}


void gameover()
{
	cleardevice();
	setcolor(LIGHTRED);
	settextstyle(80, 0, _T("华文彩云"));
	outtextxy(150,200,"游戏结束!");
	char s[10];
	InputBox(s,10,"请输入姓名:");
	sscanf(s,"%s",rank[10].name);
	rank[10].score=score;
	FILE *fp;
	int i,b,j;
	char a[10];
	fp=fopen("D:\\list.txt","r");
	if(fp!=NULL){
	for(i=0;i<10;i++)
		fscanf(fp,"%s%d",rank[i].name,&rank[i].score);
	fclose(fp);
	fp=fopen("D:\\list.txt","w");
	for(i=0;i<10;i++)
		if(rank[10].score>rank[i].score)
		{
			strcpy(a,rank[10].name);
			b=rank[10].score;
			for(j=9;j>=i;j--)
			{
				rank[j+1].score=rank[j].score;
				strcpy(rank[j+1].name,rank[j].name);
			}
			rank[i].score=b;
			strcpy(rank[i].name,a);
			break;
		}
	for(i=0;i<10;i++)
		fprintf(fp,"%10s%5d\n",rank[i].name,rank[i].score);
	fclose(fp);
	}
	else{
		fp=fopen("D:\\list.txt","w");
		fprintf(fp,"%10s%5d\n",rank[10].name,rank[10].score);
		fclose(fp);
	}
	getch();
}


int startmenu()
{
	int key,n=401;
	cleardevice();
	srand((unsigned)time(NULL));
	for(int i = 0; i < MAXSTAR; i++)
	{
		star[i].x = 0;
		star[i].y = rand() % 480;
		star[i].step = (rand() % 5000) / 1000.0 + 1;
		star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);
		star[i].color = RGB(star[i].color, star[i].color, star[i].color);
		star[i].x = rand() % 640;
	}
	while(1){
		for(int i = 0; i < MAXSTAR; i++)
		{
			putpixel((int)star[i].x, star[i].y, 0);
			star[i].x += star[i].step;
			if (star[i].x > 640)
			{
				star[i].x = 0;
				star[i].y = rand() % 480;
				star[i].step = (rand() % 5000) / 1000.0 + 1;
				star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);
				star[i].color = RGB(star[i].color, star[i].color, star[i].color);
			}
			putpixel((int)star[i].x, star[i].y, star[i].color);
		}
		setcolor(LIGHTRED);
		settextstyle(50, 0, _T("华文彩云"));
		outtextxy(210,120," 继续游戏!");
		outtextxy(210,190," 开始游戏!");
		outtextxy(210,260,"   排行榜!");
		outtextxy(210,330,"     退出!");
		if(kbhit()!=0){ 
			if((key=getch())==224)	key=key+getch();
			if(key==UP){
				n--;
				if(n%5==0)	n--;
			}
			if(key==DOWN){
				n++;
				if(n%5==0)	n++;
			}
			if(key==13)	return n%5;
		}
		if(n%5==1){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,190,225,230,3.0,3.3);solidpie(425,190,475,230,-0.15,0.15);
			solidpie(175,330,225,370,3.0,3.3);solidpie(425,330,475,370,-0.15,0.15);
		}
		if(n%5==2){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,190,225,230,3.0,3.3);solidpie(425,190,475,230,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
			solidpie(175,260,225,300,3.0,3.3);solidpie(425,260,475,300,-0.15,0.15);
		}
		if(n%5==3){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,260,225,300,3.0,3.3);solidpie(425,260,475,300,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,190,225,230,3.0,3.3);solidpie(425,190,475,230,-0.15,0.15);
			solidpie(175,330,225,370,3.0,3.3);solidpie(425,330,475,370,-0.15,0.15);
		}
		if(n%5==4){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,330,225,370,3.0,3.3);solidpie(425,330,475,370,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,260,225,300,3.0,3.3);solidpie(425,260,475,300,-0.15,0.15);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
		}
		Sleep(20);
	}
	return n;
}


void list()
{
	cleardevice();
	FILE *fp;
	int i;
	char num[10][10],n[2];
	fp=fopen("D:\\list.txt","r");
	if(fp==NULL)
	{
		printf("暂无记录\n");
		exit(0);
	}
	for(i=0;i<10;i++)
		fscanf(fp,"%s%d",rank[i].name,&rank[i].score);
	fclose(fp);
	setcolor(LIGHTRED);
	settextstyle(30, 0, _T("宋体"));
	for(i=0;i<10;i++){
		itoa(rank[i].score,num[i],10);
		itoa(i+1,n,10);
		outtextxy(100,80+i*30,"第  名：");
		outtextxy(130,80+i*30,n);
		outtextxy(210,80+i*30,rank[i].name);
		outtextxy(400,80+i*30,"得分：");
		outtextxy(500,80+i*30,num[i]);
	}
	getch();
}

int savedata()
{
	cleardevice();
	int key,n=401;
	setcolor(LIGHTRED);
	settextstyle(50, 0, _T("华文彩云"));
	outtextxy(210,120," 保存游戏!");
	outtextxy(210,220," 继续游戏!");
	outtextxy(210,320,"     退出!");
	while(1){
		if(kbhit()!=0){ 
			if((key=getch())==224)	key=key+getch();
			if(key==UP){
				n--;
				if(n%4==0)	n--;
			}
			if(key==DOWN){
				n++;
				if(n%4==0)	n++;
			}
			if(key==13)	return n%4;
		}
		if(n%4==1){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,220,225,260,3.0,3.3);solidpie(425,220,475,260,-0.15,0.15);
			solidpie(175,320,225,360,3.0,3.3);solidpie(425,320,475,360,-0.15,0.15);
		}
		if(n%4==2){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,220,225,260,3.0,3.3);solidpie(425,220,475,260,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
			solidpie(175,320,225,360,3.0,3.3);solidpie(425,320,475,360,-0.15,0.15);
		}
		if(n%4==3){
			setfillcolor(YELLOW);setfillstyle(0);
			solidpie(175,320,225,360,3.0,3.3);solidpie(425,320,475,360,-0.15,0.15);
			setfillcolor(BLACK);setfillstyle(0);
			solidpie(175,120,225,160,3.0,3.3);solidpie(425,120,475,160,-0.15,0.15);
			solidpie(175,220,225,260,3.0,3.3);solidpie(425,220,475,260,-0.15,0.15);
		}
	}
}

