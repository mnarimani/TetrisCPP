

#include "Tetromino.h"

std::array<std::wstring,7> Tetromino::s_TetrominoShapes = {
	L"..X...X...X...X.",
	L"..X..XX...X.....",
	L".....XX..XX.....",
	L"..X..XX..X......",
	L".X...XX...X.....",
	L".X...X...XX.....",
	L"..X...X..XX....."
};

Tetromino::Tetromino(const std::wstring &shape, int posX, int posY, char renderChar)
	: m_Shape(shape),
	  m_PositionX(posX),
	  m_PositionY(posY),
	  m_RenderChar(renderChar),
	  m_Rotation(0)
{}

int Tetromino::DrawOrder() const
{
	return 1;
}

void Tetromino::Draw(const std::unique_ptr<wchar_t[]> &screen, int screenWidth)
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (m_Shape[GetIndexWithRotation(x, y)] != L'.')
				screen[(m_PositionY + y + 2) * screenWidth + (m_PositionX + x + 2)] = m_RenderChar;
		}
	}
}
int Tetromino::GetIndexWithRotation(int x, int y) const
{
	int pi = 0;
	switch (m_Rotation % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = y * 4 + x;                // 4  5  6  7
		break;                        // 8  9 10 11
		//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + y - (x * 4);        //13  9  5  1
		break;                        //14 10  6  2
		//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (y * 4) - x;        //11 10  9  8
		break;                        // 7  6  5  4
		// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - y + (x * 4);        // 2  6 10 14
		break;                        // 1  5  9 13
	}                                    // 0  4  8 12

	return pi;
}
std::shared_ptr<Tetromino> Tetromino::Create(int &&positionX, int &&positionY, char &&renderChar)
{
	unsigned int length = Tetromino::s_TetrominoShapes.size();
	int index = (rand() % length);
	std::wstring &shape = Tetromino::s_TetrominoShapes [index];
	return std::make_shared<Tetromino>(shape,
									   std::forward<int>(positionX),
									   std::forward<int>(positionY),
									   std::forward<char>(renderChar));
}
int Tetromino::PositionX() const
{
	return m_PositionX;
}
int Tetromino::PositionY() const
{
	return m_PositionY;
}
std::wstring Tetromino::Shape() const
{
	return m_Shape;
}
void Tetromino::SetRotation(int mRotation)
{
	m_Rotation = mRotation;
}
void Tetromino::SetPositionX(int mPositionX)
{
	m_PositionX = mPositionX;
}
void Tetromino::SetPositionY(int mPositionY)
{
	m_PositionY = mPositionY;
}
int Tetromino::Rotation() const
{
	return m_Rotation;
}
char Tetromino::RenderChar() const
{
	return m_RenderChar;
}
