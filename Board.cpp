#include "Board.h"

enum {
	EXIT,   //�˳�
	CLICK,   //��ȷ���
	RESTART,    //���¿�ʼ
	REGRET,      //����
	FAIL      //������
};

//��ʼ������
Board::Board()
{
	this->blackImg = new IMAGE;
	this->whiteImg = new IMAGE;
	this->flag = 1;
	this->dist = 10;

	loadimage(this->blackImg, "picture/black.png");
	loadimage(this->whiteImg, "picture/white.png");

	this->chessSize = 55;
	this->mapSize = 15;

	this->leftMargin = 362;
	this->topMargin = 45;

	this->data.regretCount = 3;
	this->data.first = -1;
	this->data.aiStep = 0;
	this->data.playerStep = 0;
	this->data.map.resize(this->mapSize);

	for ( int i = 0; i < mapSize; i++ )
	{
		this->data.map[i].resize(this->mapSize);
	}

	this->data.first = -1;
}


//��ȡ���λ��
Coord Board::getMouse(Coord ai, Coord player)
{
	Coord index;
	int flag = 1;
	while ( 1 )
	{
		if ( peekmessage(&( this->mouse ), EX_MOUSE) && this->mouse.message == WM_LBUTTONDOWN )
		{

			switch ( this->checkClick(index) )
			{
				case EXIT:
					this->flag = 0;
					this->buttonsMusic();   //��ť����
					break;
				case CLICK:
					this->flag = 1;
					return index;
					break;
				case RESTART:
					this->flag = 2;
					this->buttonsMusic();
					break;
				case REGRET:
					this->flag = 3;
					this->buttonsMusic();
					if(this->data.regretCount && flag)
					{
						this->regretChess(ai, player);   //����
						flag = 0;
					}
					break;
				default:
					this->flag = 4;
					break;
			}
		}
		else
			continue;

		if ( this->flag < 3 )
			return { ( 0,0 ) };
		else
			continue;
	}
}

//ת������
Coord Board::convertCoord(Coord index)
{
	Coord temp;
	//map���±�תΪ���ص�����
	if ( index.x >= 0 && index.x < this->mapSize )
	{
		temp.x = ( this->leftMargin + index.x * this->chessSize ) - this->chessSize * 0.5;
		temp.y = ( this->topMargin + index.y * this->chessSize ) - this->chessSize * 0.5 + 6;

	}
	//���ص�����ת��Ϊmap���±�
	else
	{
		temp.x = ( index.x - this->leftMargin ) / this->chessSize;
		temp.y = ( index.y - this->topMargin ) / this->chessSize;
	}
	return temp;
}

//͸����ͼ
void Board::putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
		// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for ( int iy = 0; iy < picture_height; iy++ )
	{
		for ( int ix = 0; ix < picture_width; ix++ )
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ( ( src[srcX] & 0xff000000 ) >> 24 ); //0xAArrggbb;AA��͸����
			int sr = ( ( src[srcX] & 0xff0000 ) >> 16 ); //��ȡRGB���R
			int sg = ( ( src[srcX] & 0xff00 ) >> 8 );   //G
			int sb = src[srcX] & 0xff;              //B
			if ( ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight )
			{
				dstX = ( ix + x ) + ( iy + y ) * graphWidth; //���Դ������صĽǱ�
				int dr = ( ( dst[dstX] & 0xff0000 ) >> 16 );
				int dg = ( ( dst[dstX] & 0xff00 ) >> 8 );
				int db = dst[dstX] & 0xff;
				draw[dstX] = ( ( sr * sa / 255 + dr * ( 255 - sa ) / 255 ) << 16 )  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ( ( sg * sa / 255 + dg * ( 255 - sa ) / 255 ) << 8 )         //��p=sa/255 , FP=sg , BP=dg
					| ( sb * sa / 255 + db * ( 255 - sa ) / 255 );              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}

}

//�ж����λ�ã����ض�Ӧ��ֵ
int Board::checkClick(Coord& index)
{
	if ( mouse.x > 8 && mouse.y > 398 && mouse.x < 286 && mouse.y < 478 )
	{
		return RESTART;
	}
	else if ( mouse.x > 8 && mouse.y > 576 && mouse.x < 286 && mouse.y <  656 )
	{
		return REGRET;
	}
	else if ( mouse.x > 140 && mouse.y > 804 && mouse.x < 216 && mouse.y < 835 )
	{
		return EXIT;
	}
	else if ( mouse.x >= this->leftMargin && mouse.y >= this->topMargin && mouse.x <= 1128 && mouse.y <= 825 )
	{
		index.x = this->mouse.x, index.y = this->mouse.y;
		int ret = this->checkStep(index);    //�жϵ�ǰλ���Ƿ��ھ��������Ƿ�������
		if ( ret )
		{
			return CLICK;
		}
		else
		{
			return FAIL;
		}
	}
	else
	{
		return FAIL;
	}
}

 //�жϵ�ǰλ���Ƿ��ھ��������Ƿ�������
int Board::checkStep(Coord& index)
{
	int x = ( index.x - this->leftMargin ) % this->chessSize;
	int y = ( index.y - this->topMargin ) % this->chessSize;

	x = ( x > ( (double) this->chessSize / 2 ) ) ? ( index.x - x + this->chessSize ) : ( index.x - x );
	y = ( y > ( (double) this->chessSize / 2 ) ) ? ( index.y - y + this->chessSize ) : ( index.y - y );

	double ret = ( ( x - mouse.x ) * ( x - mouse.x ) ) + ( ( y - mouse.y ) * ( y - mouse.y ) );

	double dists = sqrt(ret);
	index.x = x, index.y = y;
	index = this->convertCoord(index);

	if ( this->data.map[index.y][index.x] == 0 && dists < this->dist )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//�����ǰ���̵�״��
void Board::showChess()
{
	for ( int i = 0; i < this->mapSize; i++ )
	{
		for ( int j = 0; j < this->mapSize; j++ )
		{
			if ( this->data.map[i][j] )
			{
				Coord index;
				index.x = j, index.y = i;
				index = this->convertCoord(index);
				if ( this->data.map[i][j] == -1 )
				{
					if ( this->data.first == -1 )
						this->putimagePNG(index.x, index.y, this->blackImg);
					else
						this->putimagePNG(index.x, index.y, this->whiteImg);
				}
				else
				{
					if ( this->data.first == -1 )
						this->putimagePNG(index.x, index.y, this->whiteImg);
					else
						this->putimagePNG(index.x, index.y, this->blackImg);
				}
			}
		}
	}
}

//����
void Board::regretChess(Coord ai, Coord player)
{
	//�ı���һ�ص�˫������������
	this->data.map[ai.y][ai.x] = 0;
	this->data.map[player.y][player.x] = 0;
	
	//�滭����״��
	BeginBatchDraw();
	cleardevice();
	putimage(0, 0, this->boardImg);

	this->showChess();

	EndBatchDraw();

	//�޸Ļ������
	this->data.regretCount--;
}

//��ť����
void Board::buttonsMusic()
{
	mciSendString("open music/select.mp3", 0, 0, 0);
	mciSendString("play music/select.mp3 wait", 0, 0, 0);
	mciSendString("close music/select.mp3", 0, 0, 0);
	Sleep(80);
}

//�ͷ��ڴ�
Board::~Board()
{
	delete this->whiteImg;
	delete this->blackImg;
}
