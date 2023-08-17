#include "Game.h"
#include "cereal/archives/binary.hpp"     //第三方的序列化库
#include "cereal/types/vector.hpp"        //需额外导入，参考项目目录下的cereal库
#include<Windows.h>
#include<mmsystem.h>                 
#pragma comment(lib,"Winmm.lib")

//静态成员赋值
int Game::width = Width;
int Game::height = Height;

//序列化函数
template <class Archive>
void serialize(Archive& archive, Buffer& data) {
	archive(data.map, data.regretCount, data.first, data.aiStep, data.playerStep);
}

Game::Game()
{
	this->aiChess = new AI;         //为AI指针分配空间
	this->myChess = new Player;     //为玩家指针分配空间
	this->board = new Board;     //为棋盘指针分配空间
	this->boardImg = new IMAGE;   //为游戏背景图片指针分配空间
	this->exitImg = new IMAGE;   //为游戏退出图片指针分配空间
	this->setinFile = new ifstream;  //为游戏文件输入指针分配空间
	this->setoutFile = new ofstream;  //为游戏文件输出指针分配空间
	this->reStart = false;        //是否重新开始游戏
	this->first = -1;            //先手
	this->win = 0;               //游戏进行
	this->setFileNume = "setfile.txt";    
	loadimage(this->boardImg, "picture/棋盘.png");   //加载游戏游戏背景图片
	loadimage(this->exitImg, "picture/退出.png");   //加载退出按钮图片
	this->board->boardImg = this->boardImg;         //为board类的图片指针赋值

}

void Game::menu()
{
	//开辟绘画窗口
	initgraph(width, height);

	//加载开始界面
	loadimage(NULL, "picture/开始界面.png");
	board->putimagePNG(140, 30, this->exitImg);

	//判断鼠标点击
	ExMessage msg;
	while ( 1 )
	{

		if ( peekmessage(&msg, EX_MOUSE) )
		{
			if ( msg.message == WM_LBUTTONDOWN )
			{
				//重新开始游戏
				if ( msg.x > 427 && msg.x < 746 && msg.y> 650 && msg.y < 722 )
				{
					this->reStart = true;
					this->board->buttonsMusic();
					break;
				}
				//读取游戏存档
				else if ( msg.x > 427 && msg.x < 746 && msg.y>764 && msg.y < 837 )
				{
					this->reStart = false;
					this->board->buttonsMusic();
					break;
				}
				//退出游戏
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

//游戏数值初始化
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

	//判断是否读取文件内的数据，用cereal库实现
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
		//如果现在是AI下棋
		if ( this->aiChess->playNow )
		{
			//AI下棋
			this->aiChess->playerChess(this->board,this->myChess->step);

			//下棋音乐
			this->playMusic();

			//判断当前游戏结果
			if ( this->gameOver() || this->gameEqual() )
			{
				this->endShow();
				break;
			}

			//改变参数
			this->aiChess->playNow = false;
			this->myChess->playNow = true;
		}

		//如果现在是玩家下棋
		if(this->myChess->playNow)
		{
			//玩家下棋
			this->myChess->playerChess(this->aiChess->lastPos,this->board);
			
			//如果玩家选择下棋
			if ( this->board->flag == 1 )
			this->playMusic();    //播放音乐

			//重新开始游戏
			if ( this->board->flag == 2 )
			{
				this->reStart = true;    
				this->Init();    //初始化游戏数据
				continue;
			}
			//退出游戏并保存游戏数据
			else if ( !this->board->flag )
			{
				this->Storage();
				break;
			}
			//判断当前游戏结果
			if ( this->gameOver() || this->gameEqual())
			{
				this->endShow();
				break;
			}
			//改变参数
			this->myChess->playNow = false;
			this->aiChess->playNow = true;
		}

	}
}

//播放下棋音乐
void Game::playMusic()
{
	mciSendString("open music/down.wav", 0, 0, 0);
	mciSendString("play music/down.wav wait", 0, 0, 0);
	mciSendString("close music/down.wav", 0, 0, 0);
}

//存储游戏数据
void Game::Storage()
{
	this->board->data.first = this->first;
	this->board->data.aiStep = this->aiChess->step;
	this->board->data.playerStep = this->myChess->step;

	this->setoutFile->open(this->setFileNume, ios::binary | ios::out);
	this->setoutFile->clear();
	this->setoutFile->seekp(0);
	//cereal库操作
	cereal::BinaryOutputArchive archive(*(this->setoutFile));
	archive(this->board->data);
    
	this->setoutFile->close();
}

//判断游戏是否结束
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

//游戏结束展示
void Game::endShow()
{
	if ( this->win == 1 )
	{
		loadimage(0, "picture/胜利.jpg",Width,Height);
		this->overMusic();
		MessageBox(0, ("恭喜您胜利！再接再厉，乘胜追击哦 ！！"), ("YES"), 0);
	}
	else if (this->win == -1)
	{
		loadimage(0, "picture/失败.jpg", Width, Height);
		this->overMusic();
		MessageBox(0, ("您失败了，不要灰心下次再来吧 ！！"), ("YES"), 0);

	}
	else if (this->win == 2)
	{
		MessageBox(0, ( "居然打成平局咯，势均力敌呀 ！！" ), ( "YES" ), 0);
	}

}

//游戏结束音乐
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
		mciSendString("open music/不错.mp3", 0, 0, 0);
		mciSendString("play music/不错.mp3 wait", 0, 0, 0);
		mciSendString("close music/不错.mp3", 0, 0, 0);
	}
}

//游戏是否平局
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

//释放内存
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
