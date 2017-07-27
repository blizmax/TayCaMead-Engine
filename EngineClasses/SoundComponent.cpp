#include "EngineClasses/SoundComponent.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( SoundComponent )

		SoundComponent::SoundComponent()
		{
			SetName( "SoundComponent" );
		}

		SoundComponent::~SoundComponent()
		{
			if ( m_sound )
			{
				delete m_sound;
				m_sound = nullptr;
			}
		}

		void SoundComponent::Construct( const std::string& path ) const
		{
			m_path->assign( path );
		}

		void SoundComponent::TCMPoolCreate()
		{
			Super::TCMPoolCreate();
		}

		void SoundComponent::TCMPoolRelease()
		{
			m_path->clear();

			if ( m_sound )
			{
				delete m_sound;
				m_sound = nullptr;
			}

			Super::TCMPoolRelease();
		}

		void SoundComponent::Init()
		{
			if ( m_bInit )
				return;

			Super::Init();

			m_sound = Audio::AudioEngine::LoadSound( *m_path );

			SetVolume( m_data.m_volume );
			SetPitch( m_data.m_pitch );
			SetPosition( m_data.m_position );
			SetAttenuation( m_data.m_attenuation );
			SetLoop( m_data.m_isLooping );
			SetRelativeToListener( m_data.m_isRelativeToListener );
			WarpSeekerAt( m_data.m_seekerPosition );
			SetMinimumRangeToHear( m_data.m_minimumRangeToHear );

			if ( m_bPlayOnInit && IsValid() )
				m_sound->Play();
		}

		void SoundComponent::OnEnable()
		{
			if ( IsValid() )
				m_sound->Play();
		}

		void SoundComponent::OnDisable()
		{
			if ( IsValid() )
				m_sound->Stop();
		}

		void SoundComponent::SetVolume( const float& volume )
		{
			if ( IsValid() )
				m_sound->SetVolume( volume );
			m_data.m_volume = volume;
		}

		float SoundComponent::GetVolume() const
		{
			if ( IsValid() )
				return m_sound->GetVolume();
			return m_data.m_volume;
		}

		void SoundComponent::SetPitch( const float& pitch )
		{
			if ( IsValid() )
				m_sound->SetPitch( pitch );
			m_data.m_pitch = pitch;
		}

		float SoundComponent::GetPitch()
		{
			if ( IsValid() )
				return m_sound->GetPitch();
			return m_data.m_pitch;
		}

		void SoundComponent::SetPosition( const Math::Vec3& position )
		{
			if ( IsValid() )
				m_sound->SetPosition( position );
			m_data.m_position = position;
		}

		Math::Vec3 SoundComponent::GetPosition() const
		{
			if ( IsValid() )
				return m_sound->GetPosition();
			return m_data.m_position;
		}

		void SoundComponent::SetAttenuation( const float& attenuation )
		{
			if ( IsValid() )
				m_sound->SetAttenuation( attenuation );
			m_data.m_attenuation = attenuation;
		}

		float SoundComponent::GetAttenuation() const
		{
			if ( IsValid() )
				return m_sound->GetAttenuation();
			return m_data.m_attenuation;
		}

		void SoundComponent::SetLoop( const bool& value )
		{
			if ( IsValid() )
				m_sound->SetLoop( value );
			m_data.m_isLooping = value;
		}

		bool SoundComponent::IsLooping() const
		{
			if ( IsValid() )
				return m_sound->IsLooping();
			return m_data.m_isLooping;
		}

		void SoundComponent::SetRelativeToListener( const bool& value )
		{
			if ( IsValid() )
				m_sound->SetRelativeToListener( value );
			m_data.m_isRelativeToListener = value;
		}

		void SoundComponent::WarpSeekerAt( const float& position )
		{
			if ( IsValid() )
				m_sound->WarpSeekerAt( position );
			m_data.m_seekerPosition = position;
		}

		float SoundComponent::GetSeeker() const
		{
			if ( IsValid() )
				return m_sound->GetSeeker();
			return m_data.m_seekerPosition;
		}

		Audio::Data::AudioStatus SoundComponent::GetStatus() const
		{
			if ( IsValid() )
				return m_sound->GetStatus();
			return Audio::Data::AudioStatus::FAILURE;
		}

		void SoundComponent::SetMinimumRangeToHear( const float& range )
		{
			if ( IsValid() )
				m_sound->SetMinimumRangeToHear( range );
			m_data.m_minimumRangeToHear = range;
		}

		float SoundComponent::GetMinimumRangeToHear() const
		{
			if ( IsValid() )
				return m_sound->GetMinimumRangeToHear();
			return m_data.m_minimumRangeToHear;
		}

		void SoundComponent::Play()
		{
			if ( IsValid() )
				m_sound->Play();
			else if ( !m_bInit )
				SetPlayOnInit( true );
		}

		void SoundComponent::Play() const
		{
			if ( IsValid() )
				m_sound->Play();
		}

		void SoundComponent::Pause() const
		{
			if ( IsValid() )
				m_sound->Pause();
		}

		void SoundComponent::Stop()
		{
			if ( IsValid() )
				m_sound->Stop();
			else if ( !m_bInit )
				SetPlayOnInit( false );
		}

		void SoundComponent::Stop() const
		{
			if ( IsValid() )
				m_sound->Stop();
		}

		float SoundComponent::GetDuration() const
		{
			if ( IsValid() )
				return m_sound->GetDuration();
			return -666.f;
		}

		unsigned SoundComponent::GetSampleRate() const
		{
			if ( IsValid() )
				return m_sound->GetSampleRate();
			return -666;
		}

		unsigned SoundComponent::GetChannelCount() const
		{
			if ( IsValid() )
				return m_sound->GetChannelCount();
			return -666;
		}

		bool SoundComponent::IsValid() const
		{
			return m_sound && m_sound->IsValid();
		}
	}
}
