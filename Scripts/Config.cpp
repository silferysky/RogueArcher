#include "Config.h"

void REConfig::ConfigInit()
{

	RESerialiser Serialiser;
	rapidjson::Document Windows = Serialiser.DeserialiseFromFile("Resources/Windows.json");
	const char* Window;
	x = Windows["x"].GetInt();
	y = Windows["y"].GetInt();
	height = Windows["height"].GetInt();
	width = Windows["width"].GetInt();
	byte = Windows["byte"].GetInt();
	flags = Windows["flags"].GetInt();
	title = Windows["title"].GetString();
	sound = Windows["sound"].GetBool();
}

char* REConfig::GetTitle()
{
	return const_cast<char*>(title);
}

bool REConfig::GetSound()
{
	return sound;
}


int REConfig::GetX()
{
	return x;
}

int REConfig::GetY()
{
	return y;
}

int REConfig::GetHeight()
{
	return height;
}

int REConfig::GetWidth()
{
	return width;
}

int REConfig::GetByte()
{
	return byte;
}

int REConfig::GetFlags()
{
	return flags;
}

float REConfig::GetFPS()
{
	return FPS;
}

int REConfig::SetX(int newx)
{
	x = newx;
	return x;
}

int REConfig::SetY(int newy)
{
	y = newy;
	return y;
}

int REConfig::SetHeight(int newheight)
{
	height = newheight;
	return height;
}

int REConfig::SetWidth(int newwidth)
{
	width = newwidth;
	return width;
}

int REConfig::SetByte(int newbyte)
{
	byte = newbyte;
	return byte;
}

int REConfig::SetFlags(int newflags)
{
	flags = newflags;
	return flags;
}

float REConfig::SetFPS(int FPSset)
{
	FPS = 1 / float(FPSset);
	return FPS;
}

bool REConfig::SetSound(bool NewSound)
{
	sound = NewSound;
	return NewSound;
}


