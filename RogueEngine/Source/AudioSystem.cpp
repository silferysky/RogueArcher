/* Start Header ************************************************************************/
/*!
\file           AudioSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AudioSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "AudioSystem.h"
#include "REEngine.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "FMODHelper.h"

namespace Rogue
{
	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//

	AudioSystem::AudioSystem()
		: System(SystemID::id_AUDIOSYSTEM), m_muted{ false }
	{}

	void AudioSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_AUDIOSYSTEM, AudioSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<AudioEmitterComponent>());

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<AudioSystem>(signature);

		auto cameraSys = g_engine.m_coordinator.GetSystem<CameraSystem>();

		auto front = cameraSys->GetCameraFront();
		auto up = cameraSys->GetCameraUp();

		listenerFront = { front.x, front.x, front.z };
		listenerUp = { up.x, up.y, up.z };
	}

	void AudioSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Audio System");

		if (!g_engine.GetIsFocused())
		{
			m_muted = true;
			for (auto entity : m_entities)
			{
				g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound().Pause(m_muted);
			}
		}
		else
		{
			m_muted = false;
			for (auto entity : m_entities)
			{
				g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound().Pause(m_muted);
			}
		}
		for (auto entity : m_entities)
		{

			if (m_muted)
				break;

			auto& aEmitter = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity);

			if (!aEmitter.getIsScaling())
				continue;

			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_trackingTarget) && g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(m_trackingTarget))
			{
				auto sound = aEmitter.getSound();
				Vec2 targetPos = g_engine.m_coordinator.GetComponent<TransformComponent>(m_trackingTarget).GetPosition();
				Vec2 targetVel = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_trackingTarget).getVelocity();

				FMOD_VECTOR pos{ targetPos.x, targetPos.y, 0 };
				FMOD_VECTOR vel{ targetVel.x * g_deltaTime, targetVel.y * g_deltaTime, 0 };

				sound.m_system->set3DListenerAttributes(0, &pos, &vel, &listenerFront, &listenerUp);
			}
		}

		for (auto& pair : AudioManager::instance().getAudioMap())
		{
			pair.second.Update();
		}

		g_engine.m_coordinator.EndTimeSystem("Audio System");
	}

	void AudioSystem::Receive(Event& ev)
	{
		switch (ev.GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent& keytriggeredevent = dynamic_cast<KeyTriggeredEvent&>(ev);
			KeyPress keycode = keytriggeredevent.GetKeyCode();
			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::KeyM)
					ToggleMute();
			}
			return;
		} //End KeyTriggered
		}
	}

	void AudioSystem::Shutdown()
	{
		for (auto entity : m_entities)
		{
			g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).DestroySound();
		}
	}

	void AudioSystem::ToggleMute()
	{
		m_muted = !m_muted;

		for (auto entity : m_entities)
		{
			g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).getSound().Pause(m_muted);
		}

	}

	void AudioSystem::TrueInit()
	{
		for (auto& objectIterator : g_engine.m_coordinator.GetActiveObjects())
		{
			if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(objectIterator))
			{
				m_trackingTarget = objectIterator;

				break;
			}
		}

		InitSounds();
	}

	void AudioSystem::InitSounds()
	{
		for (auto entity : m_entities)
		{
			auto& aEmitter = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity);

			aEmitter.CreateSound();

			if (aEmitter.getIsScaling())
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
				aEmitter.getSound().Set3DLocation(transform.GetPosition());
				aEmitter.getSound().Set3DMaxDistance(aEmitter.getMaxDistance());
			}

		}
	}

	void AudioSystem::ShutdownSounds()
	{
		for (auto entity : m_entities)
		{
			g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entity).DestroySound();
		}


	}
}