#pragma once

#include "AudioComponent.h"

namespace TCM
{
	namespace Engine
	{
		class ENGINECLASSES_DLL_EXPORT SoundComponent :
				public AudioComponent
		{
			TCM_CLASS(SoundComponent, AudioComponent)

		public:
			SoundComponent();
			~SoundComponent();

			void Construct( const std::string& path ) const;
			void TCMPoolCreate() override;
			void TCMPoolRelease() override;
			void Init() override;
			void OnEnable() override;
			void OnDisable() override;

			/**
			 * \brief \n Sets the volume of the sound. (0->100) \n
			 * \param volume _IN_ The new volume of the sound.
			 */
			void SetVolume( const float& volume ) override;

			/**
			 * \brief \n Returns the volume of the sound. \n
			 * \return The volume of the sound.
			 */
			float GetVolume() const override;

			/**
			 * \brief \n Sets the pitch of the sound. \n
			 * \param pitch The new pitch of the sound.
			 */
			void SetPitch( const float& pitch ) override;

			/**
			 * \brief \n Returns the pitch of the sound. \n
			 * \return The pitch of the sound.
			 */
			float GetPitch() override;

			/**
			 * \brief \n Sets the position of the sound. \n
			 * \param position The new position of the sound. \n
			 * \remarks The sound needs to be mono channeled !
			 */
			void SetPosition( const Math::Vec3& position ) override;

			/**
			 * \brief \n Returns the  of the sound. \n
			 * \return The position of the sound.
			 */
			Math::Vec3 GetPosition() const override;

			/**
			 * \brief \n Sets the attenuation of the sound. \n
			 * \param attenuation _IN_ The new attenuation value.
			 */
			void SetAttenuation( const float& attenuation ) override;

			/**
			 * \brief \n Returns the attenuation of the sound. \n
			 * \return The attenuation of the sound.
			 */
			float GetAttenuation() const override;

			/**
			 * \brief \n Sets the looping setting of the sound. \n
			 * \param value _IN_ The new looping setting.
			 */
			void SetLoop( const bool& value ) override;

			/**
			 * \brief \n Returns the lopping setting of the sound. \n
			 * \return The looping setting of the sound.
			 */
			bool IsLooping() const override;

			/**
			 * \brief \n Sets the relativity of the sound to the listener. \n
			 * \param value _IN_ The new relativity of the sound to the listener setting.
			 */
			void SetRelativeToListener( const bool& value ) override;

			/**
			 * \brief \n Warps the seeker to the specific value. \n
			 * \param position _IN_ The time where you want the sound to continue in \b second.
			 */
			void WarpSeekerAt( const float& position ) override;

			/**
			 * \brief \n Returns the seeker at current time. \n
			 * \return The current time of the sound in \b second.
			 */
			float GetSeeker() const override;

			/**
			 * \brief \n Returns the status of the sound. \n
			 * \return The status of the sound.
			 */
			Audio::Data::AudioStatus GetStatus() const override;

			/**
			 * \brief \n Sets the minimum range of the sound to be audible. \n
			 * \param range _IN_ The new minimum range for the sound to be audible.
			 */
			void SetMinimumRangeToHear( const float& range ) override;

			/**
			 * \brief \n Returns the minimum range of the sound to be audible. \n
			 * \return The minimum range for the sound to be audible.
			 */
			float GetMinimumRangeToHear() const override;

			/**
			 * \brief \n Plays the sound. \n
			 */
			void Play() override;

			/**
			 * \brief \n Plays the sound. \n
			 */
			void Play() const override;

			/**
			 * \brief \n Pauses the sound. \n
			 */
			void Pause() const override;

			/**
			 * \brief \n Stops the sound. \n
			 */
			void Stop() override;

			/**
			 * \brief \n Stops the sound. \n
			 */
			void Stop() const override;

			/**
			 * \brief \n Returns the duration of the sound. \n
			 * \return The total duration of the sound.
			 */
			float GetDuration() const override;

			/**
			 * \brief \n Returns the sample rate of the sound. \n
			 * \return The sample rate of the sound.
			 */
			unsigned int GetSampleRate() const override;

			/**
			 * \brief \n Returns the channel count of the sound. \n
			 * \return The channel count of the sound.
			 */
			unsigned int GetChannelCount() const override;

			/**
			 * \brief Returns wether the audio component is valid or not
			 * \return True is valid, false otherwise
			 */
			bool IsValid() const override;

		private:
			Audio::Data::Sound* m_sound;
		};
	}
}
