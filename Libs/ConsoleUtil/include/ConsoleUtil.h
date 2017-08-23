#pragma once
#include <string>

namespace ConsoleUtil
{
	enum Colour : int
	{
		Black = 0x00,
		DarkBlue = 0x01,
		DarkGreen = 0x02,
		DarkCyan = 0x03,
		DarkRed = 0x04,
		DarkMagenta = 0x05,
		DarkYellow = 0x06,
		LightGrey = 0x07,
		DarkGrey = 0x08,
		Blue = 0x09,
		Green = 0x0A,
		Cyan = 0x0B,
		Red = 0x0C,
		Magenta = 0x0D,
		Yellow = 0x0E,
		White = 0x0F
	};

	enum Alignment {Left, Right, Centre};

	struct TableCell
	{
		std::string data = "";
		ConsoleUtil::Alignment alignment = ConsoleUtil::Alignment::Right;
		ConsoleUtil::Colour colour = ConsoleUtil::Colour::LightGrey;
	};

	//Sets the console colour
	//Foreground is the colour of the text
	void SetColour(Colour foreground, Colour background = Colour::Black);

	//Sets the console window to the maximum size supported
	//The buffer is resized to be the same size of the window
	void SetToMaxSize();

	//Sets the position of the cursor
	//Note top left is {0,0}
	void SetCursorPosition(int x, int y);

	//Sets whether the cursor is visible
	void SetCursorVisible(bool visible);

	//Resets the console colour to light grey text and black background
	void ResetColour();

	//Gets the maximum width of the console window
	int GetMaxWidth();

	//Gets the maximum height of the console window
	int GetMaxHeight();

	//Prints a char
	//Optionally prints out the char multiple times
	void PrintChars(char c, int numOfChars = 1, Colour colour = Colour::LightGrey);

	//Prints an integer
	//The integer can be given some padding to ensure it takes up a mimimum area of space
	//eg a padding of 4 for the numbers 4 15 182 each on separate lines with a right allignment would produce
	//   4
	//  15
	// 182
	void PrintInt(int num, Colour colour = Colour::LightGrey, int pad = 0, char padChar = ' ', Alignment align = Alignment::Right);

	//Prints a string
	//For info on padding see PrintInt
	void PrintString(const std::string& s, Colour colour = Colour::LightGrey, int pad = 0, char padChar = ' ', Alignment align = Alignment::Right);

	//Prints out a table with default formatteding
	//Assumes std::string* is an array of length rows * cols
	void PrintTable(const std::string* table, int rows, int cols);

	//Prints out a table with each cell individually formatted
	//Assumes TableCell* is an array of length rows * cols
	void PrintTable(const TableCell* table, int rows, int cols);

	void EndLine();

	//Clears the entire console
	void Clear();

	//Outputs "Press any key to continue" and waits for input
	void Pause();

	//Returns true if a key has been pressed
	bool IsAnyKeyPressed();

	int GetKeyPressed();
}