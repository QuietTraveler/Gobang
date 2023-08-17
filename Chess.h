#pragma once
#include"Board.h"

class Player {
public:
	Player();   //���캯��

	void playerChess(Coord ai, Board*& board);   //���庯��

	~Player();   //��������

protected:
	int playNow;    //�����ֵ�˭����

	IMAGE* chessImg;   //����ͼƬ

	Coord lastPos;    //��һ������

	int step;        //����

//��Ԫ��
public:
	friend class Game;
};

class AI {
public:
	AI();      //���캯��

	void playerChess(Board*& board, int playStep);    //���庯��

	int getScore(Board*& board,Coord index);        //��ȡ��������ı������λ�ñ��

	bool is_Ok(int x,int y);        //�жϳ�map�Ƿ�Խ��

	//������Ԫ��ڰ��������
	vector<int> countNum1(Board*& board, int x,int y);    
						 
	vector<int> countNum2(Board*& board, int x,int y);
						  
	vector<int> countNum3(Board*& board, int x,int y);
						 
	vector<int> countNum4(Board*& board, int x,int y);    

	//���ݺڰ����������ÿ�������ӵ�λ������
	int CountScore(vector<int>nums);

	//�ж���Ϸ�Ƿ���ʤ����
	bool gameOver(Board*& board, Coord index,int ch);

	~AI();   //��������

private:

	int score;      //��ǰ����

	int maxScore;    //��߷�

	int playNow;    //�����ֵ�˭����

	IMAGE* chessImg;   //����ͼƬ

	Coord lastPos;    //��һ������

	int step;        //����

//��Ԫ��
public:
	friend class Game;
};