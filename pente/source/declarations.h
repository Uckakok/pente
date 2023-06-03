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
