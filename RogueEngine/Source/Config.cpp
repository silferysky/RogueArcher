#include "Config.h"
#include "FileIO.h"
#include "Main.h"

namespace Rogue
{
	REConfig::REConfig() :
		m_title{ "" }, m_byte{ 0 }, m_flags{ 0 }, m_FPS{ 1 / 60 }, m_height{ 0 }, m_sound{ true }, m_x{ 0 }, m_y{ 0 }
	{}

	void REConfig::ConfigInit()
	{
		RESerialiser Serialiser;
		rapidjson::Document Windows = Serialiser.DeserialiseFromFile("Resources/Config.json");

		m_x = Windows["x"].GetInt();
		m_y = Windows["y"].GetInt();
		m_height = Windows["height"].GetInt();
		m_width = Windows["width"].GetInt();
		m_byte = Windows["byte"].GetInt();
		m_flags = Windows["flags"].GetInt();
		m_title = Windows["title"].GetString();
		m_sound = Windows["sound"].GetBool();
		m_FPS = Windows["FPS"].GetInt();
	}

	std::string REConfig::GetTitle() const
	{
		return m_title;
	}

	bool REConfig::GetSound() const
	{
		return m_sound;
	}


	int REConfig::GetX() const
	{
		return m_x;
	}

	int REConfig::GetY() const
	{
		return m_y;
	}

	int REConfig::GetHeight() const
	{
		return m_height;
	}

	int REConfig::GetWidth() const
	{
		return m_width;
	}

	BYTE REConfig::GetByte() const
	{
		return m_byte;
	}

	int REConfig::GetFlags() const
	{
		return m_flags;
	}

	int REConfig::GetFPS() const
	{
		return m_FPS;
	}

	void REConfig::SetX(int newx)
	{
		m_x = newx;
	}

	void REConfig::SetY(int newy)
	{
		m_y = newy;
	}

	void REConfig::SetHeight(int newheight)
	{
		m_height = newheight;
	}

	void REConfig::SetWidth(int newwidth)
	{
		m_width = newwidth;
	}

	void REConfig::SetByte(int newbyte)
	{
		m_byte = newbyte;
	}

	void REConfig::SetFlags(int newflags)
	{
		m_flags = newflags;
	}

	void REConfig::SetFPS(int FPSset)
	{
		m_FPS = FPSset;
	}

	void REConfig::SetSound(bool NewSound)
	{
		m_sound = NewSound;
	}


}

