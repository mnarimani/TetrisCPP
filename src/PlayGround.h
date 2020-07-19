#pragma once

#include "Drawable.h"
#include "Tetromino.h"

class PlayGround : public Drawable
{
public:
	PlayGround(int width, int height);
	int DrawOrder() const override;
	void Draw(const std::unique_ptr<wchar_t[]> &screen, int screenWidth) override;

	bool CheckPiecePlacement(const std::shared_ptr<Tetromino>& tetromino) const;
	int LockPiece(const std::shared_ptr<Tetromino>& tetromino);

	int Width() const;
	int Height() const;
private:
	int m_Width, m_Height;
	std::vector<int> m_ConstructedLines;
	std::unique_ptr<char[]> m_Field;
};
