#pragma once

#include "fmod.h"

class Sound
{
private:
	 bool _soundon;//check if sound is on
	 bool _possible;//check if possible to play sound
	 char* _currentsound;//check for current played sound

	 FMOD_RESULT _result;
	 FMOD_SYSTEM *_system;
	 FMOD_SOUND *_sound;
	 FMOD_CHANNEL *_channel;

public:
	 Sound();
	 Sound(Sound&);
	 Sound& operator = (Sound&);
	 ~Sound();
	 float Timer = 0.0f;
	 char PlayCounter = 0;
	 float PlayTimer = 0.0f;
	 char PlayCap = 0;
   void CreateOneShot(const char * filename, FMOD_SOUND **sound)
	 void Create(const char* filename, char counterCap, float playTimer);
	 void init(); //initialises sound
	 void setvolume(float volume); //sets volume
	 void load(const char* tFile); //loads the file
	 void unload(void);//unloads the file
	 void playsound(bool pause = false);//plays the sound
	 void soundupdate();//updates the sound
	 bool getsound();//gets the current sound
	 void playsoundloop(bool pause = false);//plays sounds in the loop
	 void togglesound();//toggles the current sound
	 void togglepause();//toggle pause for the current sound

	 void setpause(bool pause);//pauses current sound
	 void setsound(bool sound);//set current sound

};