
#include <vector>
#include "PlayGround.h"

PlayGround::PlayGround(int width, int height)
	: m_Width(width), m_Height(height)
{
	m_Field = std::make_unique<char[]>(m_Width * m_Height);

	// Put empty space ' ' at every position except borders, which are '#'
	
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			m_Field[y * m_Width + x] = (x == 0 || x == m_Width - 1 || y == m_Height - 1) ? '#' : ' ';
		}
	}
}

int PlayGround::DrawOrder() const
{
	return 0;
}

void PlayGround::Draw(const std::unique_ptr<wchar_t[]>& screen, int screenWidth)
{
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			screen[(y + 2) * screenWidth + (x + 2)] = m_Field[y * m_Width + x];
		}
	}

	for (auto& v : m_ConstructedLines)
	{
		for (int px = 1; px < m_Width - 1; px++)
		{
			for (int py = v; py > 0; py--)
				m_Field[py * m_Width + px] = m_Field[(py - 1) * m_Width + px];
			m_Field[px] = ' ';
		}
	}

	m_ConstructedLines.clear();
}

int PlayGround::Width() const
{
	return m_Width;
}

int PlayGround::Height() const
{
	return m_Height;
}

bool PlayGround::CheckPiecePlacement(const std::shared_ptr<Tetromino>& tetromino) const
{
	// All Field cells >0 are occupied
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// Get index into piece
			const int tetrominoIndex = tetromino->GetIndexWithRotation(x, y);

			// Get index into field
			const int fieldIndex = (tetromino->PositionY() + y) * m_Width + (tetromino->PositionX() + x);

			if (fieldIndex < 0 || fieldIndex > m_Width * m_Height)
			{
				return false;
			}

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (tetromino->PositionX() + x >= 0 && tetromino->PositionX() + x < m_Width)
			{
				if (tetromino->PositionY() + y >= 0 && tetromino->PositionY() + y < m_Height)
				{
					// In Bounds so do collision check
					if (tetromino->Shape()[tetrominoIndex] != L'.' && m_Field[fieldIndex] != ' ')
						return false; // fail on first hit
				}
			}
		}
	}

	return true;

}

int PlayGround::LockPiece(const std::shared_ptr<Tetromino>& tetromino)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			int index = tetromino->GetIndexWithRotation(px, py);

			if (tetromino->Shape()[index] != L'.')
			{
				m_Field[(tetromino->PositionY() + py) * m_Width + (tetromino->PositionX() + px)] =
					tetromino->RenderChar();
			}
		}
	}

	int constructedLines = 0;
	
	// Check every row
	for (int row = 3; row < m_Height - 1; ++row)
	{
		bool isLine = true;

		for (int column = 1; column < m_Width - 1; ++column)
		{
			if (m_Field[row * m_Width + column] == ' ')
			{
				isLine = false;
				break;
			}
		}

		if (isLine)
		{
			constructedLines++;
			
			// Remove Line, set to =
			for (int px = 1; px < m_Width - 1; px++)
				m_Field[row * m_Width + px] = '=';

			m_ConstructedLines.push_back(row);
		}
	}

	return constructedLines;
}
