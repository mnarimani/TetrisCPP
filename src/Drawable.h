#pragma once

#include <memory>

class Drawable
{
public:
	virtual int DrawOrder() const = 0;
	virtual void Draw(const std::unique_ptr<wchar_t[]>& screen, int screenWidth) = 0;
};
