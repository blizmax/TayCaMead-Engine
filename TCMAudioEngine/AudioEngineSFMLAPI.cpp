#include "TCMAudioEngine/SFMLAudio/AudioEngineSFMLAPI.h"

#define ToSFMLVector3(vector) sf::Vector3f(vector.x, vector.y, vector.z)
#define ToEngineVector3(vector) TCM::Engine::Math::Vec3(vector.x, vector.y, vector.z)

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
#pragma region Music
					SFMLMusic::~SFMLMusic()
					{
						if ( m_music )
						{
							m_music->stop();
							delete m_music;
							m_music = nullptr;
						}
					}

					void SFMLMusic::SetVolume( const float& volume )
					{
						m_music->setVolume( volume );
					}

					float SFMLMusic::GetVolume() const
					{
						return m_music->getVolume();
					}

					void SFMLMusic::SetPitch( const float& pitch )
					{
						m_music->setPitch( pitch );
					}

					float SFMLMusic::GetPitch()
					{
						return m_music->getPitch();
					}

					void SFMLMusic::SetPosition( const Math::Vec3& position )
					{
						m_music->setPosition( ToSFMLVector3(position) );
					}

					Math::Vec3 SFMLMusic::GetPosition() const
					{
						return ToEngineVector3(m_music->getPosition());
					}

					void SFMLMusic::SetAttenuation( const float& attenuation )
					{
						m_music->setAttenuation( attenuation );
					}

					float SFMLMusic::GetAttenuation() const
					{
						return m_music->getAttenuation();
					}

					void SFMLMusic::SetLoop( const bool& value )
					{
						m_music->setLoop( value );
					}

					bool SFMLMusic::IsLooping() const
					{
						return m_music->getLoop();
					}

					void SFMLMusic::SetRelativeToListener( const bool& value )
					{
						m_music->setRelativeToListener( value );
					}

					void SFMLMusic::WarpSeekerAt( const float& position )
					{
						m_music->play();
						m_music->setPlayingOffset( sf::seconds( position ) );
						m_music->pause();
					}

					float SFMLMusic::GetSeeker() const
					{
						sf::Time time = m_music->getPlayingOffset();
						return time.asSeconds();
					}

					Audio::Data::AudioStatus SFMLMusic::GetStatus() const
					{
						if ( IsValid() )
							return static_cast<Engine::Audio::Data::AudioStatus>(m_music->getStatus());
						return Engine::Audio::Data::AudioStatus::FAILURE;
					}

					void SFMLMusic::SetMinimumRangeToHear( const float& range )
					{
						m_music->setMinDistance( range );
					}

					float SFMLMusic::GetMinimumRangeToHear() const
					{
						return m_music->getMinDistance();
					}

					void SFMLMusic::Play() const
					{
						m_music->play();
					}

					void SFMLMusic::Pause() const
					{
						m_music->pause();
					}

					void SFMLMusic::Stop() const
					{
						m_music->stop();
					}

					float SFMLMusic::GetDuration() const
					{
						return m_music->getDuration().asSeconds();
					}

					unsigned int SFMLMusic::GetSampleRate() const
					{
						return m_music->getSampleRate();
					}

					unsigned SFMLMusic::GetChannelCount() const
					{
						return m_music->getChannelCount();
					}

					bool SFMLMusic::IsValid() const
					{
						return m_music != nullptr;
					}
#pragma endregion

#pragma region Sound
					SFMLSound::~SFMLSound()
					{
						if ( m_sound )
						{
							m_sound->stop();
							delete m_sound;
							m_sound = nullptr;
						}

						if ( m_soundBuffer )
						{
							delete m_soundBuffer;
							m_soundBuffer = nullptr;
						}
					}

					void SFMLSound::SetVolume( const float& volume )
					{
						m_sound->setVolume( volume );
					}

					float SFMLSound::GetVolume() const
					{
						return m_sound->getVolume();
					}

					void SFMLSound::SetPitch( const float& pitch )
					{
						m_sound->setPitch( pitch );
					}

					float SFMLSound::GetPitch()
					{
						return m_sound->getPitch();
					}

					void SFMLSound::SetPosition( const Math::Vec3& position )
					{
						m_sound->setPosition( ToSFMLVector3(position) );
					}

					Math::Vec3 SFMLSound::GetPosition() const
					{
						return ToEngineVector3(m_sound->getPosition());
					}

					void SFMLSound::SetAttenuation( const float& attenuation )
					{
						m_sound->setAttenuation( attenuation );
					}

					float SFMLSound::GetAttenuation() const
					{
						return m_sound->getAttenuation();
					}

					void SFMLSound::SetLoop( const bool& value )
					{
						m_sound->setLoop( value );
					}

					bool SFMLSound::IsLooping() const
					{
						return m_sound->getLoop();
					}

					void SFMLSound::SetRelativeToListener( const bool& value )
					{
						m_sound->setRelativeToListener( value );
					}

					void SFMLSound::WarpSeekerAt( const float& position )
					{
						m_sound->setPlayingOffset( sf::Time( sf::seconds( position ) ) );
					}

					float SFMLSound::GetSeeker() const
					{
						return m_sound->getPlayingOffset().asSeconds();
					}

					Audio::Data::AudioStatus SFMLSound::GetStatus() const
					{
						if ( IsValid() )
							return Engine::Audio::Data::AudioStatus( m_sound->getStatus() );
						return Engine::Audio::Data::AudioStatus::FAILURE;
					}

					void SFMLSound::SetMinimumRangeToHear( const float& range )
					{
						m_sound->setMinDistance( range );
					}

					float SFMLSound::GetMinimumRangeToHear() const
					{
						return m_sound->getMinDistance();
					}

					void SFMLSound::Play() const
					{
						m_sound->play();
					}

					void SFMLSound::Pause() const
					{
						m_sound->pause();
					}

					void SFMLSound::Stop() const
					{
						m_sound->stop();
					}

					float SFMLSound::GetDuration() const
					{
						return m_soundBuffer->getDuration().asSeconds();
					}

					unsigned int SFMLSound::GetSampleRate() const
					{
						return m_soundBuffer->getSampleRate();
					}

					unsigned int SFMLSound::GetChannelCount() const
					{
						return m_soundBuffer->getChannelCount();
					}

					bool SFMLSound::IsValid() const
					{
						return m_sound && m_soundBuffer;
					}
#pragma endregion
				} // Data

				AudioEngineSFMLAPI::AudioEngineSFMLAPI()
				{
					sf::err().rdbuf( nullptr );
				}

				Audio::Data::Sound* AudioEngineSFMLAPI::CoreLoadSound( const std::string& path, const bool& relative )
				{
					(void)relative; // SFML does not need hint about this

					sf::SoundBuffer* buffer = new sf::SoundBuffer();
					if ( !buffer->loadFromFile( path ) )
					{
						TCMWARNING("Sound tried to be load at " + path + " could not be found ! (Is it the correct path ?)");
						return new Data::SFMLSound( nullptr, nullptr );
					}
					sf::Sound* sound = new sf::Sound( *buffer );

					Data::SFMLSound* sfmlSound = new Data::SFMLSound( sound, buffer );
					return sfmlSound;
				}

				Audio::Data::Music* AudioEngineSFMLAPI::CoreOpenMusic( const std::string& path, const bool& relative )
				{
					(void)relative; // SFML does not need hint about this

					sf::Music* music = new sf::Music();

					if ( !music->openFromFile( path ) )
					{
						TCMWARNING("Music tried to be opened at " + path + " could not be found ! (Is it the correct path ?)");
						return new Data::SFMLMusic( nullptr );
					}

					Data::SFMLMusic* sfmlMusic = new Data::SFMLMusic( music );

					return sfmlMusic;
				}
			}
		}
	}
}
