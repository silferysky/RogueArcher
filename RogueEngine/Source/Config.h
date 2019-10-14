#pragma once
#include "FileIO.h"
#include "Main.h"
class REConfig
{
public:
	REConfig() = default;
	~REConfig() = default;
	void ConfigInit();

	std::string GetTitle();
	bool GetSound();
	int GetX();
	int GetY();
	int GetHeight();
	int GetWidth();
	BYTE GetByte();
	int GetFlags();
	float GetFPS();

	int SetX(int newx);
	int SetY(int newy);
	int SetHeight(int newheight);
	int SetWidth(int newwidth);
	int SetByte(int newbyte);
	int SetFlags(int newflags);
	float SetFPS(int FPSset);
	bool SetSound(bool NewSound);

private:
	float FPS = 1 / 60;
	int x = 0, y = 0, height = 0, width = 0, flags = 0;
	int byte = 0;
	std::string title = "";
	bool sound = true;
};