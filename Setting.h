#pragma once
#include <stdio.h>
#include <conio.h>
#include <string>
#include<Windows.h>
#include<iostream>
using namespace std;
class Setting
{
protected:
	int BLACK = 0;      /*  0 : 까망 */
	int DARK_BLUE = 1;    /*  1 : 어두운 파랑 */
	int DARK_GREEN = 2;    /*  2 : 어두운 초록 */
	int DARK_SKY_BLUE = 3;  /*  3 : 어두운 하늘 */
	int DARK_RED = 4;    /*  4 : 어두운 빨강 */
	int DARK_VOILET = 5;  /*  5 : 어두운 보라 */
	int DARK_YELLOW = 6;  /*  6 : 어두운 노랑 */
	int GRAY = 7;      /*  7 : 회색 */
	int DARK_GRAY = 8;    /*  8 : 어두운 회색 */
	int BLUE = 9;      /*  9 : 파랑 */
	int GREEN = 10;      /* 10 : 초록 */
	int SKY_BLUE = 11;    /* 11 : 하늘 */
	int RED = 12;      /* 12 : 빨강 */
	int VOILET = 13;      /* 13 : 보라 */
	int YELLOW = 14;      /* 14 : 노랑 */
	int WHITE = 15;      /* 15 : 하양 */
	int x;
	int y;
	int color;
public:
	Setting();
	~Setting();
	int gotoxy(int x, int y);
	void SetColor(int color);
	void SetCursor(bool bVisible);
};
