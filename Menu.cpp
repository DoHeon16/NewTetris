#include "Menu.h"
#include<iostream>
#include<conio.h>
using namespace std;

Menu::Menu() :level(0), lines(0), ab_x(5), ab_y(1), limit(0)
{
	init();
}


Menu::~Menu()
{
}

int Menu::show_logo()
{

	gotoxy(13, 3);
	cout << "������������������������������������������������������������������������������������������������������������������������������";
	Sleep(100);
	gotoxy(13, 4);
	cout << "���ߡߡ�  �ߡߡ�  �ߡߡ�   �ߡߡ�   �ߡߡ�   �ߡߡ�   ��    �� ��";
	Sleep(100);
	gotoxy(13, 5);
	cout << "��  ��    ��        ��     ��       ��  ��     ��     �ߡ�  �� ��";
	Sleep(100);
	gotoxy(13, 6);
	cout << "��  ��    �ߡߡ�    ��     ��       ��  ��     ��     ��  �ߡ� ��";
	Sleep(100);
	gotoxy(13, 7);
	cout << "��  ��    ��        ��     ��       ��  ��     ��     ��    �� ��";
	Sleep(100);
	gotoxy(13, 8);
	cout << "��  ��    �ߡߡ�    ��     �ߡߡ�   �ߡߡ�   �ߡߡ�   ��    �� ��";
	Sleep(100);
	gotoxy(13, 9);
	cout << "������������������������������������������������������������������������������������������������������������������������������";

	gotoxy(28, 20);
	cout << "Please Press Any Key~!";

	for (int i = 0; i >= 0; i++)
	{
		if (i % 40 == 0)
		{
			for (int j = 0; j<5; j++)
			{
				gotoxy(18, 14 + j);
				cout << "                                                          ";
			}
			show_cur_block(rand() % 7, rand() % 4, 6, 14);
			show_cur_block(rand() % 7, rand() % 4, 12, 14);
			show_cur_block(rand() % 7, rand() % 4, 19, 14);
			show_cur_block(rand() % 7, rand() % 4, 24, 14);
		}
		if (_kbhit())
			break;
		Sleep(30);
	}

	_getche();
	system("cls");

	return 0;

}

int Menu::input_data()
{
	SetColor(GRAY); //�۾� �� �׷��̷� ����
	gotoxy(10, 8);
	cout << "��������������������<GAME KEY>��������������������" << endl;
	Sleep(10);
	gotoxy(10, 9);
	cout << "�� UP   : Rotate Block        ��" << endl;
	Sleep(10);
	gotoxy(10, 10);
	cout << "�� DOWN : Move One-Step Down  ��" << endl;
	Sleep(10);
	gotoxy(10, 11);
	cout << "�� SPACE: Move Bottom Down    ��" << endl;
	Sleep(10);
	gotoxy(10, 12);
	cout << "�� LEFT : Move Left           ��" << endl;
	Sleep(10);
	gotoxy(10, 13);
	cout << "�� RIGHT: Move Right          ��" << endl;
	Sleep(10);
	gotoxy(10, 14);
	cout << "������������������������������������������������������������" << endl;

	char ch;

	gotoxy(10, 3);
	cout << "1�ܰ���� �����մϴ�." << endl;
	gotoxy(10, 4);
	cout << "�����Ͻðڽ��ϱ�?(y/n)  -->    ";
	cin >> ch;
	while (ch != 'y')
	{
		gotoxy(10, 5);
		cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���. -->  ";
		cin >> ch;
	}
	level = 0;
	gotoxy(10, 6);
	
	money = 50000;
	cout << "���� �ݾ��� " << money << "���Դϴ�.";

	Sleep(1000);
	system("cls");
	return 0;
}

int Menu::init()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if ((j == 0) || (j == 13))
			{
				total_block[i][j] = 1; //�׵θ� ���� �����
			}
			else {
				total_block[i][j] = 0; //�׵θ� ���� �����
			}
		}
	}

	for (int j = 0; j < 14; j++)         //ȭ���� ���� ���� ���� 1�� ä���.
		total_block[20][j] = 1; //�׵θ� �ظ� �����

								//�������� �ʱ�ȭ
	level = 0;
	lines = 0;
	ab_x = 5;
	ab_y = 1;


	stage_data[0].speed = 40;
	stage_data[0].stick_rate = 20;
	stage_data[0].clear_line = 20;
	stage_data[1].speed = 38;
	stage_data[1].stick_rate = 18;
	stage_data[1].clear_line = 20;
	stage_data[2].speed = 35;
	stage_data[2].stick_rate = 18;
	stage_data[2].clear_line = 20;
	stage_data[3].speed = 30;
	stage_data[3].stick_rate = 17;
	stage_data[3].clear_line = 20;
	stage_data[4].speed = 25;
	stage_data[4].stick_rate = 16;
	stage_data[4].clear_line = 20;
	stage_data[5].speed = 20;
	stage_data[5].stick_rate = 14;
	stage_data[5].clear_line = 20;
	stage_data[6].speed = 15;
	stage_data[6].stick_rate = 14;
	stage_data[6].clear_line = 20;
	stage_data[7].speed = 10;
	stage_data[7].stick_rate = 13;
	stage_data[7].clear_line = 20;
	stage_data[8].speed = 6;
	stage_data[8].stick_rate = 12;
	stage_data[8].clear_line = 20;
	stage_data[9].speed = 4;
	stage_data[9].stick_rate = 11;
	stage_data[9].clear_line = 99;
	return 0;
}

int Menu::show_cur_block(int shape, int angle, int x, int y)
{
	switch (shape)
	{
	case 0:
		SetColor(RED);
		break;
	case 1:
		SetColor(BLUE);
		break;
	case 2:
		SetColor(SKY_BLUE);
		break;
	case 3:
		SetColor(WHITE);
		break;
	case 4:
		SetColor(YELLOW);
		break;
	case 5:
		SetColor(VOILET);
		break;
	case 6:
		SetColor(GREEN);
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((j + y) < 0) //��� ������ �ƴϸ�
				continue;

			if (block[shape][angle][j][i] != 0) //���� ��翡 ����
			{
				gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
				cout << "��"<< endl;
			}
		}
	}
	SetColor(BLACK); //�ظ鿡 ������ ���������� ��ȭ?
	gotoxy(77, 23);  //?
	return 0;
}

int Menu::erase_cur_block(int shape, int angle, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[shape][angle][j][i] != 0)
			{
				gotoxy((i + x) * 2 + ab_x, j + y + ab_y);//��ġ�� ��� ��ȭ�Ǵ°�?
				cout << "  " << endl;
				//break;
			}
		}
	}
	return 0;
}