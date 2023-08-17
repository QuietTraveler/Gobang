#pragma once
#include<vector>
#include<easyx.h>
#include<iostream>
#include<math.h>
#include<cstdlib>
#include<cstring>
#include<time.h>

using namespace std;

//棋子坐标
typedef struct Coord {
	int x;
	int y;
}Coord;

//棋盘数据
typedef struct Buffer {
	vector< vector<int> >map;    //棋盘内数据    -1 AI   1 玩家    0 无
	int regretCount;   //悔棋次数
	int first;           //先手
	int aiStep;         //Ai 步数
	int playerStep;     //玩家 步数
}Buffer;

class Board
{
public:
	Board();    //构造函数

	Coord getMouse(Coord ai,Coord player);   //获取鼠标点击位置

	Coord convertCoord(Coord index);    //转换坐标

	void putimagePNG(int x, int y, IMAGE* picture);   //输出图片

	int checkClick(Coord& index);    //检查鼠标点击

	int checkStep(Coord& index);       //判断点击是否合法

	void showChess();              //显示存储的棋盘

	void regretChess(Coord ai, Coord player);   //悔棋

	void buttonsMusic();    //按钮音乐

	~Board();     //析构函数

private:
	Buffer data;    //棋盘数据
	 
	IMAGE* whiteImg;   //白棋图片指针

	IMAGE* blackImg;   //黑棋图片指针

	IMAGE* boardImg;    //棋盘指针
	 
	int mapSize;       //棋盘大小
 
	int chessSize;     //棋子图片大小

	int leftMargin;    //棋盘左边界

	int topMargin;    //棋盘右边界

	ExMessage mouse;    //鼠标指针

	int flag;           //棋盘鼠标点击位置标记

	int dist;         //鼠标点击与中心点的距离

//友元类
public:
	friend class Game; 
	friend class AI;
	friend class Player;
};

