#include "StdAfx.h"
// #include "SoundSystem.h"
// #include "SoundEngine.h"
#include <locale.h>
#include <time.h>

namespace DuiLib
{
	SoundSystem::SoundSystem()
		: m_pSoundEngine(NULL)
		,isInster(false)
	{

	}

	SoundSystem::~SoundSystem()
	{

	}

	/*SoundSystem* SoundSystem::Instance()
	{
		return ms_pSingleton;
	}*/

	void SoundSystem::preloadBackgroundMusic( CDuiString strFilePath )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->preloadBackgroundMusic(strFilePath);
		}
	}

	void SoundSystem::playBackgroundMusic( CDuiString strFilePath, bool bLoop )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->playBackgroundMusic(strFilePath, bLoop);
		}
	}

	void SoundSystem::stopBackgroundMusic( bool bReleaseData )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->stopBackgroundMusic(bReleaseData);
		}
	}

	void SoundSystem::pauseBackgroundMusic()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->pauseBackgroundMusic();
		}
	}

	void SoundSystem::resumeBackgroundMusic()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->resumeBackgroundMusic();
		}
	}

	void SoundSystem::rewindBackgroundMusic()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->rewindBackgroundMusic();
		}
	}

	bool SoundSystem::willPlayBackgroundMusic()
	{
		if (m_pSoundEngine)
		{
			return m_pSoundEngine->willPlayBackgroundMusic();
		}

		return false;
	}

	bool SoundSystem::isBackgroundMusicPlaying()
	{
		if (m_pSoundEngine)
		{
			return m_pSoundEngine->isBackgroundMusicPlaying();
		}

		return false;
	}

	float SoundSystem::getBackgroundMusicVolume()
	{
		if (m_pSoundEngine)
		{
			return m_pSoundEngine->getBackgroundMusicVolume();
		}

		return 0.0f;
	}

	void SoundSystem::setBackgroundMusicVolume( float volume )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->setBackgroundMusicVolume(volume);
		}
	}

	float SoundSystem::getEffectsVolume()
	{
		if (m_pSoundEngine)
		{
			return m_pSoundEngine->getEffectsVolume();
		}

		return 0.0f;
	}

	void SoundSystem::setEffectsVolume( float volume )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->setEffectsVolume(volume);
		}
	}

	unsigned int SoundSystem::playEffect( CDuiString strFilePath, bool bLoop )
	{
		if (m_pSoundEngine)
		{
			for (int i = 0; i < smap.GetSize(); i++)
			{
				if (LPCTSTR first = smap.GetAt(i))
				{
					if (strFilePath.Compare(first) != 0)
					{
						isInster = true;
					}
					else
					{
						int* second = static_cast<int*>(smap.Find(first));
						isInster = false;
						long n_timezone = (long)time(NULL);
						float timetemp = (float)(n_timezone - *second);
						if ((int)timetemp <= 500)
						{
							return -1;
						}
						else
						{
							int n_timezone = 0;
							n_timezone = (long)time(NULL);

							*second = n_timezone;
							break;
						}
					}
				}
			}
			if (smap.GetSize() <= 0)
			{
				long n_timezone = 0;
				n_timezone = (long)time(NULL);
				smap.Insert(strFilePath.GetData(), &n_timezone);
			}
			m_pSoundEngine->playEffect(strFilePath, bLoop);
			if(isInster)
			{
				long n_timezone = 0;
				n_timezone = (long)time(NULL);
				smap.Insert(strFilePath.GetData(), &n_timezone);
			}
			return 1;
		}
		return 0;
	}

	void SoundSystem::pauseEffect( unsigned int nSoundId )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->pauseEffect(nSoundId);
		}
	}

	void SoundSystem::pauseAllEffects()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->pauseAllEffects();
		}
	}

	void SoundSystem::resumeEffect( unsigned int nSoundId )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->resumeEffect(nSoundId);
		}
	}

	void SoundSystem::resumeAllEffects()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->resumeAllEffects();
		}
	}

	void SoundSystem::stopEffect( unsigned int nSoundId )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->stopEffect(nSoundId);
		}
	}

	void SoundSystem::stopAllEffects()
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->stopAllEffects();
		}
	}

	void SoundSystem::preloadEffect( CDuiString strFilePath )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->preloadEffect(strFilePath);
		}
	}

	void SoundSystem::unloadEffect( CDuiString strFilePath )
	{
		if (m_pSoundEngine)
		{
			m_pSoundEngine->unloadEffect(strFilePath);
		}
	}

	int SoundSystem::Return_Max()
	{
		if (m_pSoundEngine)
		{
			return m_pSoundEngine->Return_Max();
		}
		return 0;
	}

	void SoundSystem::Soundend()
	{
		if(m_pSoundEngine)
		{
			m_pSoundEngine->Soundend();
		}
	}

	void SoundSystem::Musicend()
	{
		if(m_pSoundEngine)
		{
			m_pSoundEngine->Musicend();
		}
	}
}//LORD namespace end.
