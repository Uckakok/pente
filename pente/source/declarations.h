#pragma once

#define	WHITE	0
#define	BLACK	1
#define	EMPTY	2

#define REGULARPENTE	0
#define KERYOPENTE	1
#define ASCIICONSOLE	0
#define	UTF8CONSOLE	1

#define PENTEPORT	62380

#define MAXSTRING	1024
#define WHITECOLOUR	"\033[0;37m"
#define	REDCOLOUR	"\033[0;31m"

#define BOARDSIZE	19



struct coordinates {
	int x;
	int y;
	coordinates(int x = 0, int y = 0)
		: x(x), y(y)
	{
	}
	bool operator==(const coordinates& left) const {
		return (x == left.x && y == left.y);
	}
};

#define WINVALUE	200000
#define	ONEMOVEWIN 50000
#define UNLOCKEDFOUR	25000
#define	TAKENPAWNS	40000
#define POSSIBLETAKING	20000
#define LOCKEDFOUR	4000
#define UNLOCKEDTHREE	3000

#define ADJACENT8	800
#define ADJACENT7	700
#define ADJACENT6	600
#define ADJACENT5	500
#define ADJACENT4	400
#define ADJACENT3	300
#define ADJACENT2	200
#define ADJACENT1	100