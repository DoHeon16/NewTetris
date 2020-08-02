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

	ConsoleCursor.bVisible = bVisible; // true 보임, false 안보임

	ConsoleCursor.dwSize = 1; // 커서사이즈

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor); // 설정
}
