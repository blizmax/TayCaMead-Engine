#pragma once
#include "AudioComponent.h"

namespace TCM
{
	namespace Engine
	{
		class ENGINECLASSES_DLL_EXPORT MusicComponent :
				public AudioComponent
		{
			TCM_CLASS(MusicComponent, AudioComponent)

		public:
			MusicComponent();
			~MusicComponent();

			void Construct( const std::string& path ) const;
			void TCMPoolCreate() override;
			void TCMPoolRelease() override;
			void Init() override;
			void OnEnable() override;
			void OnDisable() override;

			/**
			 * \brief \n Sets the volume of the music. (0->100) \n
			 * \param volume _IN_ The new volume of the music.
			 */
			void SetVolume( const float& volume ) override;

			/**
			 * \brief \n Returns the volume of the music. \n
			 * \return The volume of the music.
			 */
			float GetVolume() const override;

			/**
			 * \brief \n Sets the pitch of the music. \n
			 * \param pitch The new pitch of the music.
			 */
			void SetPitch( const float& pitch ) override;

			/**
			 * \brief \n Returns the pitch of the music. \n
			 * \return The pitch of the music.
			 */
			float GetPitch() override;

			/**
			 * \brief \n Sets the position of the music. \n
			 * \param position The new position of the music. \n
			 * \remarks The music needs to be mono channeled !
			 */
			void SetPosition( const Math::Vec3& position ) override;

			/**
			 * \brief \n Returns the  of the music. \n
			 * \return The position of the music.
			 */
			Math::Vec3 GetPosition() const override;

			/**
			 * \brief \n Sets the attenuation of the music. \n
			 * \param attenuation _IN_ The new attenuation value.
			 */
			void SetAttenuation( const float& attenuation ) override;

			/**
			 * \brief \n Returns the attenuation of the music. \n
			 * \return The attenuation of the music.
			 */
			float GetAttenuation() const override;

			/**
			 * \brief \n Sets the looping setting of the music. \n
			 * \param value _IN_ The new looping setting.
			 */
			void SetLoop( const bool& value ) override;

			/**
			 * \brief \n Returns the lopping setting of the music. \n
			 * \return The looping setting of the music.
			 */
			bool IsLooping() const override;

			/**
			 * \brief \n Sets the relativity of the music to the listener. \n
			 * \param value _IN_ The new relativity of the music to the listener setting.
			 */
			void SetRelativeToListener( const bool& value ) override;

			/**
			 * \brief \n Warps the seeker to the specific value. \n
			 * \param position _IN_ The time where you want the music to continue in \b second.
			 */
			void WarpSeekerAt( const float& position ) override;

			/**
			 * \brief \n Returns the seeker at current time. \n
			 * \return The current time of the music in \b second.
			 */
			float GetSeeker() const override;

			/**
			 * \brief \n Returns the status of the music. \n
			 * \return The status of the music.
			 */
			Audio::Data::AudioStatus GetStatus() const override;

			/**
			 * \brief \n Sets the minimum range of the music to be audible. \n
			 * \param range _IN_ The new minimum range for the music to be audible.
			 */
			void SetMinimumRangeToHear( const float& range ) override;

			/**
			 * \brief \n Returns the minimum range of the music to be audible. \n
			 * \return The minimum range for the music to be audible.
			 */
			float GetMinimumRangeToHear() const override;

			/**
			 * \brief \n Plays the music or starts it on Init. \n
			 */
			void Play() override;

			/**
			 * \brief \n Plays the music. \n
			 */
			void Play() const override;

			/**
			 * \brief \n Pauses the music. \n
			 */
			void Pause() const override;

			/**
			 * \brief \n Stops the music. \n
			 */
			void Stop() override;

			/**
			 * \brief \n Stops the music. \n
			 */
			void Stop() const override;

			/**
			 * \brief \n Returns the duration of the music. \n
			 * \return The total duration of the music.
			 */
			float GetDuration() const override;

			/**
			 * \brief \n Returns the sample rate of the music. \n
			 * \return The sample rate of the music.
			 */
			unsigned int GetSampleRate() const override;

			/**
			 * \brief \n Returns the channel count of the music. \n
			 * \return The channel count of the music.
			 */
			unsigned int GetChannelCount() const override;

			/**
			 * \brief Returns wether the audio component is valid or not
			 * \return True is valid, false otherwise
			 */
			bool IsValid() const override;

		private:
			Audio::Data::Music* m_music = nullptr;
		};
	}
}
