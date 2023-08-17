#pragma once
#include"Board.h"

class Player {
public:
	Player();   //构造函数

	void playerChess(Coord ai, Board*& board);   //下棋函数

	~Player();   //析构函数

protected:
	int playNow;    //现在轮到谁下棋

	IMAGE* chessImg;   //棋子图片

	Coord lastPos;    //上一回坐标

	int step;        //步数

//友元类
public:
	friend class Game;
};

class AI {
public:
	AI();      //构造函数

	void playerChess(Board*& board, int playStep);    //下棋函数

	int getScore(Board*& board,Coord index);        //获取鼠标点击，改变鼠标点击位置标记

	bool is_Ok(int x,int y);        //判断出map是否越界

	//计算五元组黑白旗的数量
	vector<int> countNum1(Board*& board, int x,int y);    
						 
	vector<int> countNum2(Board*& board, int x,int y);
						  
	vector<int> countNum3(Board*& board, int x,int y);
						 
	vector<int> countNum4(Board*& board, int x,int y);    

	//根据黑白棋的数量给每个可落子的位置评分
	int CountScore(vector<int>nums);

	//判断游戏是否有胜利者
	bool gameOver(Board*& board, Coord index,int ch);

	~AI();   //析构函数

private:

	int score;      //当前分数

	int maxScore;    //最高分

	int playNow;    //现在轮到谁下棋

	IMAGE* chessImg;   //棋子图片

	Coord lastPos;    //上一回坐标

	int step;        //步数

//友元类
public:
	friend class Game;
};