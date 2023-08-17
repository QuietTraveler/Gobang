#pragma once
#include<fstream>
#include"Chess.h"

#define Width 1185   //���
#define Height 880    //�߶�

using namespace std;

class Game
{
public:
	Game();   //���캯��

	void menu();   //�˵�����

	void Init();    //��ʼ������

	void play();          //��Ϸ����

	void playMusic();     //��������

	void Storage();     //�洢����

	int gameOver();       //�ж���Ϸ�Ƿ���ʤ����

	void endShow();       //���չʾ

	void overMusic();     //��������

	bool gameEqual();     //�ж���Ϸ�Ƿ�ƽ��

	~Game();            //��������

private: 
	IMAGE* boardImg;       //��Ϸ����ͼƬ

	IMAGE* exitImg;         //�˳���ťͼƬ

	ifstream* setinFile;       //��Ϸ�洢�ļ�����ָ��

	ofstream* setoutFile;      //��Ϸ�洢�ļ����ָ��
	 
	const char* setFileNume;   //��Ϸ�洢�ļ���

	bool reStart;            //�Ƿ����¿�ʼ��Ϸ

	int first;               // ����
	 
	AI* aiChess;             //AI 

	Player* myChess;             //���

	Board* board;             //����

	int win;                //˭Ӯ

private:
	static int width;        //���ڿ��

	static int height;        //���ڸ߶�

};

