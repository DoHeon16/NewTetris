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
	int BLACK = 0;      /*  0 : ��� */
	int DARK_BLUE = 1;    /*  1 : ��ο� �Ķ� */
	int DARK_GREEN = 2;    /*  2 : ��ο� �ʷ� */
	int DARK_SKY_BLUE = 3;  /*  3 : ��ο� �ϴ� */
	int DARK_RED = 4;    /*  4 : ��ο� ���� */
	int DARK_VOILET = 5;  /*  5 : ��ο� ���� */
	int DARK_YELLOW = 6;  /*  6 : ��ο� ��� */
	int GRAY = 7;      /*  7 : ȸ�� */
	int DARK_GRAY = 8;    /*  8 : ��ο� ȸ�� */
	int BLUE = 9;      /*  9 : �Ķ� */
	int GREEN = 10;      /* 10 : �ʷ� */
	int SKY_BLUE = 11;    /* 11 : �ϴ� */
	int RED = 12;      /* 12 : ���� */
	int VOILET = 13;      /* 13 : ���� */
	int YELLOW = 14;      /* 14 : ��� */
	int WHITE = 15;      /* 15 : �Ͼ� */
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
