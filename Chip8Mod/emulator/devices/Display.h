#pragma once
#include <cstdint>
#include <memory>
#include <d3d8.h>
#include <d3d8types.h>

#define SPRITE_WIDTH 8

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT)

class Display
{
public:
	Display();
	Display(const Display&) = delete;
	~Display() = default;

	// Clears the screen
	inline void Clear()
	{
		memset(m_screen, 0, sizeof(m_screen));
	}

	// Backups screen to buffer
	inline void Backup()
	{
		memcpy(m_screenBackup, m_screen, sizeof(m_screen));
	}

	// Restores screen state from backup
	inline void Restore()
	{
		memcpy(m_screen, m_screenBackup, sizeof(m_screen));
	}

	/* Returns screen */
	inline uint8_t* GetScreen() { return m_screen; };

	/* Updates texture */
	void Update(LPDIRECT3DTEXTURE8 texture);

	/* Render display */
	void Render();

private:
	uint8_t m_screen[DISPLAY_SIZE];
	uint8_t m_screenBackup[DISPLAY_SIZE];
};

