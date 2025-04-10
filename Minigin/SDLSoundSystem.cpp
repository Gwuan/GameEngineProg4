#include "SDLSoundSystem.h"
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <algorithm>

class SDLSoundSystem::Impl
{
public:
	Impl();
	~Impl();

	void PlaySoundEffect(const std::string& path, float volume);
	void PlayMusic(const std::string& path, float volume);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();
	void PauseAll();
	void ResumeAll();

private:
	void ProcessQueue();
	static Uint8 ClampVolume(float volume);

	bool m_IsRunning = true;
	std::queue<QueuedAudio> m_SoundEffectQueue;

	std::thread m_AudioThread;
	std::mutex m_AudioMutex;
	std::condition_variable m_NeedsAudio;

	Mix_Music* m_Music = nullptr;
};

SDLSoundSystem::Impl::Impl()
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
		std::cout << "SDL error:" << Mix_GetError() << std::endl;
	}
	else
	{
		std::cout << "Open audio success" << std::endl; 
	}

	
	m_AudioThread = std::thread(&Impl::ProcessQueue, this);

}

SDLSoundSystem::Impl::~Impl()
{
	m_IsRunning = false;
	m_NeedsAudio.notify_one();
	m_AudioThread.join();

	Mix_FreeMusic(m_Music);

	Mix_CloseAudio();
	Mix_Quit();
}

void SDLSoundSystem::Impl::PlaySoundEffect(const std::string& path, float volume)
{
	const QueuedAudio newSoundEffect = 
	{
		path,
		volume,
		true
	};

	std::unique_lock<std::mutex> lock(m_AudioMutex);
	m_SoundEffectQueue.emplace(newSoundEffect);
	m_NeedsAudio.notify_one();
}

void SDLSoundSystem::Impl::PlayMusic(const std::string& path, float volume)
{
	const QueuedAudio newMusic
	{
		path,
		volume,
		false
	};

	std::unique_lock<std::mutex> lock(m_AudioMutex);
	m_SoundEffectQueue.emplace(newMusic);
	m_NeedsAudio.notify_one();
}

void SDLSoundSystem::Impl::PauseMusic()
{
	Mix_PauseMusic();
}

void SDLSoundSystem::Impl::ResumeMusic()
{
	Mix_ResumeMusic();
}

void SDLSoundSystem::Impl::StopMusic()
{
	Mix_HaltMusic();
}

void SDLSoundSystem::Impl::PauseAll()
{
	Mix_Pause(-1);
	Mix_PauseMusic();
}

void SDLSoundSystem::Impl::ResumeAll()
{
	Mix_Resume(-1);
	Mix_ResumeMusic();
}

void SDLSoundSystem::Impl::ProcessQueue()
{
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

				// If current chunk is not present inside the map...
				if (soundEffect == chunkCache.end())
				{
					auto chunk = Mix_LoadWAV(currentAudio.path.c_str());

					// If the soundEffect can't be loaded in, skip to the next while loop cycle
					if (!chunk) 
					{
					    std::cerr << "Failed to load sound effect: " << currentAudio.path << "\nError: " << Mix_GetError() << "\n";
					    queueLock.unlock();
					    continue;
					}

					// If it is valid, insert it inside the map
					chunkCache.emplace(key, chunk);
				}

				chunkCache.at(key)->volume = ClampVolume(currentAudio.volume);
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
				if (m_Music != nullptr)
				{
					Mix_VolumeMusic(ClampVolume(currentAudio.volume));
				}
				#ifdef _DEBUG
				else
				{
					throw std::runtime_error(std::string("Failed to load music: ") + Mix_GetError());
				}
				#endif

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

Uint8 SDLSoundSystem::Impl::ClampVolume(float volume)
{
	const int result = std::clamp(static_cast<int>(MIX_MAX_VOLUME * volume), 0, MIX_MAX_VOLUME);
	return static_cast<Uint8>(result);
}

SDLSoundSystem::SDLSoundSystem()
	: m_pImpl(new Impl())
{}

SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}


void SDLSoundSystem::PlaySoundEffect(const std::string& path, const float volume)
{
	m_pImpl->PlaySoundEffect(path, volume);
}

void SDLSoundSystem::PlayMusic(const std::string& path, const float volume)
{
	m_pImpl->PlayMusic(path, volume);
}

void SDLSoundSystem::PauseMusic()
{
	m_pImpl->PauseMusic();
}

void SDLSoundSystem::ResumeMusic()
{
	m_pImpl->ResumeMusic();
}

void SDLSoundSystem::StopMusic()
{
	m_pImpl->StopMusic();
}

void SDLSoundSystem::PauseAll()
{
	m_pImpl->PauseAll();
}

void SDLSoundSystem::ResumeAll()
{
	m_pImpl->ResumeAll();
}