#ifndef  __SOUND_ENGINE_H__
#define  __SOUND_ENGINE_H__

#pragma once

namespace DuiLib
{
	class UILIB_API SoundEngine
	{
	public:
		SoundEngine();
		~SoundEngine();

	public:
		virtual void Soundend();
		virtual void Musicend();

		/**
		 @brief Preload background music
		 @param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
		 */
		virtual void preloadBackgroundMusic(CDuiString strFilePath);
    
		/**
		@brief Play background music
		@param pszFilePath The path of the background music file,or the FileName of T_SoundResInfo
		@param bLoop Whether the background music loop or not
		*/
		virtual void playBackgroundMusic(CDuiString strFilePath, bool bLoop);
		virtual void playBackgroundMusic(CDuiString strFilePath) {
    		this->playBackgroundMusic(strFilePath, false);
		}

		/**
		@brief Stop playing background music
		@param bReleaseData If release the background music data or not.As default value is false
		*/
		virtual void stopBackgroundMusic(bool bReleaseData);
		virtual void stopBackgroundMusic() {
    		this->stopBackgroundMusic(false);
		}

		/**
		@brief Pause playing background music
		*/
		virtual void pauseBackgroundMusic();

		/**
		@brief Resume playing background music
		*/
		virtual void resumeBackgroundMusic();

		/**
		@brief Rewind playing background music
		*/
		virtual void rewindBackgroundMusic();

		virtual bool willPlayBackgroundMusic();

		/**
		@brief Whether the background music is playing
		@return If is playing return true,or return false
		*/
		virtual bool isBackgroundMusicPlaying();

		// properties
		/**
		@brief The volume of the background music max value is 1.0,the min value is 0.0
		*/
		virtual float getBackgroundMusicVolume();

		/**
		@brief set the volume of background music
		@param volume must be in 0.0~1.0
		*/
		virtual void setBackgroundMusicVolume(float volume);

		/**
		@brief The volume of the effects max value is 1.0,the min value is 0.0
		*/
		virtual float getEffectsVolume();

		/**
		@brief set the volume of sound effecs
		@param volume must be in 0.0~1.0
		*/
		virtual void setEffectsVolume(float volume);

		// for sound effects
		/**
		@brief Play sound effect
		@param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
		@bLoop Whether to loop the effect playing, default value is false
		*/
		virtual unsigned int playEffect(CDuiString strFilePath, bool bLoop);
		virtual unsigned int playEffect(CDuiString strFilePath) {
    		return this->playEffect(strFilePath, false);
		}

		/**
		@brief Pause playing sound effect
		@param nSoundId The return value of function playEffect
		*/
		virtual void pauseEffect(unsigned int nSoundId);

		/**
		@brief Pause all playing sound effect
		@param nSoundId The return value of function playEffect
		*/
		virtual void pauseAllEffects();

		/**
		@brief Resume playing sound effect
		@param nSoundId The return value of function playEffect
		*/
		virtual void resumeEffect(unsigned int nSoundId);

		/**
		@brief Resume all playing sound effect
		@param nSoundId The return value of function playEffect
		*/
		virtual void resumeAllEffects();

		/**
		@brief Stop playing sound effect
		@param nSoundId The return value of function playEffect
		*/
		virtual void stopEffect(unsigned int nSoundId);

		/**
		@brief Stop all playing sound effects
		*/
		virtual void stopAllEffects();

		/**
		@brief          preload a compressed audio file
		@details        the compressed audio will be decode to wave, then write into an 
		internal buffer in SimpleaudioEngine
		*/
		virtual void preloadEffect(CDuiString strFilePath);

		/**
		@brief          unload the preloaded effect from internal buffer
		@param[in]        pszFilePath        The path of the effect file,or the FileName of T_SoundResInfo
		*/
		virtual void unloadEffect(CDuiString strFilePath);
		virtual int Return_Max();
	};
}

#endif
