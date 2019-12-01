/* Start Header ************************************************************************/
/*!
\file           Config.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Config

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <string>
#include "Windows.h"

namespace Rogue
{
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
}