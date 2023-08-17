#pragma once
#include<vector>
#include<easyx.h>
#include<iostream>
#include<math.h>
#include<cstdlib>
#include<cstring>
#include<time.h>

using namespace std;

//��������
typedef struct Coord {
	int x;
	int y;
}Coord;

//��������
typedef struct Buffer {
	vector< vector<int> >map;    //����������    -1 AI   1 ���    0 ��
	int regretCount;   //�������
	int first;           //����
	int aiStep;         //Ai ����
	int playerStep;     //��� ����
}Buffer;

class Board
{
public:
	Board();    //���캯��

	Coord getMouse(Coord ai,Coord player);   //��ȡ�����λ��

	Coord convertCoord(Coord index);    //ת������

	void putimagePNG(int x, int y, IMAGE* picture);   //���ͼƬ

	int checkClick(Coord& index);    //��������

	int checkStep(Coord& index);       //�жϵ���Ƿ�Ϸ�

	void showChess();              //��ʾ�洢������

	void regretChess(Coord ai, Coord player);   //����

	void buttonsMusic();    //��ť����

	~Board();     //��������

private:
	Buffer data;    //��������
	 
	IMAGE* whiteImg;   //����ͼƬָ��

	IMAGE* blackImg;   //����ͼƬָ��

	IMAGE* boardImg;    //����ָ��
	 
	int mapSize;       //���̴�С
 
	int chessSize;     //����ͼƬ��С

	int leftMargin;    //������߽�

	int topMargin;    //�����ұ߽�

	ExMessage mouse;    //���ָ��

	int flag;           //���������λ�ñ��

	int dist;         //����������ĵ�ľ���

//��Ԫ��
public:
	friend class Game; 
	friend class AI;
	friend class Player;
};

