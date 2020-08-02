#pragma once
#include "Menu.h"
#include"Case.h"
#define EXT_KEY         0xffffffe0   //확장키 인식값 
#define KEY_LEFT      0x4b  //왼쪽 방향키
#define KEY_RIGHT      0x4d
#define KEY_UP         0x48
#define KEY_DOWN      0x50
class Block:public Menu
{
private:
	int block_shape;
	int block_angle;
	int block_x;
	int block_y;
	int next_block_shape;
	int is_gameover;
	Case* m_case;
	int count;
	int m_math;
	int x, y;
	bool graph[15][9];
	bool start = true;
	int sum,checksum;
public:
	Block();
	~Block();
	int show_total_block();   //쌓여져있는 블럭을 화면에 표시한다.
	int make_new_block();   //return값으로 block의 모양번호를 알려줌
	int strike_check(int shape, int angle, int x, int y);   //블럭이 화면 맨 아래에 부닥쳤는지 검사 부닥치면 1을리턴 아니면 0리턴
	int merge_block(int shape, int angle, int x, int y);   //블럭이 바닥에 닿았을때 진행중인 블럭과 쌓아진 블럭을 합침
	int block_start(int shape, int* angle, int* x, int* y);   //블럭이 처음 나올때 위치와 모양을 알려줌
	int move_block(int* shape, int* angle, int* x, int* y, int* next_shape);   //게임오버는 1을리턴 바닥에 블럭이 닿으면 2를 리턴
	int check_full_line();
	void mathSolve(int& line_check,int money);
	int askEnd();
	void printAdd();
	void printMin();
	void store();
	int menu();
	void setGraph(int money, int plus);
	int rotate_block();
	void gaming();
};

