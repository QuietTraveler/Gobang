#include "Chess.h"
#define MAX 15

//初始化游戏数据
Player::Player()
{
	this->chessImg = new IMAGE;
	this->lastPos.x = 0;
	this->lastPos.y = 0;
	this->playNow = false;
	this->step = 0;
}

//玩家下棋
void Player::playerChess(Coord ai,Board*& board)
{
	Coord index=board->getMouse(ai,this->lastPos);

	if ( board->flag == 1 )
	{
		board->data.map[index.y][index.x] = 1;
		this->step++;
		//上一回的落子点
		this->lastPos.x = index.x;
		this->lastPos.y = index.y;
		//转换坐标
		index=board->convertCoord(index);
		//透明贴图
		board->putimagePNG(index.x, index.y, this->chessImg);

	}
}

Player::~Player()
{
	delete this->chessImg;
}

//初始化数据
AI::AI()
{
	srand((unsigned int)time(0));
	this->playNow = true;
	this->chessImg = new IMAGE;
	this->step = 0;
	this->score = 0;
	this->maxScore = -1;
	this->lastPos.x = 0;
	this->lastPos.y = 0;
}

//下棋
void AI::playerChess(Board*& board,int playStep)
{
	Coord pos;
	if ( !this->step )
	{
		 pos = board->convertCoord({7,7});
		board->data.map[7][7] = -1;
		board->putimagePNG(pos.x,pos.y,this->chessImg);

		this->lastPos.x = 7;
		this->lastPos.y = 7;
	}
	else
	{
		int count = 0,i=0,j=0;
		Coord maxPos{ 0, 0 };
		int total = board->mapSize * board->mapSize - ( this->step + playStep );
		vector<vector<int>>visited(board->data.map);
		this->maxScore = -1;
		while ( count < total )
		{
			Coord Pos{ 0,0 };
			this->score = 0; 
			do
			{
				i = rand() % board->mapSize;
				j = rand() % board->mapSize;
			} while (visited[i][j]);

			visited[i][j] = -1;
			Pos.x = j, Pos.y = i;

			this->score = this->getScore(board,Pos);

			if ( this->score > this->maxScore )
			{
				this->maxScore = this->score;
				maxPos.y = i;
				maxPos.x = j;
			}

			count++;
		}

		board->data.map[maxPos.y][maxPos.x] = -1;
		pos=board->convertCoord(maxPos);
		board->putimagePNG(pos.x, pos.y, this->chessImg);

		this->lastPos.x = maxPos.x;
		this->lastPos.y = maxPos.y;
	}
	this->step++;
}

//计算当前位置总分数
int AI::getScore(Board*& board,Coord index)
{
	int ret = 0;
	int x1(index.x), x2(index.x), x3(index.x), x4(index.x);
	int y1(index.y), y2(index.y), y3(index.y), y4(index.y);
	for ( int i = 0; i < 5; i++ )
	{
		if ( this->is_Ok(x1,y1) )
		{    
			//计算竖向分数
			ret += this->CountScore(this->countNum1(board,x1,y1));
			y1++;
		}

		if ( this->is_Ok(x2,y2) )
		{
			//计算横向分数
			ret += this->CountScore(this->countNum2(board,x2,y2));
			x2++;
		}		
		
		if ( this->is_Ok(x3,y3) )
		{
			//计算正斜向分数
			ret += this->CountScore(this->countNum3(board,x3,y3));
			y3++, x3++;
		}		
		
		if ( this->is_Ok(x4,y4) )
		{
			//计算负斜向分数
			ret += this->CountScore(this->countNum4(board,x4,y4));
			y4++, x4--;
		}
	}
	return ret;
}

//判断map下标是否越界
bool AI::is_Ok(int x,int y)
{
	if ( x < 0 || x >= MAX || y < 0 || y >= MAX )
		return false;
	else
		return true;
}

//统计竖向双方棋子数量
vector<int> AI::countNum1(Board* &board,int x,int y)
{
	vector<int> nums;
	nums.resize(2);
	int cnt = 5;
	while ( cnt-- && this->is_Ok(x,y) )
	{
		if ( board->data.map[y][x] == -1 )
			nums[0]++;
		if ( board->data.map[y][x] == 1 )
			nums[1]++;
		y--;
	}
	//如果出现越界情况，双方棋子数量都为空
	if ( cnt >= 0 )
		return {};
	else 
	return nums;
}


//同上
vector<int> AI::countNum2(Board*& board, int x,int y)
{
	vector<int> nums;
	nums.resize(2);
	int cnt = 5;
	while ( cnt-- && this->is_Ok(x,y) )
	{
		if ( board->data.map[y][x] == -1 )
			nums[0]++;
		if ( board->data.map[y][x] == 1 )
			nums[1]++;
		x--;
	}
	if ( cnt >= 0 )
		return {};
	else
		return nums;
}

//同上
vector<int> AI::countNum3(Board*& board, int x,int y)
{
	vector<int> nums;
	nums.resize(2);
	int cnt = 5;
	while ( cnt-- && this->is_Ok(x,y) )
	{
		if ( board->data.map[y][x] == -1 )
			nums[0]++;
		if ( board->data.map[y][x] == 1 )
			nums[1]++;
		y--, x--;
	}
	if ( cnt >= 0 )
		return {};
	else
		return nums;
}

//同上
vector<int> AI::countNum4(Board*& board, int x,int y)
{
	vector<int> nums;
	nums.resize(2);
	int cnt = 5;
	while ( cnt-- && this->is_Ok(x,y) )
	{
		if ( board->data.map[y][x] == -1 )
			nums[0]++;
		if ( board->data.map[y][x] == 1 )
			nums[1]++;
		y--, x++;
	}
	if ( cnt >= 0 )
		return {};
	else
		return nums;
}

//评分
int AI::CountScore(vector<int>nums)
{
	if(nums.empty())
	return 0;

	int g = nums[0], b = nums[1];

	if ( g == 0 && b == 0 )		    return 10;
	else if ( g == 1 && b == 0 )	return 35;
	else if ( g == 2 && b == 0 )	return 1500;
	else if ( g == 3 && b == 0 )	return 18000;
	else if ( g == 4 && b == 0 )	return 1000000;
	else if ( g == 0 && b == 1 )	return 15;
	else if ( g == 0 && b == 2 )	return 400;
	else if ( g == 0 && b == 3 )	return 6000;
	else if ( g == 0 && b == 4 )	return 150000;
	else if ( g != 0 && b != 0 )	return 0;

	return 0;
}

//判断游戏是否出现胜利者
bool AI::gameOver(Board*& board,Coord index,int ch)
{
	vector<int>val;
	int x1(index.x), x2(index.x), x3(index.x), x4(index.x);
	int y1(index.y), y2(index.y), y3(index.y), y4(index.y);
	for(int i = 0; i < 5; i++ )
	{
		if(this->is_Ok(x1,y1))
		{
			val = countNum1(board, x1, y1);
			if ( !val.empty() && val[ch] == 5 )
				return true;
			y1++;
		}

		if ( this->is_Ok(x2, y2) )
		{
			val = countNum2(board, x2, y2);
			if ( !val.empty() && val[ch] == 5 )
				return true;
			x2++;
		}

		if ( this->is_Ok(x3, y3) )
		{
			val = countNum3(board, x3, y3);
			if ( !val.empty() && val[ch] == 5 )
				return true;
			y3++, x3++;
		}

		if ( this->is_Ok(x4, y4) )
		{
			val = countNum4(board, x4, y4);
			if ( !val.empty() && val[ch] == 5 )
				return true;
			y4++, x4--;
		}
	}
	
	return false;
}

AI::~AI()
{
	delete this->chessImg;
}
