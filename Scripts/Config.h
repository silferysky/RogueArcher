#pragma once
#include "FileIO.h"

class REConfig
{
public:
	void ConfigInit();
	//const char* GetWindow();
	int GetX();
	int GetY();
	int GetHeight();
	int GetWidth();
	int GetByte();
	int GetFlags();
	float GetFPS();
	int SetX(int newx);
	int SetY(int newy);
	int SetHeight(int newheight);
	int SetWidth(int newwidth);
	int SetByte(int newbyte);
	int SetFlags(int newflags);
	float SetFPS(int FPSset);
	
private:
	float FPS = 1 / 60;
	int x = 0, y = 0, height = 0, width = 0, byte = 0, flags = 0;
	//const char* Window = "";
};