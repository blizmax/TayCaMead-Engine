#pragma once
#pragma comment(lib, "TCMAudioEngine/TCMAudioEngine.lib")
#include "TCMAudioEngine/AudioEngine/AudioEngine.h"
#include "TCMComponent.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief \n Struct containing every data an Audio Component should have. \n 
		 */
		struct ENGINECLASSES_DLL_EXPORT AudioData
		{
			float m_volume = 50.f;
			float m_pitch = 1.f;
			Math::Vec3 m_position = Math::Vec3();
			float m_attenuation = 1.f;
			bool m_isLooping = false;
			bool m_isRelativeToListener = false;
			float m_seekerPosition = 0.f;
			float m_minimumRangeToHear = 1.f;

			AudioData() = default;

			AudioData( const AudioData& other )
				: m_volume( other.m_volume )
				, m_pitch( other.m_pitch )
				, m_position( other.m_position )
				, m_attenuation( other.m_attenuation )
				, m_isLooping( other.m_isLooping )
				, m_isRelativeToListener( other.m_isRelativeToListener )
				, m_seekerPosition( other.m_seekerPosition )
				, m_minimumRangeToHear( other.m_minimumRangeToHear )
			{
			}

			AudioData& operator=( const AudioData& other )
			{
				if ( this == &other )
					return *this;
				m_volume = other.m_volume;
				m_pitch = other.m_pitch;
				m_position = other.m_position;
				m_attenuation = other.m_attenuation;
				m_isLooping = other.m_isLooping;
				m_isRelativeToListener = other.m_isRelativeToListener;
				m_seekerPosition = other.m_seekerPosition;
				m_minimumRangeToHear = other.m_minimumRangeToHear;
				return *this;
			}
		};

		/**
		 * \brief \n Abstract Audio Component, base for every Audio Component. \n
		 */
		class ENGINECLASSES_DLL_EXPORT AudioComponent abstract : public TCMComponent
		{
			TCM_CLASS( TCMComponent )

			TCM_SERIALIZE( m_path
				, m_bPlayOnInit
				, m_data );

			AudioComponent()
				: m_bPlayOnInit( false )
				, m_path( new std::string() )
				, m_data( AudioData() )
			{
			}

			virtual ~AudioComponent()
			{
				if ( m_path )
					delete m_path;
			}

			void SetPlayOnInit( const bool bPlayOnInit )
			{
				m_bPlayOnInit = bPlayOnInit;
			}

			virtual void SetVolume( const float& volume ) = 0;
			virtual float GetVolume() const = 0;

			virtual void SetPitch( const float& pitch ) = 0;
			virtual float GetPitch() = 0;

			virtual void SetPosition( const Math::Vec3& position ) = 0;
			virtual Math::Vec3 GetPosition() const = 0;

			virtual void SetAttenuation( const float& attenuation ) = 0;
			virtual float GetAttenuation() const = 0;

			virtual void SetLoop( const bool& value ) = 0;
			virtual bool IsLooping() const = 0;

			virtual void SetRelativeToListener( const bool& value ) = 0;

			virtual void WarpSeekerAt( const float& position ) = 0;
			virtual float GetSeeker() const = 0;

			virtual Audio::Data::AudioStatus GetStatus() const = 0;

			virtual void SetMinimumRangeToHear( const float& range ) = 0;
			virtual float GetMinimumRangeToHear() const = 0;

			virtual void Play() = 0;
			virtual void Play() const = 0;
			virtual void Pause() const = 0;
			virtual void Stop() = 0;
			virtual void Stop() const = 0;

			virtual float GetDuration() const = 0;
			virtual unsigned int GetSampleRate() const = 0;
			virtual unsigned int GetChannelCount() const = 0;

			virtual bool IsValid() const = 0;

			virtual void OnEnable() override
			{
				if ( m_bPlayOnInit )
					Play();
			}

			virtual void OnDisable() override
			{
				Stop();
			}

			void OnPositionChanged( const Math::Vec3& position ) override
			{
				SetPosition( position );
			}

		protected:
			void OnDestroy() override
			{
				Stop();
			}

			bool m_bPlayOnInit;
			std::string* m_path;
			AudioData m_data;
		};
	}
}
