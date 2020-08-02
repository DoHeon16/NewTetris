#include "Block.h"
#include<iostream>
using namespace std;

Block::Block() :block_shape(0), next_block_shape(0), block_x(0), block_y(0), block_angle(0), is_gameover(0), count(0), m_math(1),sum(0),checksum(0)
{
	m_case = new Case();
}

Block::~Block()
{
}

int Block::show_total_block()
{
	//   SetColor(RED);
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (j == 0 || j == 13 || i == 20)      //������ ���� �ܺ� ���� ����
			{
				SetColor((level % 6) + 1); //�ܺ� ���� �������� ����

			}
			else {
				SetColor(YELLOW);  //�������� �׾Ҵ� ����� ��ũ�׷��̷� �����ֱ�
			}
			gotoxy((j * 2) + ab_x, i + ab_y); //Ŀ�� ��ġ ����-> ��?
			if (total_block[i][j] != 0)
			{
				if (j == 0 || j == 13 || i == 20) {
					cout << "��" << endl;
				}
				else
					cout << "��" << endl;
			}
			else {
				cout << "  " << endl;
			}

		}
	}
	SetColor(BLACK);
	gotoxy(77, 23);
	return 0;
}

int Block::make_new_block()
{
	int i = rand() % 100;
	if (i <= stage_data[level].stick_rate)      //����� ����Ȯ�� ���
		return 0;                     //����� ������� ����
	int shape = (rand() % 6) + 1;      //shape���� 1~6�� ���� ��
	return shape;
}

int Block::strike_check(int shape, int angle, int x, int y)
{
	int block_dat; //���鿡 ����� �˻��ϱ� ���� ����

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (((x + j) == 0) || ((x + j) == 13))//x+j �� �� �� ���� ������
				block_dat = 1;
			else {
				if ((y + i) >= 0) // y��ǥ�� -3���� �����ϹǷ� 0���� �϶� ������ ����
					block_dat = total_block[y + i][x + j];
				else
					block_dat = 0;
			}

			if ((block_dat != 0) && (block[shape][angle][i][j] != 0))       //������ �˻�                                                         //�������� ��ǥ�� ��������
			{

				return 1;
			}
		}
	}
	return 0;
}

int Block::merge_block(int shape, int angle, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			total_block[y + i][x + j] |= block[shape][angle][i][j]; //��Ʈor������ 
		}
	}
	check_full_line();
	show_total_block();

	return 0;
}

int Block::block_start(int shape, int * angle, int * x, int * y)
{
	*x = 5;
	*y = -3;
	*angle = 0;
	count++;
	return 0;
}

int Block::move_block(int * shape, int * angle, int * x, int * y, int * next_shape)
{
	erase_cur_block(*shape, *angle, *x, *y);

	(*y)++;   //���� ��ĭ �Ʒ��� ����
	if (strike_check(*shape, *angle, *x, *y) == 1)
	{
		if (*y < 1)   //���ӿ��� 
		{
			(*y)--;
			return 1;
		}
		(*y)--;
		merge_block(*shape, *angle, *x, *y);
		*shape = *next_shape;
		*next_shape = make_new_block();

		block_start(*shape, angle, x, y);   //angle,x,y�� ��������
		m_case->show_next_block(*next_shape);
		return 2; //
	}
	return 0;//�Ʒ��� ������ �� �ƹ��͵� ������
}

int Block::check_full_line()
{
	int j, line_count = 0, line_check = 0, check = 0;
	for (int i = 0; i < 20; i++)
	{
		for (j = 1; j < 13; j++)
		{
			if (total_block[i][j] == 0) {
				break;
			}
		}

		if (j == 13)   //������ �� ä������
		{
			check = 1;
			line_check++;
			m_math++;
			m_case->addlines(1);
			show_total_block();
			SetColor(WHITE);
			gotoxy(1 * 2 + ab_x, i + ab_y);
			for (j = 1; j < 25; j++)
			{
				line_count += total_block[i][j];
				cout << "$";
				Sleep(10);
			}
			gotoxy(1 * 2 + ab_x, i + ab_y);
			for (j = 1; j < 13; j++)
			{
				cout << "  ";
				Sleep(10);
			}

			for (int k = i; k > 0; k--)
			{
				for (j = 1; j < 13; j++)
					total_block[k][j] = total_block[k - 1][j];
			}
			for (j = 1; j < 13; j++)
				total_block[0][j] = 0;
			int m_money = 0;

			m_money += (line_count * 700);
			mathSolve(m_math,m_money);
			if (m_math % 4 != 0) {
				setGraph(m_money, 2);
			}
			m_case->show_gamestat(level);
		}

	}
	show_total_block();
	m_case->show_gamestat(level);
	if ((count %= 8) == 0 && check != 1) {
		int min = 5000;
		m_case->minMoney(min);
		setGraph(min, 3);
		m_case->show_gamestat(level);
		printMin();
	}

	if (line_check >= 2) {
		printAdd();
		checksum = line_check * 1000;
		m_case->addMoney(checksum);
		setGraph(0, 1);
		m_case->show_gamestat(level);
		m_case->addSum(checksum);
	}
	return 0;
}

void Block::mathSolve(int & math, int money)
{
	if (math % 4 == 0) {
		system("cls");
		srand(time(NULL));
		clock_t start, end;//�ð� ������ ����
						   //�ð� ���� ���� �ٽú���
		gotoxy(20, 10);
		SetColor(SKY_BLUE);
		sum += money;
		//int a = time(0);

		//while (1) {
		//   if (time(0) - a >= 5)
		//      return;
		//}
		cout << "��¦ ���� Ÿ��~~~~~ ��Ģ������ Ǯ�����^^!" << endl;
		int code = rand() % 3;
		int num1 = rand() % 100 + 1;
		int num2 = rand() % 100 + 1;
		int ans;
		int cor;
		gotoxy(20, 11);
		cout << "5�ʰ� ������ Ʋ�� ������ �����մϴ�." << endl;
		start = clock();//���� �ð� üũ
		switch (code) {
		case 0:
			gotoxy(20, 15);
			SetColor(GREEN);
			cout << num1 << " + " << num2;
			end = clock();
			while (((double)(end - start) / (double)CLOCKS_PER_SEC) < 5) {
				end = clock();
			}
			system("cls");
			gotoxy(20, 15);
			cout << "������ �����ּ���. -->  ";
			cin >> ans;
			if (ans == (num1 + num2)) {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �߰��˴ϴ�." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �Ҹ�˴ϴ�." << endl;
				cor = 0;
				Sleep(1000);
			}
			break;
		case 1:
			gotoxy(20, 15);
			SetColor(GREEN);
			cout << num1 << " - " << num2 ;
			end = clock();
			while (((double)(end - start) / (double)CLOCKS_PER_SEC) < 5) {
				end = clock();
			}
			system("cls");
			gotoxy(20, 15);
			cout << "������ �����ּ���. -->  ";
			cin >> ans;
			if (ans == (num1 - num2)) {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �߰��˴ϴ�." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �Ҹ�˴ϴ�." << endl;
				cor = 0;
				Sleep(1000);
			}
			break;
		case 2:
			gotoxy(20, 15);
			SetColor(GREEN);
			cout << num1 << " �� " << num2 ;
			end = clock();
			while (((double)(end - start) / (double)CLOCKS_PER_SEC) < 5) {
				end = clock();
			}
			system("cls");
			gotoxy(20, 15);
			cout << "������ �����ּ���. -->  ";
			cin >> ans;
			if (ans == (num1*num2)) {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �߰��˴ϴ�." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "�����Դϴ�. ���� ������ �Ҹ�˴ϴ�." << endl;
				cor = 0;
				Sleep(1000);
			}
			break;
		}

		system("cls");
		Sleep(1000);
		if (cor == 1) {
			setGraph(0, 1);
		}
		return;
	}
}

int Block::askEnd()
{
	system("cls");
	SetColor(WHITE);
	gotoxy(1, 1);
	cout << level + 1 << "�ܰ谡 ����Ǿ����ϴ�. �� �Ͻðڽ��ϱ�?(y/n) ===> ";
	char ch;
	cin >> ch;
	//while (ch != 'y' || ch != 'n') {
	//	gotoxy(1, 3);
	//	cout << "�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ��� ==> ";
	//	cin >> ch;
	//}
	if (ch == 'y') {
		system("cls");
		return 0;
	}
	else if(ch == 'n') {
		system("cls");
		return 1;
	}
}

void Block::printAdd()
{
	int a = rand() % 4;
	switch (a) {
	case 0:
		gotoxy(10, 23);
		cout << "��Ʈ������ īī����ũ�� ���޸� �ξ����ϴ�.                                " << endl; break;
	case 1:
		gotoxy(10, 23);
		cout << "��Ʈ������ ���ȱ���� ��ȭ�Ǿ����ϴ�.                                     " << endl; break;
	case 2:
		gotoxy(10, 23);
		cout << "��Ʈ������ �Ｚ���� ���޸� �ξ����ϴ�.                                    " << endl; break;
	case 3:
		gotoxy(10, 23);
		cout << "���ΰ� ���� ������ ��ȭ�߽��ϴ�.                                          " << endl; break;
	}
	gotoxy(10, 24);
	cout << "�ְ��� ����մϴ�.      " << endl;
	setGraph(0, 1);
}

void Block::printMin()
{
	int a = rand() % 4;
	switch (a) {
	case 0:
		gotoxy(10, 23);
		cout << "��Ʈ������ ������� ���Ǵ� ��簡 �����ϴ�.                          " << endl; break;
	case 1:
		gotoxy(10, 23);
		cout << "���ο� ���� ������ ��ȭ�Ǿ����ϴ�.                                       " << endl; break;
	case 2:
		gotoxy(10, 23);
		cout << "���޾�ü����� ����� �������ϴ�.                                        " << endl; break;
	case 3:
		gotoxy(10, 23);
		cout << "��Ʈ���� ���� ���ῡ ���� ����� ���ȭ�Ǿ����ϴ�.                 " << endl; break;
	}
	gotoxy(10, 24);
	cout << "5000���� �ְ��� �϶��մϴ�.                                                  " << endl;
	setGraph(0, 3);
}

void Block::store()
{
	system("cls");
	int car = 600000;
	int house = 700000;
	int building = 100000;
	int ship = 800000;
	int airplane = 900000;
	gotoxy(10, 5);
	SetColor(YELLOW);
	cout << "���� ���� �� : " << m_case->getSum() << endl;
	gotoxy(10, 7);
	cout << "1. �ڵ���" << '\t' << '\t' << "2. ��" << '\t'<< '\t' << "3. ����"<<'\t'<<'\t'<<"4. ��"<<'\t'<< '\t' << "5. �����" << endl;
	gotoxy(10, 8);
	cout << "60����" << '\t' << '\t' << "70����" << '\t' << '\t' << "100����" << '\t' << '\t' << "80����" << '\t' << '\t' << "90����" << endl;
	//������ �߰�
	gotoxy(10, 10);
	cout << "������ �����Ͻðڽ��ϱ�?  ==>  ";
	int choice;
	cin >> choice;
	switch (choice) {
	case 1:
		if (m_case->getSum() < car) {
			gotoxy(10, 12);
			cout << "���� �����մϴ�. �����մϴ�." << endl;
			break;
		}
		else
		{
			gotoxy(10, 12);
			cout << "                ������" << endl;
			gotoxy(10, 13);
			cout << "               ���ˢˢˡ�" << endl;
			gotoxy(10, 14);
			cout << "               ���ˢˢˢˡ�" << endl;
			gotoxy(10, 15);
			cout << "               ���ˢˢˢˡ�" << endl;
			gotoxy(10, 16);
			cout << "     ������������������" << endl;
			gotoxy(10, 17);
			cout << "  ��������������������" << endl;
			gotoxy(10, 18);
			cout << "      ����ۡۡۡ����ۡۡۡ����" << endl;
			gotoxy(10, 19);
			cout << "         ��ۣ����ۡ���ۣ����ۡ��" << endl;
			gotoxy(10, 20);
			cout << "            �ۡۡ�        �ۡۡ�" << endl;
			gotoxy(10, 22);
			cout << "�ڵ����� �����ϼ̽��ϴ�." << endl;
			m_case->minSum(car);
			break;
		}
	case 2:
		if (m_case->getSum() < house) {
			gotoxy(10, 12);
			cout << "���� �����մϴ�. �����մϴ�." << endl;
			break;
		}
		else {
			gotoxy(10, 12);
			cout << "     A" << endl;
			gotoxy(10, 13);
			cout << "        iQBQ: BBB" << endl;
			gotoxy(10, 14);
			cout << "       QBBBBQ QBM" << endl;
			gotoxy(10, 15);
			cout << "     1BQBBBBBBBBM" << endl;
			gotoxy(10, 16);
			cout << "   iBBBBBBBBBBBBQ" << endl;
			gotoxy(10, 17);
			cout << " .BQBBBBBBBQBBBBBBB" << endl;
			gotoxy(10, 18);
			cout << "PBQBBBQBQBBBBBBBBBBB" << endl;
			gotoxy(10, 19);
			cout << "   BBBBQBBBBBBBBBB" << endl;
			gotoxy(10, 20);
			cout << "   BQBBBB: iBBBBBB" << endl;
			gotoxy(10, 21);
			cout << "   QBBBBB   BQBBBB" << endl;
			gotoxy(10, 23);
			cout << "���� �����ϼ̽��ϴ�." << endl;
			m_case->minSum(house);
			break;
		}
	case 3:
		if (m_case->getSum() < building) {
			gotoxy(10, 12);
			cout << "���� �����մϴ�. �����մϴ�." << endl;
			break;
		}
		else {
			gotoxy(10, 12);
			cout << "          i" << endl;
			gotoxy(10, 13);
			cout << "          ." << endl;
			gotoxy(10, 14);
			cout << "          7" << endl;
			gotoxy(10, 15);
			cout << "          i" << endl;
			gotoxy(10, 16);
			cout << "         .I" << endl;
			gotoxy(10, 17);
			cout << "         :ii" << endl;
			gotoxy(10, 18);
			cout << "         rvL" << endl;
			gotoxy(10, 19);
			cout << "        .7vv" << endl;
			gotoxy(10, 20);
			cout << "        :urv" << endl;
			gotoxy(10, 21);
			cout << "        .Yir:" << endl;
			gotoxy(10, 22);
			cout << "        :1r7i" << endl;
			gotoxy(10, 23);
			cout << "       .Yu77i" << endl;
			gotoxy(10, 24);
			cout << "       .L1v:j" << endl;
			gotoxy(10, 25);
			cout << "       .J1v.S." << endl;
			gotoxy(10, 26);
			cout << "       :Y5L:I." << endl;
			gotoxy(10, 27);
			cout << "      .sLJj:1." << endl;
			gotoxy(10, 28);
			cout << "      .17uu:ij" << endl;
			gotoxy(10, 29);
			cout << "      :YvJY.:U." << endl;
			gotoxy(10, 30);
			cout << "      :Y7jL::U." << endl;
			gotoxy(10, 31);
			cout << "      J7ruK.rI7" << endl;
			gotoxy(10, 32);
			cout << "    .7IJu2K277rv.." << endl;
			gotoxy(10, 33);
			cout << "   RBBBBBQBBBBBBBQg" << endl; 
			gotoxy(10, 35);
			cout << "������ �����ϼ̽��ϴ�." << endl;
			m_case->minSum(building);
			break;
		}
	case 4:
		if (m_case->getSum() < ship) {
			gotoxy(10, 12);
			cout << "���� �����մϴ�. �����մϴ�." << endl;
			break;
		}
		else
		{
			gotoxy(10, 12);
			cout << "             sYvvU            rQKqE" << endl;
			gotoxy(10, 13);
			cout << "             KjvL5:          BBgPBR" << endl;
			gotoxy(10, 14);
			cout << "             ..7r:.           iiZ:i" << endl;
			gotoxy(10, 15);
			cout << "            i..rr.::        i7:rd:i7" << endl;
			gotoxy(10, 16);
			cout << "           7DSUjJSb:        BQBRgBBP" << endl;
			gotoxy(10, 17);
			cout << "            2vvvJuPr        BBQRdbES" << endl;
			gotoxy(10, 18);
			cout << "               i.               E" << endl;
			gotoxy(10, 19);
			cout << "  vRBBggQJggQJgQJggQ..    .. iBiBBQB" << endl;
			gotoxy(10, 20);
			cout << "     QQELQQEgB: J QgBDDJDQLDQBQBBBBB" << endl;
			gotoxy(10, 21);
			cout << "          QDPMgdMgBgdMgBBQBQBdsBBBB" << endl;
			gotoxy(10, 22);
			cout << "          ZQgsvDiiQ2ZDiQ2ZDQ2ZDvBBB" << endl;
			gotoxy(10, 23);
			cout << "           1P5jPvPvDgQgDgfsfwQgd" << endl;
			gotoxy(10, 25);
			cout << "�踦 �����ϼ̽��ϴ�." << endl;
			m_case->minSum(ship);
			break;
		}
	case 5:
		if (m_case->getSum() < airplane) {
			gotoxy(10, 12);
			cout << "���� �����մϴ�. �����մϴ�." << endl;
			break;
		}
		else
		{
			gotoxy(10, 12);
			cout << "                 rBE" << endl;
			gotoxy(10, 13);
			cout << "   :.          :QBBB" << endl;
			gotoxy(10, 14);
			cout << " .BBBBBBBXv:  BBBBBP" << endl;
			gotoxy(10, 15);
			cout << "  uBBBBBQBBBQBBBBBU" << endl;
			gotoxy(10, 16);
			cout << "     :gBBBBBBQBBg" << endl;
			gotoxy(10, 17);
			cout << "        vBBBBBBBu" << endl;
			gotoxy(10, 18);
			cout << "       vBBBQBBBBB" << endl;
			gotoxy(10, 19);
			cout << " rM1.iBBBBBPZBQBB7" << endl;
			gotoxy(10, 20);
			cout << "5BBBBBBBBB:  BBBBB" << endl;
			gotoxy(10, 21);
			cout << "  rBBBBBI     BBBB." << endl;
			gotoxy(10, 22);
			cout << "    iBBBg      BBBM" << endl;
			gotoxy(10, 23);
			cout << "      gBQ       gr" << endl;
			gotoxy(10, 25);
			cout << "����⸦ �����ϼ̽��ϴ�." << endl;
			m_case->minSum(airplane);
			break;
		}
	default:
		gotoxy(10, 12);
		cout << "�߸��� �Է��Դϴ�. �����մϴ�." << endl;
		break;
	}
	Sleep(1500);
	system("cls");
	return;
}

int Block::menu()
{
	SetColor(YELLOW);
	gotoxy(10, 10);
	cout << "<< ��Ʈ���� ���迡 ���� ���� ȯ���մϴ�~!~!~! >>" << endl;
	SetColor(RED);
	gotoxy(10, 12);
	cout << "1. ���� ä���ϱ�" << endl;
	gotoxy(10, 14);
	cout << "2. ����" << endl;
	gotoxy(10, 16);
	cout << "3. �����ϱ�" << endl;

	gotoxy(10, 18);
	cout << "�Է��ϼ��� -> ";
	int a;
	cin >> a;
	switch (a)
	{
	case 1:
		system("cls");
		input_data();
		show_total_block();//��Ʈ���� �׵θ� ���
		block_shape = make_new_block();
		next_block_shape = make_new_block();
		m_case->show_next_block(next_block_shape);
		block_start(block_shape, &block_angle, &block_x, &block_y);
		m_case->show_gamestat(level);
		rotate_block();
		return 1;
	case 2:
		store();
		return 1;
	case 3:
		gotoxy(10, 22);
		cout << "�����մϴ�." << endl;
		system("cls");
		return 0;
	default:
		gotoxy(10, 22);
		cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
		system("cls");
		return 1;
	}
}

void Block::setGraph(int money, int plus)
{
	int up[10][10] = { { 0,0,0,0,1,1,0,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,1,1,1,1,1,1,0,0 },{ 0,1,1,1,1,1,1,1,1,0 },{ 1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 } };
	int down[10][10] = { { 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 },{ 0,0,0,1,1,1,1,0,0,0 } ,{ 1,1,1,1,1,1,1,1,1,1 } ,{ 0,1,1,1,1,1,1,1,1,0 } ,{ 0,0,1,1,1,1,1,1,0,0 } ,{ 0,0,0,1,1,1,1,0,0,0 } ,{ 0,0,0,0,1,1,0,0,0,0 } };
	SetColor(WHITE);
	if (plus == 2) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gotoxy(55 + 2 * i, 10 + j);
				if (down[i][j] == 1) {
					cout << "��";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << m_case->getSum() << "�� ��� ����";
	}
	else if (plus == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gotoxy(55 + 2 * i, 10 + j);
				if (up[j][i] == 1) {
					cout << "��";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << sum << "�� ���";
	}
	else if (plus == 3) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gotoxy(55 + 2 * i, 10 + j);
				if (down[j][i] == 1) {
					cout << "��";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << money << "�� �϶�";
	}

}


int Block::rotate_block()
{
	char keytemp = 0;

	for (int i = 1; 1; i++)//���� ����
	{
		if (_kbhit())//����Ű ����
		{
			keytemp = _getche();
			if (keytemp == EXT_KEY)
			{
				keytemp = _getche();
				switch (keytemp)
				{
				case KEY_UP:      //ȸ���ϱ�

					if (strike_check(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0)//ȸ�� �������� ���캻��.
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_angle = (block_angle + 1) % 4;
						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_LEFT:      //�������� �̵�
					if (block_x > 1)
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_x--;
						if (strike_check(block_shape, block_angle, block_x, block_y) == 1)//�̵����� �� ����� �����Ѵٸ�
							block_x++;//�ٽ� ���󺹱�

						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_RIGHT:      //���������� �̵�

					if (block_x < 14)
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_x++;
						if (strike_check(block_shape, block_angle, block_x, block_y) == 1)
							block_x--;
						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_DOWN:      //�Ʒ��� �̵�
					is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
					show_cur_block(block_shape, block_angle, block_x, block_y);
					break;
				}
			}
			if (keytemp == 27) // esc�� ������ �����߿� �ܻ��� ���� ���� ����
			{
				erase_cur_block(block_shape, block_angle, block_x, block_y);
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}

			if (keytemp == 32)   //�����̽��ٸ� ��������
			{
				while (is_gameover == 0)
				{
					is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
				}
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}
		}
		if (i%stage_data[level].speed == 0)//�ܰ躰 �ӵ�����
		{
			if (keytemp == 27) // esc�� ������ �����߿� �ܻ��� ���� ���� ����
			{
				erase_cur_block(block_shape, block_angle, block_x, block_y);
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}

			is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);//��ϵ��� ���پ� ������

			show_cur_block(block_shape, block_angle, block_x, block_y);
		}

		//if (stage_data[level].clear_line <= m_case->getlines())   // ���� ���� ���� ���־� �ϴ� ���� ������ ũ�ų� ������ �������� Ŭ����
		//														  // *** ���� ���� �ѹ��� �������� ���� ��츦 ����ġ�� �Ǹ� �������� Ŭ��� �� �� ���� ������ ==�� <=�� ���� ***
		//{
		//	m_case->deleteLC();
		//	gotoxy(10, 26);
		//	cout << level + 1 << "�ܰ��� ��ǥ �� ���� �޼��Ͽ����ϴ�." << endl;
		//	Sleep(700);
		//	is_gameover = askEnd();
		//	level++; // ������
		//	count = 0;
		//	/*	for (int i = 0; i < 20; i++) //�׿����ִ� ��� ���� ����
		//	{
		//	for (int j = 1; j < 13; j++)
		//	{
		//	if (total_block[i][j] == 1)
		//	total_block[i][j] = 0;
		//	}
		//	}*/

		//	show_total_block(); // ��ü ��, �ܺ��� ȭ�鿡 ǥ���Ѵ�.
		//	m_case->show_gamestat(level); // �����ʿ� ���� �ܰ�, ����, �� �� �����ֱ�
		//	m_case->show_next_block(block_shape); // ������� ������


		//}
		if (m_case->getMoney() >= (level + 1) * 300000) {

			m_case->deleteLC();
			gotoxy(10, 26);
			cout << level + 1 << "�ܰ��� ��ǥ �ݾ��� �޼��Ͽ����ϴ�." << endl;
			Sleep(700);

			is_gameover = askEnd();
			level++; // ������
			count = 0;
			//	for (int i = 0; i < 20; i++)
			//	 {
			//		 for (int j = 1; j < 13; j++)
			//			 {
			//				 if (total_block[i][j] == 1)
			//				 total_block[i][j] = 0;
			//				}
			//	 }

			show_total_block(); // ��ü ��, �ܺ��� ȭ�鿡 ǥ���Ѵ�.
			m_case->show_gamestat(level); // �����ʿ� ���� �ܰ�, ����, �� �� �����ֱ�
			m_case->show_next_block(block_shape); // ������� ������
		}
		if (m_case->getMoney() <= 0) {
			is_gameover = 1;
		}

		if (is_gameover == 1)
		{
			for (int i = 0; i < 20; i++)
			{
				for (int j = 1; j < 13; j++)
				{
					if (total_block[i][j] != 0)
						total_block[i][j] = 0;
				}
			}
			if (level == 11) {
				system("cls");
				gotoxy(10, 8);
				cout << "��� Ŭ�����ϼ̽��ϴ�! �����մϴ�~!" << endl;
				gotoxy(10, 10);
				cout << "�������� ���ư��ϴ�." << endl;
				return 0;
			}
			m_case->show_gameover(is_gameover, level);
			count = 0;
			int m = 50000;
			m_case->setMoney(m);
			SetColor(GRAY);
			break;
		}
		//�ܻ��� ���´ٸ� Ű�� �Է¹��� �� ����
		gotoxy(77, 23);
		Sleep(15);
		gotoxy(77, 23);
	}
	is_gameover = init();
	return 0;
}

void Block::gaming()
{
	int end = 1;
	while (end) {
		show_logo();
		end = menu();
	}
	gotoxy(10, 8);
	cout << "����˴ϴ�. �ȳ�~~" << endl;
}

