#include "Display.h"
#include <memory>

Display::Display()
{
	// initialization 
	memset(m_screen, 0, sizeof(m_screen));
	memset(m_screenBackup, 0, sizeof(m_screen));
}

void Display::Update(LPDIRECT3DTEXTURE8 texture)
{
	// updating texture data
	
	D3DLOCKED_RECT locked_rect;
	RECT lock_rect;
	lock_rect.top = 0;
	lock_rect.left = 0;
	lock_rect.bottom = 32;
	lock_rect.right = 64;

	texture->LockRect(0, &locked_rect, &lock_rect, 0);
	uint32_t* colors = (uint32_t*)locked_rect.pBits;
	
	for (int i = 0; i < DISPLAY_SIZE; i++)
	{
		colors[i] = m_screen[i] ? 0xFFFFFFFF : 0xFF101010;
	}

	texture->UnlockRect(0);
}

void Display::Render()
{
	// rendering the plane
	//glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
}
