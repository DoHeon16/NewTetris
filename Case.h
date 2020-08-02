#pragma once
#include"Menu.h"
class Case :public Menu
{
private:
	int lines;
	int money;
	int sum;
public:
	Case();
	~Case();
	int show_gamestat(int& level); //menu 변수와 연결
	int show_next_block(int& shape);//make_new_block과 변수 연결
	int show_gameover(int& gameover, int& level);
	int getMoney();
	void setMoney(int& money);
	void addMoney(int& money);
	void minMoney(int& money);
	int getlines();
	void addlines(const int& lines);
	void deleteLC();
	void addSum(int& sum);
	void minSum(int& sum);
	int getSum();
};