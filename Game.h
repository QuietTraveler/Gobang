#pragma once
#include<fstream>
#include"Chess.h"

#define Width 1185   //宽度
#define Height 880    //高度

using namespace std;

class Game
{
public:
	Game();   //构造函数

	void menu();   //菜单函数

	void Init();    //初始化函数

	void play();          //游戏函数

	void playMusic();     //下棋音乐

	void Storage();     //存储数据

	int gameOver();       //判断游戏是否有胜利者

	void endShow();       //结局展示

	void overMusic();     //结束音乐

	bool gameEqual();     //判断游戏是否平局

	~Game();            //析构函数

private: 
	IMAGE* boardImg;       //游戏背景图片

	IMAGE* exitImg;         //退出按钮图片

	ifstream* setinFile;       //游戏存储文件输入指针

	ofstream* setoutFile;      //游戏存储文件输出指针
	 
	const char* setFileNume;   //游戏存储文件名

	bool reStart;            //是否重新开始游戏

	int first;               // 先手
	 
	AI* aiChess;             //AI 

	Player* myChess;             //玩家

	Board* board;             //棋盘

	int win;                //谁赢

private:
	static int width;        //窗口宽度

	static int height;        //窗口高度

};

