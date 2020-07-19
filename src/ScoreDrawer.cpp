#include "ScoreDrawer.h"

int ScoreDrawer::DrawOrder() const
{
	return 3;
}

void ScoreDrawer::Draw(const std::unique_ptr<wchar_t[]>& screen, int screenWidth)
{
	swprintf_s(&screen[2 * screenWidth + 18], 16, L"SCORE: %8d", Score);
}
