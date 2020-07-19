#pragma once
#include "Drawable.h"

class ScoreDrawer : public Drawable
{
public:
	int Score;
	int DrawOrder() const override;
	void Draw(const std::unique_ptr<wchar_t[]>& screen, int screenWidth) override;
};
