#pragma once
#include "Menu.h"
#include"Case.h"
#define EXT_KEY         0xffffffe0   //Ȯ��Ű �νİ� 
#define KEY_LEFT      0x4b  //���� ����Ű
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
	int show_total_block();   //�׿����ִ� ���� ȭ�鿡 ǥ���Ѵ�.
	int make_new_block();   //return������ block�� ����ȣ�� �˷���
	int strike_check(int shape, int angle, int x, int y);   //���� ȭ�� �� �Ʒ��� �δ��ƴ��� �˻� �δ�ġ�� 1������ �ƴϸ� 0����
	int merge_block(int shape, int angle, int x, int y);   //���� �ٴڿ� ������� �������� ���� �׾��� ���� ��ħ
	int block_start(int shape, int* angle, int* x, int* y);   //���� ó�� ���ö� ��ġ�� ����� �˷���
	int move_block(int* shape, int* angle, int* x, int* y, int* next_shape);   //���ӿ����� 1������ �ٴڿ� ���� ������ 2�� ����
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

