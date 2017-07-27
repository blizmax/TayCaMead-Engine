#pragma once
#ifdef _DEBUG
#pragma comment(lib, "TCMAudioEngine/debug/sfml-audio-d.lib")
#pragma comment(lib, "TCMAudioEngine/debug/sfml-system-d.lib")
#else
	#pragma comment(lib, "TCMAudioEngine/release/sfml-audio.lib")
	#pragma comment(lib, "TCMAudioEngine/release/sfml-system.lib")
#endif

#include "TCMAudioEngine/SFMLAudio/SFML/Audio.hpp"
#include "TCMAudioEngine/AudioEngine/AudioEngine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			namespace SFML
			{
				namespace Data
				{
					struct SFMLMusic : Audio::Data::Music
					{
						SFMLMusic() = delete;

						explicit SFMLMusic( sf::Music* music ) : m_music( music )
						{
						}

						~SFMLMusic();

						void SetVolume( const float& volume ) override;
						float GetVolume() const override;

						void SetPitch( const float& pitch ) override;
						float GetPitch() override;

						void SetPosition( const Math::Vec3& position ) override;
						Math::Vec3 GetPosition() const override;

						void SetAttenuation( const float& attenuation ) override;
						float GetAttenuation() const override;

						void SetLoop( const bool& value ) override;
						bool IsLooping() const override;

						void SetRelativeToListener( const bool& value ) override;

						void WarpSeekerAt( const float& position ) override;
						float GetSeeker() const override;

						Engine::Audio::Data::AudioStatus GetStatus() const override;

						void SetMinimumRangeToHear( const float& range ) override;
						float GetMinimumRangeToHear() const override;

						void Play() const override;
						void Pause() const override;
						void Stop() const override;

						float GetDuration() const override;

						unsigned int GetSampleRate() const override;

						unsigned int GetChannelCount() const override;

						bool IsValid() const override;

					private:
						sf::Music* m_music;
					};

					struct SFMLSound : Audio::Data::Sound
					{
						explicit SFMLSound( sf::Sound* sound, sf::SoundBuffer* soundBuffer )
							: m_sound( sound )
							, m_soundBuffer( soundBuffer )
						{
						}

						~SFMLSound();

						void SetVolume( const float& volume ) override;
						float GetVolume() const override;

						void SetPitch( const float& pitch ) override;
						float GetPitch() override;

						void SetPosition( const Math::Vec3& position ) override;
						Math::Vec3 GetPosition() const override;

						void SetAttenuation( const float& attenuation ) override;
						float GetAttenuation() const override;

						void SetLoop( const bool& value ) override;
						bool IsLooping() const override;

						void SetRelativeToListener( const bool& value ) override;
						void WarpSeekerAt( const float& position ) override;

						float GetSeeker() const override;
						Engine::Audio::Data::AudioStatus GetStatus() const override;

						void SetMinimumRangeToHear( const float& range ) override;
						float GetMinimumRangeToHear() const override;

						void Play() const override;
						void Pause() const override;
						void Stop() const override;

						float GetDuration() const override;
						unsigned int GetSampleRate() const override;
						unsigned int GetChannelCount() const override;

						bool IsValid() const override;

					protected:
						sf::Sound* m_sound = nullptr;
						sf::SoundBuffer* m_soundBuffer = nullptr;
					};
				}

				class AudioEngineSFMLAPI : public AudioEngine
				{
				public:
					AudioEngineSFMLAPI();
					~AudioEngineSFMLAPI() = default;

				protected:
					Audio::Data::Sound* CoreLoadSound( const std::string& path, const bool& relative ) override;
					Audio::Data::Music* CoreOpenMusic( const std::string& path, const bool& relative ) override;
				};
			}
		}
	}
}
