#include "EngineClasses/MusicComponent.h"

namespace TCM
{
	namespace Engine
	{
		TCM_DEFINE( MusicComponent )

		MusicComponent::MusicComponent()
		{
			SetName( "MusicComponent" );
		}

		MusicComponent::~MusicComponent()
		{
			if ( m_music )
			{
				delete m_music;
				m_music = nullptr;
			}
		}

		void MusicComponent::Construct( const std::string& path ) const
		{
			m_path->assign( path );
		}

		void MusicComponent::TCMPoolCreate()
		{
			Super::TCMPoolCreate();
		}

		void MusicComponent::TCMPoolRelease()
		{
			m_path->clear();

			if ( m_music )
			{
				delete m_music;
				m_music = nullptr;
			}

			Super::TCMPoolRelease();
		}

		void MusicComponent::Init()
		{
			if ( m_bInit )
				return;

			Super::Init();

			m_music = Audio::AudioEngine::OpenMusic( *m_path );

			SetVolume( m_data.m_volume );
			SetPitch( m_data.m_pitch );
			SetPosition( m_data.m_position );
			SetAttenuation( m_data.m_attenuation );
			SetLoop( m_data.m_isLooping );
			SetRelativeToListener( m_data.m_isRelativeToListener );
			WarpSeekerAt( m_data.m_seekerPosition );
			SetMinimumRangeToHear( m_data.m_minimumRangeToHear );

			if ( m_bPlayOnInit && IsValid() )
				m_music->Play();
		}

		void MusicComponent::OnEnable()
		{
			if ( IsValid() )
				m_music->Play();
		}

		void MusicComponent::OnDisable()
		{
			if ( IsValid() )
				m_music->Stop();
		}

		void MusicComponent::SetVolume( const float& volume )
		{
			if ( IsValid() )
				m_music->SetVolume( volume );
			m_data.m_volume = volume;
		}

		float MusicComponent::GetVolume() const
		{
			if ( IsValid() )
				return m_music->GetVolume();
			return m_data.m_volume;
		}

		void MusicComponent::SetPitch( const float& pitch )
		{
			if ( IsValid() )
				m_music->SetPitch( pitch );
			m_data.m_pitch = pitch;
		}

		float MusicComponent::GetPitch()
		{
			if ( IsValid() )
				return m_music->GetPitch();
			return m_data.m_pitch;
		}

		void MusicComponent::SetPosition( const Math::Vec3& position )
		{
			if ( IsValid() )
				m_music->SetPosition( position );
			m_data.m_position = position;
		}

		Math::Vec3 MusicComponent::GetPosition() const
		{
			if ( IsValid() )
				return m_music->GetPosition();
			return m_data.m_position;
		}

		void MusicComponent::SetAttenuation( const float& attenuation )
		{
			if ( IsValid() )
				m_music->SetAttenuation( attenuation );
			m_data.m_attenuation = attenuation;
		}

		float MusicComponent::GetAttenuation() const
		{
			if ( IsValid() )
				return m_music->GetAttenuation();
			return m_data.m_attenuation;
		}

		void MusicComponent::SetLoop( const bool& value )
		{
			if ( IsValid() )
				m_music->SetLoop( value );
			m_data.m_isLooping = value;
		}

		bool MusicComponent::IsLooping() const
		{
			if ( IsValid() )
				return m_music->IsLooping();
			return m_data.m_isLooping;
		}

		void MusicComponent::SetRelativeToListener( const bool& value )
		{
			if ( IsValid() )
				m_music->SetRelativeToListener( value );
			m_data.m_isRelativeToListener = value;
		}

		void MusicComponent::WarpSeekerAt( const float& position )
		{
			if ( IsValid() )
				m_music->WarpSeekerAt( position );
			m_data.m_seekerPosition = position;
		}

		float MusicComponent::GetSeeker() const
		{
			if ( IsValid() )
				return m_music->GetSeeker();
			return m_data.m_seekerPosition;
		}

		Audio::Data::AudioStatus MusicComponent::GetStatus() const
		{
			if ( IsValid() )
				return m_music->GetStatus();
			return Audio::Data::AudioStatus::FAILURE;
		}

		void MusicComponent::SetMinimumRangeToHear( const float& range )
		{
			if ( IsValid() )
				m_music->SetMinimumRangeToHear( range );
			m_data.m_minimumRangeToHear = range;
		}

		float MusicComponent::GetMinimumRangeToHear() const
		{
			if ( IsValid() )
				return m_music->GetMinimumRangeToHear();
			return m_data.m_minimumRangeToHear;
		}

		void MusicComponent::Play()
		{
			if ( IsValid() )
				m_music->Play();
			else if ( !m_bInit )
				SetPlayOnInit( true );
		}

		void MusicComponent::Play() const
		{
			if ( IsValid() )
				m_music->Play();
		}

		void MusicComponent::Pause() const
		{
			if ( IsValid() )
				m_music->Pause();
		}

		void MusicComponent::Stop()
		{
			if ( IsValid() )
				m_music->Stop();
			else if ( !m_bInit )
				SetPlayOnInit( false );
		}

		void MusicComponent::Stop() const
		{
			if ( IsValid() )
				m_music->Stop();
		}

		float MusicComponent::GetDuration() const
		{
			if ( IsValid() )
				return m_music->GetDuration();
			return -666.f;
		}

		unsigned MusicComponent::GetSampleRate() const
		{
			if ( IsValid() )
				return m_music->GetSampleRate();
			return -666;
		}

		unsigned MusicComponent::GetChannelCount() const
		{
			if ( IsValid() )
				return m_music->GetChannelCount();
			return -666;
		}

		bool MusicComponent::IsValid() const
		{
			return m_music && m_music->IsValid();
		}
	}
}
