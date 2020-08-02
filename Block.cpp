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
			if (j == 0 || j == 13 || i == 20)      //레벨에 따라 외벽 색이 변함
			{
				SetColor((level % 6) + 1); //외벽 색은 랜덤으로 설정

			}
			else {
				SetColor(YELLOW);  //이제까지 쌓았던 블록을 다크그레이로 보여주기
			}
			gotoxy((j * 2) + ab_x, i + ab_y); //커서 위치 변경-> 왜?
			if (total_block[i][j] != 0)
			{
				if (j == 0 || j == 13 || i == 20) {
					cout << "■" << endl;
				}
				else
					cout << "ⓣ" << endl;
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
	if (i <= stage_data[level].stick_rate)      //막대기 나올확률 계산
		return 0;                     //막대기 모양으로 리턴
	int shape = (rand() % 6) + 1;      //shape에는 1~6의 값이 들어감
	return shape;
}

int Block::strike_check(int shape, int angle, int x, int y)
{
	int block_dat; //옆면에 닿는지 검사하기 위한 변수

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (((x + j) == 0) || ((x + j) == 13))//x+j 이 양 옆 벽에 닿으면
				block_dat = 1;
			else {
				if ((y + i) >= 0) // y좌표가 -3부터 시작하므로 0부터 일때 범위를 설정
					block_dat = total_block[y + i][x + j];
				else
					block_dat = 0;
			}

			if ((block_dat != 0) && (block[shape][angle][i][j] != 0))       //벽인지 검사                                                         //좌측벽의 좌표를 빼기위함
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
			total_block[y + i][x + j] |= block[shape][angle][i][j]; //비트or연산자 
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

	(*y)++;   //블럭을 한칸 아래로 내림
	if (strike_check(*shape, *angle, *x, *y) == 1)
	{
		if (*y < 1)   //게임오버 
		{
			(*y)--;
			return 1;
		}
		(*y)--;
		merge_block(*shape, *angle, *x, *y);
		*shape = *next_shape;
		*next_shape = make_new_block();

		block_start(*shape, angle, x, y);   //angle,x,y는 포인터임
		m_case->show_next_block(*next_shape);
		return 2; //
	}
	return 0;//아래쪽 내렸을 때 아무것도 없으면
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

		if (j == 13)   //한줄이 다 채워졌음
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
		clock_t start, end;//시간 제한할 변수
						   //시간 제한 구문 다시보자
		gotoxy(20, 10);
		SetColor(SKY_BLUE);
		sum += money;
		//int a = time(0);

		//while (1) {
		//   if (time(0) - a >= 5)
		//      return;
		//}
		cout << "깜짝 퀴즈 타임~~~~~ 사칙연산을 풀어보세요^^!" << endl;
		int code = rand() % 3;
		int num1 = rand() % 100 + 1;
		int num2 = rand() % 100 + 1;
		int ans;
		int cor;
		gotoxy(20, 11);
		cout << "5초가 지나면 틀린 것으로 간주합니다." << endl;
		start = clock();//시작 시간 체크
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
			cout << "정답을 적어주세요. -->  ";
			cin >> ans;
			if (ans == (num1 + num2)) {
				gotoxy(20, 16);
				cout << "정답입니다. 쌓은 코인이 추가됩니다." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "오답입니다. 쌓은 코인이 소멸됩니다." << endl;
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
			cout << "정답을 적어주세요. -->  ";
			cin >> ans;
			if (ans == (num1 - num2)) {
				gotoxy(20, 16);
				cout << "정답입니다. 쌓은 코인이 추가됩니다." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "오답입니다. 쌓은 코인이 소멸됩니다." << endl;
				cor = 0;
				Sleep(1000);
			}
			break;
		case 2:
			gotoxy(20, 15);
			SetColor(GREEN);
			cout << num1 << " × " << num2 ;
			end = clock();
			while (((double)(end - start) / (double)CLOCKS_PER_SEC) < 5) {
				end = clock();
			}
			system("cls");
			gotoxy(20, 15);
			cout << "정답을 적어주세요. -->  ";
			cin >> ans;
			if (ans == (num1*num2)) {
				gotoxy(20, 16);
				cout << "정답입니다. 쌓은 코인이 추가됩니다." << endl;
				m_case->addMoney(sum);
				m_case->addSum(sum);
				cor = 1;
				Sleep(1000);
			}
			else {
				gotoxy(20, 16);
				cout << "오답입니다. 쌓은 코인이 소멸됩니다." << endl;
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
	cout << level + 1 << "단계가 종료되었습니다. 더 하시겠습니까?(y/n) ===> ";
	char ch;
	cin >> ch;
	//while (ch != 'y' || ch != 'n') {
	//	gotoxy(1, 3);
	//	cout << "잘못된 입력입니다. 다시 입력하세요 ==> ";
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
		cout << "테트코인이 카카오뱅크와 제휴를 맺었습니다.                                " << endl; break;
	case 1:
		gotoxy(10, 23);
		cout << "테트코인의 보안기술이 강화되었습니다.                                     " << endl; break;
	case 2:
		gotoxy(10, 23);
		cout << "테트코인이 삼성과의 제휴를 맺었습니다.                                    " << endl; break;
	case 3:
		gotoxy(10, 23);
		cout << "정부가 코인 규제를 완화했습니다.                                          " << endl; break;
	}
	gotoxy(10, 24);
	cout << "주가가 상승합니다.      " << endl;
	setGraph(0, 1);
}

void Block::printMin()
{
	int a = rand() % 4;
	switch (a) {
	case 0:
		gotoxy(10, 23);
		cout << "테트코인이 랜섬웨어에 사용되는 기사가 떴습니다.                          " << endl; break;
	case 1:
		gotoxy(10, 23);
		cout << "코인에 대한 규제가 강화되었습니다.                                       " << endl; break;
	case 2:
		gotoxy(10, 23);
		cout << "제휴업체들과의 계약이 끝났습니다.                                        " << endl; break;
	case 3:
		gotoxy(10, 23);
		cout << "테트코인 서비스 종료에 대한 유언비어가 기사화되었습니다.                 " << endl; break;
	}
	gotoxy(10, 24);
	cout << "5000원의 주가가 하락합니다.                                                  " << endl;
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
	cout << "현재 쌓인 돈 : " << m_case->getSum() << endl;
	gotoxy(10, 7);
	cout << "1. 자동차" << '\t' << '\t' << "2. 집" << '\t'<< '\t' << "3. 빌딩"<<'\t'<<'\t'<<"4. 배"<<'\t'<< '\t' << "5. 전용기" << endl;
	gotoxy(10, 8);
	cout << "60만원" << '\t' << '\t' << "70만원" << '\t' << '\t' << "100만원" << '\t' << '\t' << "80만원" << '\t' << '\t' << "90만원" << endl;
	//디자인 추가
	gotoxy(10, 10);
	cout << "무엇을 구매하시겠습니까?  ==>  ";
	int choice;
	cin >> choice;
	switch (choice) {
	case 1:
		if (m_case->getSum() < car) {
			gotoxy(10, 12);
			cout << "돈이 부족합니다. 종료합니다." << endl;
			break;
		}
		else
		{
			gotoxy(10, 12);
			cout << "                ■■■■■" << endl;
			gotoxy(10, 13);
			cout << "               ■■▦▦▦■" << endl;
			gotoxy(10, 14);
			cout << "               ■■▦▦▦▦■" << endl;
			gotoxy(10, 15);
			cout << "               ■■▦▦▦▦■" << endl;
			gotoxy(10, 16);
			cout << "     ■■■■■■■■■■■■■■■■■" << endl;
			gotoxy(10, 17);
			cout << "  〓■■■■■■■■■■■■■■■■■■" << endl;
			gotoxy(10, 18);
			cout << "      ■■■○○○■■■■○○○■■■■" << endl;
			gotoxy(10, 19);
			cout << "         ■○＠＠○■■■○＠＠○■■" << endl;
			gotoxy(10, 20);
			cout << "            ○○○        ○○○" << endl;
			gotoxy(10, 22);
			cout << "자동차를 구매하셨습니다." << endl;
			m_case->minSum(car);
			break;
		}
	case 2:
		if (m_case->getSum() < house) {
			gotoxy(10, 12);
			cout << "돈이 부족합니다. 종료합니다." << endl;
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
			cout << "집을 구매하셨습니다." << endl;
			m_case->minSum(house);
			break;
		}
	case 3:
		if (m_case->getSum() < building) {
			gotoxy(10, 12);
			cout << "돈이 부족합니다. 종료합니다." << endl;
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
			cout << "빌딩을 구매하셨습니다." << endl;
			m_case->minSum(building);
			break;
		}
	case 4:
		if (m_case->getSum() < ship) {
			gotoxy(10, 12);
			cout << "돈이 부족합니다. 종료합니다." << endl;
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
			cout << "배를 구매하셨습니다." << endl;
			m_case->minSum(ship);
			break;
		}
	case 5:
		if (m_case->getSum() < airplane) {
			gotoxy(10, 12);
			cout << "돈이 부족합니다. 종료합니다." << endl;
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
			cout << "전용기를 구매하셨습니다." << endl;
			m_case->minSum(airplane);
			break;
		}
	default:
		gotoxy(10, 12);
		cout << "잘못된 입력입니다. 종료합니다." << endl;
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
	cout << "<< 테트코인 세계에 오신 것을 환영합니다~!~!~! >>" << endl;
	SetColor(RED);
	gotoxy(10, 12);
	cout << "1. 코인 채굴하기" << endl;
	gotoxy(10, 14);
	cout << "2. 상점" << endl;
	gotoxy(10, 16);
	cout << "3. 종료하기" << endl;

	gotoxy(10, 18);
	cout << "입력하세용 -> ";
	int a;
	cin >> a;
	switch (a)
	{
	case 1:
		system("cls");
		input_data();
		show_total_block();//테트리스 테두리 모양
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
		cout << "종료합니다." << endl;
		system("cls");
		return 0;
	default:
		gotoxy(10, 22);
		cout << "잘못 입력하셨습니다." << endl;
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
					cout << "■";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << m_case->getSum() << "원 상승 예정";
	}
	else if (plus == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gotoxy(55 + 2 * i, 10 + j);
				if (up[j][i] == 1) {
					cout << "■";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << sum << "원 상승";
	}
	else if (plus == 3) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gotoxy(55 + 2 * i, 10 + j);
				if (down[j][i] == 1) {
					cout << "■";
				}
				else {
					cout << "  ";
				}
			}
		}
		//cout << money << "원 하락";
	}

}


int Block::rotate_block()
{
	char keytemp = 0;

	for (int i = 1; 1; i++)//무한 증가
	{
		if (_kbhit())//방향키 조작
		{
			keytemp = _getche();
			if (keytemp == EXT_KEY)
			{
				keytemp = _getche();
				switch (keytemp)
				{
				case KEY_UP:      //회전하기

					if (strike_check(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0)//회전 가능한지 살펴본다.
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_angle = (block_angle + 1) % 4;
						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_LEFT:      //왼쪽으로 이동
					if (block_x > 1)
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_x--;
						if (strike_check(block_shape, block_angle, block_x, block_y) == 1)//이동했을 때 블록이 존재한다면
							block_x++;//다시 원상복구

						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_RIGHT:      //오른쪽으로 이동

					if (block_x < 14)
					{
						erase_cur_block(block_shape, block_angle, block_x, block_y);
						block_x++;
						if (strike_check(block_shape, block_angle, block_x, block_y) == 1)
							block_x--;
						show_cur_block(block_shape, block_angle, block_x, block_y);
					}
					break;
				case KEY_DOWN:      //아래로 이동
					is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
					show_cur_block(block_shape, block_angle, block_x, block_y);
					break;
				}
			}
			if (keytemp == 27) // esc를 누르면 게임중에 잔상이 남는 것을 없앰
			{
				erase_cur_block(block_shape, block_angle, block_x, block_y);
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}

			if (keytemp == 32)   //스페이스바를 눌렀을때
			{
				while (is_gameover == 0)
				{
					is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
				}
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}
		}
		if (i%stage_data[level].speed == 0)//단계별 속도조절
		{
			if (keytemp == 27) // esc를 누르면 게임중에 잔상이 남는 것을 없앰
			{
				erase_cur_block(block_shape, block_angle, block_x, block_y);
				show_cur_block(block_shape, block_angle, block_x, block_y);
			}

			is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);//블록들이 한줄씩 내려옴

			show_cur_block(block_shape, block_angle, block_x, block_y);
		}

		//if (stage_data[level].clear_line <= m_case->getlines())   // 없앤 줄의 수가 없애야 하는 줄의 수보다 크거나 같으면 스테이지 클리어
		//														  // *** 여러 줄이 한번에 없어져서 같은 경우를 지나치게 되면 스테이지 클리어를 할 수 없기 때문에 ==을 <=로 수정 ***
		//{
		//	m_case->deleteLC();
		//	gotoxy(10, 26);
		//	cout << level + 1 << "단계의 목표 줄 수를 달성하였습니다." << endl;
		//	Sleep(700);
		//	is_gameover = askEnd();
		//	level++; // 레벨업
		//	count = 0;
		//	/*	for (int i = 0; i < 20; i++) //쌓여져있는 블록 리셋 구문
		//	{
		//	for (int j = 1; j < 13; j++)
		//	{
		//	if (total_block[i][j] == 1)
		//	total_block[i][j] = 0;
		//	}
		//	}*/

		//	show_total_block(); // 전체 블럭, 외벽을 화면에 표시한다.
		//	m_case->show_gamestat(level); // 오른쪽에 게임 단계, 점수, 줄 수 보여주기
		//	m_case->show_next_block(block_shape); // 블럭모양을 보여줌


		//}
		if (m_case->getMoney() >= (level + 1) * 300000) {

			m_case->deleteLC();
			gotoxy(10, 26);
			cout << level + 1 << "단계의 목표 금액을 달성하였습니다." << endl;
			Sleep(700);

			is_gameover = askEnd();
			level++; // 레벨업
			count = 0;
			//	for (int i = 0; i < 20; i++)
			//	 {
			//		 for (int j = 1; j < 13; j++)
			//			 {
			//				 if (total_block[i][j] == 1)
			//				 total_block[i][j] = 0;
			//				}
			//	 }

			show_total_block(); // 전체 블럭, 외벽을 화면에 표시한다.
			m_case->show_gamestat(level); // 오른쪽에 게임 단계, 점수, 줄 수 보여주기
			m_case->show_next_block(block_shape); // 블럭모양을 보여줌
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
				cout << "모두 클리어하셨습니다! 축하합니다~!" << endl;
				gotoxy(10, 10);
				cout << "메인으로 돌아갑니다." << endl;
				return 0;
			}
			m_case->show_gameover(is_gameover, level);
			count = 0;
			int m = 50000;
			m_case->setMoney(m);
			SetColor(GRAY);
			break;
		}
		//잔상이 남는다면 키를 입력받을 때 수정
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
	cout << "종료됩니다. 안녕~~" << endl;
}

