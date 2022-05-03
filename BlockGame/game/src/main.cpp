#include "gameplay/game.h"

int main()
{
	blockcraft::game g;

	while (g.getRunning())
	{
		g.update();
	}

	return 0;
}