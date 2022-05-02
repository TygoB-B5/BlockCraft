#include "gameplay/game.h"

int main()
{
	blockcraft::game g;
	g.init();

	while (g.getRunning())
	{
		g.update();
	}

	return 0;
}