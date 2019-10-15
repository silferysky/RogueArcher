#pragma once
#include <string>
#include "Windows.h"

class REConfig
{
public:

	REConfig();

	// Init config
	void ConfigInit();

	// Getters
	std::string GetTitle() const;
	bool GetSound() const;
	int GetX() const;
	int GetY() const;
	int GetHeight() const;
	int GetWidth() const;
	BYTE GetByte() const;
	int GetFlags() const;
	int GetFPS() const;

	// Setters
	void SetX(int newx);
	void SetY(int newy);
	void SetHeight(int newheight);
	void SetWidth(int newwidth);
	void SetByte(int newbyte);
	void SetFlags(int newflags);
	void SetFPS(int FPSset);
	void SetSound(bool NewSound);

private:
	std::string m_title;
	int m_FPS;
	int m_x;
	int m_y;
	int m_height;
	int m_width;
	int m_flags;
	bool m_sound;
	BYTE m_byte;
};