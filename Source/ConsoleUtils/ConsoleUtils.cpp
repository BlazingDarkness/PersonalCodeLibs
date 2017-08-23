#include "ConsoleUtils.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>

inline int Max(int a, int b)
{
	return a > b ? a : b;
}

void ConsoleUtils::SetColour(Colour foreground, Colour background)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(foreground + (background << 4)));
}

void ConsoleUtils::ResetColour()
{
	SetColour(Colour::LightGrey, Colour::Black);
}

void ConsoleUtils::SetToMaxSize()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD maxSize = GetLargestConsoleWindowSize(h);

	COORD bufferSize;
	bufferSize.X = maxSize.X;
	bufferSize.Y = maxSize.Y < 200 ? 200 : maxSize.Y;
	SetConsoleScreenBufferSize(h, bufferSize);

	/*SMALL_RECT consoleSize;
	consoleSize.Top = 0;
	consoleSize.Left = 0;
	consoleSize.Bottom = maxSize.Y - 1;
	consoleSize.Right = maxSize.X - 1;

	if (!SetConsoleWindowInfo(h, TRUE, &consoleSize))
	{
		std::cout << "FAILED" << std::endl;
	}*/

	CONSOLE_FONT_INFO fontInfo;
	if (GetCurrentConsoleFont(h, FALSE, &fontInfo))
	{
		SHORT pixelWidth = fontInfo.dwFontSize.X * maxSize.X;
		SHORT pixelHeight = fontInfo.dwFontSize.Y * maxSize.Y;

		if (!MoveWindow(GetConsoleWindow(), 0, 0, pixelWidth, pixelHeight, TRUE))
		{
			std::cout << "FAILED" << std::endl;
		}
	}
}

void ConsoleUtils::SetCursorPosition(int x, int y)
{
	COORD p = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void ConsoleUtils::SetCursorVisible(bool visible)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(handle, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(handle, &cursorInfo);
}

int ConsoleUtils::GetMaxWidth()
{
	return static_cast<int>(GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE)).X);
}

int ConsoleUtils::GetMaxHeight()
{
	return static_cast<int>(GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE)).Y);
}

void ConsoleUtils::PrintChars(char c, int numOfChars, Colour colour)
{
	SetColour(colour);
	for (int i = 0; i < numOfChars; ++i)
	{
		std::cout << c;
	}
}

void ConsoleUtils::PrintInt(int num, Colour colour, int pad, char padChar, Alignment align)
{
	std::string s = std::to_string(num);

	PrintString(s, colour, pad, padChar, align);
}

void ConsoleUtils::PrintString(const std::string& s, Colour colour, int pad, char padChar, Alignment align)
{
	SetColour(colour);

	int length = static_cast<int>(s.size());
	int before = 0;
	int after = 0;

	if (pad > length)
	{
		//calc padding amount
		switch (align)
		{
		case ConsoleUtils::Alignment::Right:
			before = pad - length;
			break;
		case ConsoleUtils::Alignment::Left:
			after = pad - length;
			break;
		case ConsoleUtils::Alignment::Centre:
			before = (pad - length) / 2;
			after  = pad - (length + before);
			break;
		}

		//Output with padding
		for (int i = 0; i < before; ++i)
		{
			std::cout << padChar;
		}
		std::cout << s;
		for (int i = 0; i < after; ++i)
		{
			std::cout << padChar;
		}
	}
	else
	{
		std::cout << s;
	}
}

void ConsoleUtils::PrintTable(const std::string* table, int rows, int cols)
{
	int* columnWidths = new int[cols];
	int lineLength = cols * 3 + 1;

	//calc size of table and width of each column
	for (int column = 0; column < cols; ++column)
	{
		columnWidths[column] = 0;
		for (int row = 0; row < rows; ++row)
		{
			columnWidths[column] = Max(static_cast<int>(table[row * cols + column].size()), columnWidths[column]);
		}
		lineLength += columnWidths[column];
	}

	//output table
	for (int row = 0; row < rows; ++row)
	{
		//Row dividers before and after the first row
		if (row < 2)
		{
			ConsoleUtils::PrintChars('-', lineLength);
			ConsoleUtils::EndLine();
		}
		//Column dividers and cell data output
		ConsoleUtils::PrintString("| ");
		for (int column = 0; column < cols; ++column)
		{
			ConsoleUtils::PrintString(table[row * cols + column], ConsoleUtils::LightGrey, columnWidths[column]);
			ConsoleUtils::PrintString(" | ");
		}
		ConsoleUtils::EndLine();
	}

	//Row divider after the last row
	ConsoleUtils::PrintChars('-', lineLength);
	ConsoleUtils::EndLine();

	delete[] columnWidths;
}

void ConsoleUtils::PrintTable(const TableCell* table, int rows, int cols)
{
	int* columnWidths = new int[cols];
	int lineLength = cols * 3 + 1;

	//calc size of table and width of each column
	for (int column = 0; column < cols; ++column)
	{
		columnWidths[column] = 0;
		for (int row = 0; row < rows; ++row)
		{
			columnWidths[column] = Max(static_cast<int>(table[row * cols + column].data.size()), columnWidths[column]);
		}
		lineLength += columnWidths[column];
	}

	//output table
	for (int row = 0; row < rows; ++row)
	{
		//Row dividers before and after the first row
		if (row < 2)
		{
			ConsoleUtils::PrintChars('-', lineLength);
			ConsoleUtils::EndLine();
		}
		//Column dividers and cell data output
		ConsoleUtils::PrintString("| ");
		for (int column = 0; column < cols; ++column)
		{
			ConsoleUtils::PrintString(table[row * cols + column].data, table[row * cols + column].colour, columnWidths[column], ' ', table[row * cols + column].alignment);
			ConsoleUtils::PrintString(" | ");
		}
		ConsoleUtils::EndLine();
	}

	//Row divider after the last row
	ConsoleUtils::PrintChars('-', lineLength);
	ConsoleUtils::EndLine();

	delete[] columnWidths;
}

void ConsoleUtils::EndLine()
{
	std::cout << std::endl;
}

void ConsoleUtils::Clear()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);

}

void ConsoleUtils::Pause()
{
	std::cout << "Press a key to continue" << std::endl;
	_getch();
}

bool ConsoleUtils::IsAnyKeyPressed()
{
	return _kbhit() != 0;
}

int ConsoleUtils::GetKeyPressed()
{
	return _getch();
}

