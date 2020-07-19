
#include <algorithm>
#include "Renderer.h"

Renderer::Renderer(int width, int height)
	: m_Width(width), m_Height(height)
{
	m_Screen = std::make_unique<wchar_t[]>(m_Width * m_Height);
	for (int i = 0; i < m_Width * m_Height; i++) {
		m_Screen[i] = L' ';
	}
	m_ConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(m_ConsoleHandle);
}

Renderer::~Renderer()
{
	CloseHandle(m_ConsoleHandle);
}

void Renderer::AddDrawable(const std::shared_ptr<Drawable> &drawable)
{
	// We don't want duplicate elements in our drawables list. So we do a quick check before adding it to list.
	// Alternatively, I could use a separate hashset to increase performance, but we only have 2-3 objects (maximum) every frame.
	// So it didn't worth it.
	const auto result = std::find_if(
		m_Drawables.begin(),
		m_Drawables.end(),
		[&drawable](const std::shared_ptr<Drawable> &x) -> bool
		{
			return x.get() == drawable.get();
		});

	// end() method refers to one position AFTER the last element.
	// So it doesn't make a problem if duplicate item is at the end of the list.
	if (result == m_Drawables.end()) {
		m_Drawables.push_back(drawable);
		
		std::sort(m_Drawables.begin(),
				  m_Drawables.end(),
				  [](const std::shared_ptr<Drawable> &d1, const std::shared_ptr<Drawable> &d2) -> bool
				  {
					  return d1->DrawOrder() < d2->DrawOrder();
				  });
	}
}
void Renderer::RemoveDrawable(const std::shared_ptr<Drawable> &drawable)
{
	// Note that we cannot directly remove drawable object form m_Drawables because shared_ptr objects might be different.
	// So we need to check the underlying pointer. Hence the remove_if.
	m_Drawables.erase(
		std::remove_if(
			m_Drawables.begin(),
			m_Drawables.end(),
			[&drawable](const std::shared_ptr<Drawable> &x) -> bool
			{
				return x.get() == drawable.get();
			}),
		m_Drawables.end());
}
void Renderer::DrawAll()
{
	// All drawable objects are sorted by their draw orders.
	// So here we only need to draw them one by one.
	for (auto &d : m_Drawables) {
		d->Draw(m_Screen, m_Width);
	}

	static DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterW(m_ConsoleHandle, m_Screen.get(), m_Width * m_Height, {0, 0}, &dwBytesWritten);
}
