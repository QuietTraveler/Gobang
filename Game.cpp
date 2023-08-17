#include "Game.h"
#include "cereal/archives/binary.hpp"     //�����������л���
#include "cereal/types/vector.hpp"        //����⵼�룬�ο���ĿĿ¼�µ�cereal��
#include<Windows.h>
#include<mmsystem.h>                 
#pragma comment(lib,"Winmm.lib")

//��̬��Ա��ֵ
int Game::width = Width;
int Game::height = Height;

//���л�����
template <class Archive>
void serialize(Archive& archive, Buffer& data) {
	archive(data.map, data.regretCount, data.first, data.aiStep, data.playerStep);
}

Game::Game()
{
	this->aiChess = new AI;         //ΪAIָ�����ռ�
	this->myChess = new Player;     //Ϊ���ָ�����ռ�
	this->board = new Board;     //Ϊ����ָ�����ռ�
	this->boardImg = new IMAGE;   //Ϊ��Ϸ����ͼƬָ�����ռ�
	this->exitImg = new IMAGE;   //Ϊ��Ϸ�˳�ͼƬָ�����ռ�
	this->setinFile = new ifstream;  //Ϊ��Ϸ�ļ�����ָ�����ռ�
	this->setoutFile = new ofstream;  //Ϊ��Ϸ�ļ����ָ�����ռ�
	this->reStart = false;        //�Ƿ����¿�ʼ��Ϸ
	this->first = -1;            //����
	this->win = 0;               //��Ϸ����
	this->setFileNume = "setfile.txt";    
	loadimage(this->boardImg, "picture/����.png");   //������Ϸ��Ϸ����ͼƬ
	loadimage(this->exitImg, "picture/�˳�.png");   //�����˳���ťͼƬ
	this->board->boardImg = this->boardImg;         //Ϊboard���ͼƬָ�븳ֵ

}

void Game::menu()
{
	//���ٻ滭����
	initgraph(width, height);

	//���ؿ�ʼ����
	loadimage(NULL, "picture/��ʼ����.png");
	board->putimagePNG(140, 30, this->exitImg);

	//�ж������
	ExMessage msg;
	while ( 1 )
	{

		if ( peekmessage(&msg, EX_MOUSE) )
		{
			if ( msg.message == WM_LBUTTONDOWN )
			{
				//���¿�ʼ��Ϸ
				if ( msg.x > 427 && msg.x < 746 && msg.y> 650 && msg.y < 722 )
				{
					this->reStart = true;
					this->board->buttonsMusic();
					break;
				}
				//��ȡ��Ϸ�浵
				else if ( msg.x > 427 && msg.x < 746 && msg.y>764 && msg.y < 837 )
				{
					this->reStart = false;
					this->board->buttonsMusic();
					break;
				}
				//�˳���Ϸ
				else if ( msg.x > 140 && msg.x < 234 && msg.y>30 && msg.y < 289 )
				{
					this->board->buttonsMusic();
					closegraph();
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}

	return;
}

//��Ϸ��ֵ��ʼ��
void Game::Init()
{
	this->first = -1;
	this->win = 0;
	this->board->data.regretCount = 3;
	this->board->data.first = -1;
	this->board->data.aiStep = 0;
	this->board->data.playerStep = 0;
	this->board->flag = 1;
	this->aiChess->step = 0;
	this->myChess->step = 0;

	putimage(0, 0, this->boardImg);

	for ( int i = 0; i < board->mapSize; i++ )
	{
		for ( int j = 0; j < board->mapSize; j++ )
		{
			this->board->data.map[i][j] = 0;
		}
	}

	if ( this->first == -1 )
	{
		this->aiChess->chessImg = this->board->blackImg;
		this->myChess->chessImg = this->board->whiteImg;
		this->aiChess->playNow = true;
		this->myChess->playNow = false;
	}
	else
	{
		this->aiChess->chessImg = this->board->whiteImg;
		this->myChess->chessImg = this->board->blackImg;
		this->aiChess->playNow = false;
		this->myChess->playNow = true;
	}

	//�ж��Ƿ��ȡ�ļ��ڵ����ݣ���cereal��ʵ��
	if ( !this->reStart )
	{

		setinFile->open(this->setFileNume, ios::in | ios::binary);
		this->setinFile->clear();
		this->setinFile->seekg(0);
		cereal::BinaryInputArchive archive(*( this->setinFile ));
		archive(this->board->data);

		this->setinFile->close();

	    this->first = this->board->data.first;
		this->aiChess->step = this->board->data.aiStep;
		this->myChess->step = this->board->data.playerStep;
		this->aiChess->playNow = false;
		this->myChess->playNow = true;

		this->board->showChess();
	}


}

void Game::play()
{
	while (1)
	{
		//���������AI����
		if ( this->aiChess->playNow )
		{
			//AI����
			this->aiChess->playerChess(this->board,this->myChess->step);

			//��������
			this->playMusic();

			//�жϵ�ǰ��Ϸ���
			if ( this->gameOver() || this->gameEqual() )
			{
				this->endShow();
				break;
			}

			//�ı����
			this->aiChess->playNow = false;
			this->myChess->playNow = true;
		}

		//����������������
		if(this->myChess->playNow)
		{
			//�������
			this->myChess->playerChess(this->aiChess->lastPos,this->board);
			
			//������ѡ������
			if ( this->board->flag == 1 )
			this->playMusic();    //��������

			//���¿�ʼ��Ϸ
			if ( this->board->flag == 2 )
			{
				this->reStart = true;    
				this->Init();    //��ʼ����Ϸ����
				continue;
			}
			//�˳���Ϸ��������Ϸ����
			else if ( !this->board->flag )
			{
				this->Storage();
				break;
			}
			//�жϵ�ǰ��Ϸ���
			if ( this->gameOver() || this->gameEqual())
			{
				this->endShow();
				break;
			}
			//�ı����
			this->myChess->playNow = false;
			this->aiChess->playNow = true;
		}

	}
}

//������������
void Game::playMusic()
{
	mciSendString("open music/down.wav", 0, 0, 0);
	mciSendString("play music/down.wav wait", 0, 0, 0);
	mciSendString("close music/down.wav", 0, 0, 0);
}

//�洢��Ϸ����
void Game::Storage()
{
	this->board->data.first = this->first;
	this->board->data.aiStep = this->aiChess->step;
	this->board->data.playerStep = this->myChess->step;

	this->setoutFile->open(this->setFileNume, ios::binary | ios::out);
	this->setoutFile->clear();
	this->setoutFile->seekp(0);
	//cereal�����
	cereal::BinaryOutputArchive archive(*(this->setoutFile));
	archive(this->board->data);
    
	this->setoutFile->close();
}

//�ж���Ϸ�Ƿ����
int Game::gameOver()
{
	Coord index;
	if ( this->aiChess->playNow )
	{
		index.x = this->aiChess->lastPos.x;
		index.y = this->aiChess->lastPos.y;
		this->win = -1;
		return this->aiChess->gameOver(this->board,index,0);
	}
	else if(this->myChess->playNow)
	{
		index.x = this->myChess->lastPos.x;
		index.y = this->myChess->lastPos.y;
		this->win = 1;
		return this->aiChess->gameOver(this->board,index,1);
	}

	return 0;
}

//��Ϸ����չʾ
void Game::endShow()
{
	if ( this->win == 1 )
	{
		loadimage(0, "picture/ʤ��.jpg",Width,Height);
		this->overMusic();
		MessageBox(0, ("��ϲ��ʤ�����ٽ���������ʤ׷��Ŷ ����"), ("YES"), 0);
	}
	else if (this->win == -1)
	{
		loadimage(0, "picture/ʧ��.jpg", Width, Height);
		this->overMusic();
		MessageBox(0, ("��ʧ���ˣ���Ҫ�����´������� ����"), ("YES"), 0);

	}
	else if (this->win == 2)
	{
		MessageBox(0, ( "��Ȼ���ƽ�ֿ����ƾ�����ѽ ����" ), ( "YES" ), 0);
	}

}

//��Ϸ��������
void Game::overMusic()
{
	if(this->win==1)
	{
		mciSendString("open music/win.wav", 0, 0, 0);
		mciSendString("play music/win.wav wait", 0, 0, 0);
	    mciSendString("close music/win.wav", 0, 0, 0);
	}
	else if(this->win=-1)
	{
		mciSendString("open music/defet.mp3", 0, 0, 0);
		mciSendString("play music/defet.mp3 wait", 0, 0, 0);
		mciSendString("close music/defet.mp3", 0, 0, 0);
	}
	else if ( this->win == 2 )
	{
		mciSendString("open music/����.mp3", 0, 0, 0);
		mciSendString("play music/����.mp3 wait", 0, 0, 0);
		mciSendString("close music/����.mp3", 0, 0, 0);
	}
}

//��Ϸ�Ƿ�ƽ��
bool Game::gameEqual()
{
	for ( int i = 0; i < board->mapSize; i++ )
	{
		for ( int j = 0; j < board->mapSize; j++ )
		{
			if ( board->data.map[i][j] == 0 )
				return false;
		}
	}

	this->win = 2;
	return true;
}

//�ͷ��ڴ�
Game::~Game()
{
	delete this->setinFile;
	delete this->setoutFile;
	delete this->aiChess;
	delete this->myChess;
	delete this->board;
	delete this->exitImg;
	delete this->boardImg;
}
