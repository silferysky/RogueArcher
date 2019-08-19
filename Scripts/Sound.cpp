#pragma once
#include "Sound.h"

//constructor
Sound::Sound()
{
	_soundon = false;
	_possible = false;
	_currentsound = false;
}
//copy constructor
Sound::Sound(Sound& copy)
{
	_soundon = copy._soundon;
	_possible = copy._possible;
	_currentsound = copy._currentsound;
}
//operator =
Sound& Sound::operator = (Sound& copy)
{
	_soundon = copy._soundon;
	_possible = copy._possible;
	_currentsound = copy._currentsound;
	return *this;
}
//destructor
Sound::~Sound()
{
	_soundon = false;
	_possible = false;
	_currentsound = false;
}

void Sound::Create(const char * filename, char counterCap, float playTimer)
{
	PlayCap = counterCap;
	PlayTimer = playTimer;
	FMOD_System_CreateSound(system, filename, FMOD_LOOP_OFF | FMOD_CREATESTREAM, 0, &fmodSound);
	FmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
}

void Sound::init()
{
	_possible = true;
	//create the sound system. If fails, sound is set to impossible
	_result = FMOD_System_Create(&_system);
	if (_result != FMOD_OK) 
	{
		_possible = false;
	}
	//if initialise the sound system. If fails, sound is set to impossible
	if (_possible)
	{
		_result = FMOD_System_Init(_system, 1000, FMOD_INIT_NORMAL, 0);
	}
	if (_result != FMOD_OK)
	{
		_possible = false;
	}
	//sets initial sound volume (mute)
	if (_possible)
	{
		FMOD_Channel_SetVolume(_channel, 0.0f);
	}
}
//sets the volume of the sound
void Sound::setvolume(float volume)
{
	if (_possible && _soundon && volume >= 0.0f && volume <= 1.0f) {
		FMOD_Channel_SetVolume(_channel, volume);
	}
}
//loads the sound
void Sound::load(const char * tFile)
{
	_currentsound = (char *)tFile;
	_soundon = true;
	_possible = true;
	_result = FMOD_Sound_Release(_sound);
	_result = FMOD_System_CreateStream(_system, _currentsound, FMOD_LOOP_OFF | FMOD_2D , 0, &_sound);
		//if failed to load, set sound to not possible
		if (_result != FMOD_OK) _possible = false;
}
//unloads the sound
void Sound::unload(void)
{
	if (_possible)
	{
		_result = FMOD_Sound_Release(_sound);
		FMOD_System_Release(_system);
	}
}
//plays the current sound 
void Sound::playsound(bool pause)
{
	if (_possible && _soundon) {
		_result = FMOD_System_PlaySound(_system, _sound,nullptr , pause, &_channel);
	}
}
//plays the current sound in a infinite loop
void Sound::playsoundloop(bool pause)
{
	if (_possible && _soundon) {
		_result = FMOD_System_PlaySound(_system, _sound, nullptr, pause, &_channel);
		FMOD_Channel_SetMode(_channel, FMOD_LOOP_NORMAL);
	}
}
//updates the sound
void Sound::soundupdate()
{
	FMOD_System_Update(_system);
}

//get current sound
bool Sound::getsound()
{
	return _soundon;
}
//pause the current sound
void Sound::setpause(bool pause)
{
	FMOD_Channel_SetPaused(_channel, pause);
}
//sets the current sound
void Sound::setsound(bool sound)
{
	_soundon = sound;
}
//toggles sound on or off
void Sound::togglesound()
{
	_soundon = !_soundon;
	if (_soundon == true) 
	{ 
		load(_currentsound); 
		playsound(); 
	}
	if (_soundon == false) 
	{
		unload(); 
	}
}
//pause the current sound
void Sound::togglepause()
{
	FMOD_BOOL p;
	FMOD_Channel_GetPaused(_channel, &p);
	FMOD_Channel_SetPaused(_channel, !p);
}

void Sound::CreateOneShot(const char * filename, FMOD_SOUND **sound)
{
	char * currentSound; //currently played sound
	currentSound = (char *)filename;
	if (possible && SoundOn) {
		if (*sound != NULL)
		{
			result = FMOD_Sound_Release(*sound);
		}
		result = FMOD_System_CreateSound(fmodsystem, currentSound, FMOD_LOOP_OFF | FMOD_2D, NULL, sound);
		if (result != FMOD_OK) possible = false;
	}
}
