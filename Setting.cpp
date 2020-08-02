#include "setting.h"

Setting::Setting()
	:x(0), y(0), color(0)
{
	SetCursor(false);
}

Setting::~Setting()
{
}

int Setting::gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.Y = y;
	pos.X = x;
	SetConsoleCursorPosition(hConsole, pos);
	return 0;
}

void Setting::SetColor(int color)
{
	static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std_output_handle, color);
}

void Setting::SetCursor(bool bVisible)
{
	CONSOLE_CURSOR_INFO ConsoleCursor;

	ConsoleCursor.bVisible = bVisible; // true ����, false �Ⱥ���

	ConsoleCursor.dwSize = 1; // Ŀ��������

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor); // ����
}
