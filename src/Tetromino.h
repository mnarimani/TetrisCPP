#pragma once

#include <xstring>
#include "Drawable.h"
#include <array>

class Tetromino: public Drawable
{
public:
	Tetromino(const std::wstring &shape, int posX, int posY, char renderChar);
	int DrawOrder() const override;
	void Draw(const std::unique_ptr<wchar_t[]> &screen, int screenWidth) override;
	int GetIndexWithRotation(int x, int y) const;

	int PositionX() const;
	int PositionY() const;
	int Rotation() const;
	char RenderChar() const;
	std::wstring Shape() const;
	void SetRotation(int mRotation);
	void SetPositionX(int mPositionX);
	void SetPositionY(int mPositionY);

	static std::shared_ptr<Tetromino> Create(int &&positionX, int &&positionY, char &&renderChar);

private:
	static std::array<std::wstring, 7> s_TetrominoShapes;

	const std::wstring m_Shape;
	const char m_RenderChar;
	int m_Rotation;
	int m_PositionX, m_PositionY;

};