#pragma once

#include <memory>
#include <vector>
#include "Drawable.h"
#include <Windows.h>

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();
	void AddDrawable(const std::shared_ptr<Drawable> &drawable);
	void RemoveDrawable(const std::shared_ptr<Drawable> &drawable);
	void DrawAll();
private:
	int m_Width, m_Height;
	HANDLE m_ConsoleHandle;
	std::unique_ptr<wchar_t[]> m_Screen;
	std::vector<std::shared_ptr<Drawable>> m_Drawables;
};