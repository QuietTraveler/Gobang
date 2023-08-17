#include"Game.h"

int main()
{
	Game g;
	while ( 1 )
	{
	g.menu();
	g.Init();
	g.play();
	}
	return 0;
}