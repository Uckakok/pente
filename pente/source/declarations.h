#pragma once

#define	WHITE	0
#define	BLACK	1
#define	EMPTY	2

#define REGULARPENTE	0
#define KERYOPENTE	1
#define ASCIICONSOLE	0
#define	UTF8CONSOLE	1


#define MAXSTRING	1024
#define WHITECOLOUR	"\033[0;37m"
#define	REDCOLOUR	"\033[0;31m"

#define BOARDSIZE	19

typedef struct {
	int x;
	int y;
} coordinates;
