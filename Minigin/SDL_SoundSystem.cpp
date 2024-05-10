#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <unordered_map>
#include <mutex>

// Encapsulates the details of loading and playing audio files with sdl_mixer
class AudioClip final
{
public:
	AudioClip(const std::string& soundPath, int volume, int loop = 0)
		: m_SoundPath(soundPath), m_IsLoaded{ false }, m_Sound{ nullptr }, m_Volume{ volume }
		, m_AudioChannel{ -1 }, m_Loop{ loop }
	{};
	~AudioClip()
	{
		if (m_IsLoaded)
		{
			Mix_FreeChunk(m_Sound);
			m_IsLoaded = false;
		}
	};

	void Load()
	{
		m_Sound = Mix_LoadWAV(m_SoundPath.c_str());
		if (!m_Sound)
		{
			std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << "\n";
			return;
		}

		m_IsLoaded = true;
		Mix_VolumeChunk(m_Sound, m_Volume);
	}

	void Play()
	{
		if (m_IsLoaded)
		{
			int channel = Mix_PlayChannel(m_AudioChannel, m_Sound, m_Loop);
			if (channel == -1)
			{
				// There is no free channel
				std::cerr << "Mix_PlayChannel failed: " << Mix_GetError() << "\n";
			}
		}
	}

	bool IsLoaded() const { return m_IsLoaded; }

private:
	Mix_Chunk* m_Sound;
	std::string m_SoundPath;
	bool m_IsLoaded;
	int m_Volume;
	int m_AudioChannel;
	int m_Loop;
};

// ******** SDL SOUND SYSTEM IMPLEMENTATION **********
class engine::SDL_SoundSystem::SDL_SoundSystem_Impl final
{
public:
	SDL_SoundSystem_Impl()
	{
		// Initialize SDL_mixer with desired audio format (44.1kHz, 16-bit, stereo)
		int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (result < 0)
		{
			std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << "\n";
		}

		Init();
	}

	// Init the ring buffer for the PlaySound requests
	static void Init() { m_Head = 0; m_Tail = 0; }

	~SDL_SoundSystem_Impl()
	{
		SDL_CloseAudio();
	}

	// Append new requests at the end of the (queue)
	void PlaySound(const short id)
	{
		// Make sure the queue doesnt overflow.
		// Add request as long as there fever than our maximum
		if ((m_Tail + 1) % MAX_PENDING != m_Head)
		{
			// Add at the end 
			m_Pending[m_Tail] = id;
			// Wrap the tail back around to the beggining if it reaches the end
			m_Tail = (m_Tail + 1) % MAX_PENDING;

		}
	}

	void ProcessRequests()
	{
		// If there are no pending requests, do nothing
		if (m_Head == m_Tail)
		{
			return;
		}

		// Search for the sound
		auto soundPos = m_Sounds.find(m_Pending[m_Head]);
		if (soundPos != m_Sounds.end())
		{
			auto& sound = soundPos->second;
			if (!sound->IsLoaded())
			{
				// Sound not loaded yet
				sound->Load();
			}
			sound->Play();
		}

		// Wrap the head around when it reaches the end of the array
		m_Head = (m_Head + 1) % MAX_PENDING;
	}

	void RegisterSoundID(const short id, const std::string& soundPath, const int volume)
	{
		// First check the sound is not repeated
		auto idFound = m_Sounds.find(id);
		if (idFound == m_Sounds.end())
		{
			// Not repeated -> Create the sound but dont load it yet
			m_Sounds.emplace(id, std::make_unique<AudioClip>(soundPath, volume));
		}
	}

private:
	std::unordered_map<short, std::unique_ptr<AudioClip>> m_Sounds;

	// Queue for the PlaySound requests (Ring buffer)
	static const int MAX_PENDING{ 10 };		// How many pending requests we can have
	static short m_Pending[MAX_PENDING];
	static int m_Head;	// Head of the queue : Where requests are read from (Oldest pending request)
	static int m_Tail;  // Tail of the queue : Slot in the array where the next enqueued request will be written

	// Audio thread
	std::mutex m_Mutex;
	std::condition_variable m_HasRequests;
	std::jthread m_AudioThread;

};

// Define static variables used in the implementation
short engine::SDL_SoundSystem::SDL_SoundSystem_Impl::m_Pending[MAX_PENDING];
int engine::SDL_SoundSystem::SDL_SoundSystem_Impl::m_Head;
int engine::SDL_SoundSystem::SDL_SoundSystem_Impl::m_Tail;

engine::SDL_SoundSystem::SDL_SoundSystem()
{
	pImpl = std::make_unique<SDL_SoundSystem_Impl>();
}

engine::SDL_SoundSystem::~SDL_SoundSystem()
{

}

void engine::SDL_SoundSystem::PlaySound(const short id)
{
	pImpl->PlaySound(id);
}

void engine::SDL_SoundSystem::RegisterSoundID(const short id, const std::string& soundPath, const int volume)
{
	pImpl->RegisterSoundID(id, soundPath, volume);
}

void engine::SDL_SoundSystem::ProcessRequests()
{
	pImpl->ProcessRequests();
}

/*
- Sound is now threaded, can use this as discussion point for "Usage of threading" in exam presentation
- Unlock mutex when we finish using the queue, since the queue is the one we need to protect. So after pop we unlock the mutex
- Only one thread per sound (dont use jthread for every sound)
*/
