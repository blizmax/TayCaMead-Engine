#pragma once
#include "TCMAudioEngine/AudioEngineDefine.h"
#include "MathLib/Vec3.h"

#include <string>

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			/**
			 * \brief \n Path leading to ressources in debug. \n
			 */
			const std::string pathToRessourcesInDebug = "..\\Common\\TCMAudioEngine\\Audios\\";

			namespace Data
			{
				/**
				 * \brief \n Struct describing the state of the audio. \n
				 * \param Stopped The audio is stopped.
				 * \param Paused The audio is paused.
				 * \param Playing The audio is actually playing.
				 * \param FAILURE Something went wrong when creating the audio !
				 */
				enum class AudioStatus
				{
					Stopped = 0,
					Paused,
					Playing,
					FAILURE,
				};

				/**
				 * \brief \n Base class for every audio in the engine. \n
				 */
				struct Audio abstract
				{
					virtual ~Audio() = default;

					/**
					 * \brief \n Sets the volume of the audio. (0->100) \n
					 * \param volume _IN_ The new volume of the audio.
					 */
					virtual void SetVolume( const float& volume ) = 0;
					/**
					 * \brief \n Returns the volume of the audio. \n
					 * \return The volume of the audio.
					 */
					virtual float GetVolume() const = 0;

					/**
					 * \brief \n Sets the pitch of the audio. \n
					 * \param pitch The new pitch of the audio.
					 */
					virtual void SetPitch( const float& pitch ) = 0;
					/**
					 * \brief \n Returns the pitch of the audio. \n
					 * \return The pitch of the audio.
					 */
					virtual float GetPitch() = 0;

					/**
					 * \brief \n Sets the position of the audio. \n
					 * \param position The new position of the audio. \n
					 * \remarks The audio needs to be mono channeled !
					 */
					virtual void SetPosition( const Math::Vec3& position ) = 0;
					/**
					* \brief \n Returns the  of the audio. \n
					* \return The position of the audio.
					*/
					virtual Math::Vec3 GetPosition() const = 0;

					/**
					 * \brief \n Sets the attenuation of the audio. \n
					 * \param attenuation _IN_ The new attenuation value.
					 */
					virtual void SetAttenuation( const float& attenuation ) = 0;
					/**
					 * \brief \n Returns the attenuation of the audio. \n
					 * \return The attenuation of the audio.
					 */
					virtual float GetAttenuation() const = 0;

					/**
					 * \brief \n Sets the looping setting of the audio. \n
					 * \param value _IN_ The new looping setting.
					 */
					virtual void SetLoop( const bool& value ) = 0;
					/**
					 * \brief \n Returns the lopping setting of the audio. \n
					 * \return The looping setting of the audio.
					 */
					virtual bool IsLooping() const = 0;

					/**
					 * \brief \n Sets the relativity of the audio to the listener. \n
					 * \param value _IN_ The new relativity of the audio to the listener setting.
					 */
					virtual void SetRelativeToListener( const bool& value ) = 0;

					/**
					 * \brief \n Warps the seeker to the specific value. \n
					 * \param position _IN_ The time where you want the audio to continue in \b second.
					 */
					virtual void WarpSeekerAt( const float& position ) = 0;
					/**
					 * \brief \n Returns the seeker at current time. \n
					 * \return The current time of the audio in \b second.
					 */
					virtual float GetSeeker() const = 0;

					/**
					 * \brief \n Returns the status of the audio. \n
					 * \return The status of the audio.
					 */
					virtual AudioStatus GetStatus() const = 0;

					/**
					 * \brief \n Sets the minimum range of the audio to be audible. \n
					 * \param range _IN_ The new minimum range for the audio to be audible.
					 */
					virtual void SetMinimumRangeToHear( const float& range ) = 0;
					/**
					 * \brief \n Returns the minimum range of the audio to be audible. \n
					 * \return The minimum range for the audio to be audible.
					 */
					virtual float GetMinimumRangeToHear() const = 0;

					/**
					 * \brief \n Plays the audio. \n
					 */
					virtual void Play() const = 0;
					/**
					 * \brief \n Pauses the audio. \n
					 */
					virtual void Pause() const = 0;
					/**
					 * \brief \n Stops the audio. \n
					 */
					virtual void Stop() const = 0;

					/**
					 * \brief \n Returns the duration of the audio. \n
					 * \return The total duration of the audio.
					 */
					virtual float GetDuration() const = 0;
					/**
					 * \brief \n Returns the sample rate of the audio. \n
					 * \return The sample rate of the audio.
					 */
					virtual unsigned int GetSampleRate() const = 0;
					/**
					 * \brief \n Returns the channel count of the audio. \n
					 * \return The channel count of the audio.
					 */
					virtual unsigned int GetChannelCount() const = 0;

					/**
					 * \brief Tells whether the Audio is valid or not
					 * \return True if valid, false otherwise
					 */
					virtual bool IsValid() const = 0;
				};

				/**
				 * \brief \n Audio Struct specialized in musics. \n
				 */
				struct Music abstract : Audio
				{
					virtual ~Music() = default;
				};

				/**
				 * \brief \n Audio Struct specialized in sounds. \n
				 */
				struct Sound abstract : Audio
				{
					virtual ~Sound() = default;
				};
			}

			/**
			 * \brief \n The Audio engine of the engine. \n
			 */
			class TCMAUDIOENGINE_DLL AudioEngine abstract
			{
			public:
				AudioEngine();
				virtual ~AudioEngine() = default;

				/**
				 * \brief \n Loads a sound from a file. \n
				 * \param path _IN_		The path to the file.
				 * \param relative _IN_		Is the path relative or absolute ? \n
				 * \return A sound loaded from \p path.
				 */
				static Data::Sound* LoadSound( const std::string& path, const bool& relative = false ) { return GetInstance().CoreLoadSound( path, relative ); }

				/**
				 * \brief \n Opens a music from a file. \n
				 * \param path  _IN_		The path to the file.
				 * \param relative  _IN_		Is the path relative or absolute ? \n
				 * \return  A sound opened from \p path.
				 */
				static Data::Music* OpenMusic( const std::string& path, const bool& relative = false ) { return GetInstance().CoreOpenMusic( path, relative ); }

			protected:
				/**
				 * \brief \n Function loading a sound to be overriden. \n
				 */
				virtual Data::Sound* CoreLoadSound( const std::string& path, const bool& relative ) = 0;
				/**
				* \brief \n Function opening a music to be overriden. \n
				*/
				virtual Data::Music* CoreOpenMusic( const std::string& path, const bool& relative ) = 0;

			private:
				static AudioEngine& GetInstance();
			};
		}
	}
}
