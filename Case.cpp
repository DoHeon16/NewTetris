#include "Case.h"
#include<iostream>
#include<iomanip>
using namespace std;
Case::Case() :money((level + 1) * 50000), lines(0),sum(0)
{
}

Case::~Case()
{
}

int Case::show_gamestat(int& level)
{

	static int printed_text = 0; //상수 처리
	SetColor(GRAY);
	if (printed_text == 0)
	{
		gotoxy(35, 7);
		cout << "STAGE";

		gotoxy(35, 11);
		cout << "MONEY";

		gotoxy(35, 15);
		cout << "TARGET MONEY";

	}
	gotoxy(41, 7);
	cout << setw(1) << level + 1;
	gotoxy(35, 12);
	cout << setw(10) << money;
	gotoxy(35, 16);
	cout << setw(10) << (level + 1) * 300000;

	return 0;
}

int  Case::getlines() {

	return this->lines;
}

void Case::addlines(const int& lines) {
	this->lines += lines;
}

void Case::deleteLC() {

	this->lines = 0;
}

void Case::addSum(int& sum)
{
	this->sum += sum;
}

void Case::minSum(int & sum)
{
	this->sum -= sum;
}

int Case::getSum()
{
	return sum;
}

int Case::show_next_block(int& shape)//다음 보여줄 블럭의 경계와 블럭
{

	int i, j;
	SetColor((level + 1) % 6 + 1);
	for (i = 1; i < 7; i++)
	{
		gotoxy(33, i);
		for (j = 0; j < 6; j++)
		{
			if (i == 1 || i == 6 || j == 0 || j == 5) //다음 보여줄 블럭 경계 벽
			{
				cout << "■";
			}
			else {
				cout << "  ";
			}

		}
	}

	show_cur_block(shape, 0, 15, 1);
	return 0;
}

int Case::show_gameover(int & gameover, int & level)
{
	SetColor(RED);
	gotoxy(15, 8);
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << endl;
	gotoxy(15, 9);
	cout << "┃**************************┃" << endl;
	gotoxy(15, 10);
	cout << "┃*        GAME OVER       *┃" << endl;
	gotoxy(15, 11);
	cout << "┃**************************┃" << endl;
	gotoxy(15, 12);
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << endl;

	if (level == 10) {
		gameover = 1;
	}
	else
		gameover = 0;
	fflush(stdin);
	Sleep(1000);

	_getche();
	system("cls");
	deleteLC();
	int a = (level + 1) * 50000;
	setMoney(a);
	gameover = 0;
	return 0;
}

int Case::getMoney()
{
	return money;
}

void Case::setMoney(int& money)
{
	this->money = money;
}

void Case::addMoney(int& money)
{
	this->money += money;
}

void Case::minMoney(int & money)
{
	this->money -= money;
}

