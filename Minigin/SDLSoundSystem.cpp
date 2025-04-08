#include "SDLSoundSystem.h"

#include <iostream>
#include <SDL.h>

#include "SDL_audio.h"
#include <SDL_mixer.h>

SDLSoundSystem::SDLSoundSystem()
	: m_SoundEffectQueue()
{
	// TODO: Clean this up
	unsigned int supportedFormats = MIX_INIT_OGG;
	unsigned int innitedFormats = Mix_Init(supportedFormats);
	if ((innitedFormats & supportedFormats) != supportedFormats)
	{
		std::cout << "ERROR Mix_Init: Failed to init the supported sound formats\n"; 
	}
	else
	{
		std::cout << "Mix_Init: All supported sound formats initialized!\n";
	}

	Mix_Init(MIX_INIT_MP3);
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL error: " << SDL_GetError() << std::endl;
		throw std::runtime_error("Error on SDL_INIT_AUDIO subsystem");
	}
	else
	{
		std::cout << "AUDIO INIT SUCCESS" << std::endl;

	}


	// Initialize SDL_MIXER
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
	{
		std::cout << "SDL error:" << Mix_GetError << std::endl;
	}
	else
	{
		std::cout << "Open audio success" << std::endl; 
	}

	
	m_AudioThread = std::thread(&SDLSoundSystem::ProcessQueue, this);
}

SDLSoundSystem::~SDLSoundSystem()
{
	m_IsRunning = false;
	m_NeedsAudio.notify_one();
	m_AudioThread.join();

	Mix_FreeMusic(m_Music);
}

void SDLSoundSystem::PlaySoundEffect(const std::string& path, const float volume)
{
	const QueuedAudio newSoundEffect
	{
		path,
		ClampVolume(volume),
		true
	};

	m_SoundEffectQueue.emplace(newSoundEffect);
	m_NeedsAudio.notify_one();
}

void SDLSoundSystem::PlayMusic(const std::string& path, const float volume)
{
	const QueuedAudio newMusic
	{
		path,
		ClampVolume(volume),
		false
	};

	m_SoundEffectQueue.emplace(newMusic);
	m_NeedsAudio.notify_one();
}

void SDLSoundSystem::StopSoundLoop(const std::string&, const float)
{
	throw std::runtime_error("Not implemented yet");
}

void SDLSoundSystem::StopAllSoundEffects()
{
	throw std::runtime_error("Not implemented yet");
}

int SDLSoundSystem::ClampVolume(float volume)
{
	return std::clamp(static_cast<int>(MIX_MAX_VOLUME * volume), 0, MIX_MAX_VOLUME);
}

void SDLSoundSystem::ProcessQueue()
{
	// TODO: add error handling, this can break if a single nullptr occurs
	thread_local std::map<std::string, Mix_Chunk*> chunkCache;

	std::unique_lock<std::mutex> queueLock{m_AudioMutex};

	queueLock.unlock();

	while (m_IsRunning)
	{
		queueLock.lock();
		if (!m_SoundEffectQueue.empty())
		{
			auto currentAudio = m_SoundEffectQueue.front();
			m_SoundEffectQueue.pop();

			if (currentAudio.isSoundEffect)
			{
				const auto& key = currentAudio.path;
				auto soundEffect = chunkCache.find(key);
				if (soundEffect == chunkCache.end())
				{
					auto chunk = Mix_LoadWAV(currentAudio.path.c_str());
					chunkCache.insert(std::pair<std::string, Mix_Chunk*>(key, chunk));
				}

				chunkCache.at(key)->volume = static_cast<Uint8>(currentAudio.volume);
				Mix_PlayChannel(-1, chunkCache.at(key), 0);
			}
			else
			{
				if (m_Music)
				{
					Mix_HaltMusic();
					Mix_FreeMusic(m_Music);
					m_Music = nullptr;
				}

				m_Music = Mix_LoadMUS(currentAudio.path.c_str());
				Mix_PlayMusic(m_Music, -1);
			}

			queueLock.unlock();
		}
		else
		{
			m_NeedsAudio.wait(queueLock);
			queueLock.unlock();
		}
	}

	// When thread closes
	for (auto& pair : chunkCache)
	{
		Mix_FreeChunk(pair.second);
		pair.second = nullptr;
	}
}