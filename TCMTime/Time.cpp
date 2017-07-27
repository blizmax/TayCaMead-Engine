#include "TCMTime/Time.h"

#include <memory>
#include <mutex>

namespace TCM
{
	namespace Engine
	{
#pragma region Constructor and co.
		Time::Time()
			: m_startupTime( TCMCLOCK_T::now() )
			, m_lastTimePoint( TCMCLOCK_T::now() )
			, m_deltaTime( 0 )
			, m_fixedDeltaTime( 0.02f )
			, m_unscaledDeltaTime( 0 )
			, m_unscaledFixedDeltaTime( m_fixedDeltaTime )
			, m_timeScale( 1.0f )
		{
		}

		Time::~Time()
		{
		}

		void Time::Reset()
		{
			GetInstance().CoreReset();
		}

		Time& Time::GetInstance()
		{
			static std::unique_ptr<Time> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new Time() );
			                }
			);
			return *( m_instance.get() );
		}
#pragma endregion

#pragma region Static public methods
		// * Update the timer for this new frame
		void Time::UpdateFrame()
		{
			GetInstance().CoreUpdateFrame();
		}

		// * Get the deltaTime for the last frame
		// * In seconds
		float Time::GetDeltaTime()
		{
			return GetInstance().CoreGetDeltaTime();
		}

		float Time::GetFixedDeltaTime()
		{
			return GetInstance().CoreGetFixedDeltaTime();
		}

		// * Get the unscaled deltaTime for the last frame
		// * In seconds
		float Time::GetUnscaledDeltaTime()
		{
			return GetInstance().CoreGetUnscaledDeltaTime();
		}

		float Time::GetUnscaledFixedDeltaTime()
		{
			return GetInstance().CoreGetUnscaledFixedDeltaTime();
		}

		// * Get the realtime since startup
		// * In seconds
		float Time::GetRealtimeSinceStartup()
		{
			return GetInstance().CoreGetRealtimeSinceStartup();
		}

		// * Get the actual time scale
		float Time::GetTimeScale()
		{
			return GetInstance().CoreGetTimeScale();
		}

		// * Set the actual time scale
		void Time::SetTimeScale( const float& timeScale )
		{
			GetInstance().CoreSetTimeScale( timeScale );
		}

		void Time::SetFixedDeltaTime( const float& fixedDeltaTime )
		{
			GetInstance().CoreSetFixedDeltaTime( fixedDeltaTime );
		}
#pragma endregion

#pragma region Private methods

		void Time::CoreReset()
		{
			m_startupTime = TCMCLOCK_T::now();
			m_lastTimePoint = TCMCLOCK_T::now();
			m_deltaTime = std::chrono::duration<float>( 0.0f );
			m_unscaledDeltaTime = m_deltaTime;
			m_fixedDeltaTime = std::chrono::duration<float>( 0.02f );
			m_unscaledFixedDeltaTime = m_fixedDeltaTime;
			m_timeScale = 1.0f;
		}

		// * Update the timer for this new frame
		void Time::CoreUpdateFrame()
		{
			TCMCLOCK_T::time_point newTimePoint = TCMCLOCK_T::now();

			m_unscaledDeltaTime = newTimePoint - m_lastTimePoint;

			m_deltaTime = m_unscaledDeltaTime;
			m_deltaTime *= m_timeScale;

			m_lastTimePoint = newTimePoint;
		}

		// * Get the deltaTime for the last frame
		// * In seconds
		float Time::CoreGetDeltaTime() const
		{
			return m_deltaTime.count();
		}

		float Time::CoreGetFixedDeltaTime() const
		{
			return m_fixedDeltaTime.count();
		}

		float Time::CoreGetUnscaledFixedDeltaTime() const
		{
			return m_unscaledFixedDeltaTime.count();
		}

		float Time::CoreGetUnscaledDeltaTime() const
		{
			return m_unscaledDeltaTime.count();
		}

		// * Get the realtime since startup
		// * In seconds
		float Time::CoreGetRealtimeSinceStartup() const
		{
			TCMCLOCK_T::time_point newTimePoint = TCMCLOCK_T::now();
			std::chrono::duration<float> result = ( newTimePoint - m_startupTime );
			return ( result ).count();
		}

		// * Get the actual time scale
		float Time::CoreGetTimeScale() const
		{
			return m_timeScale;
		}

		// * Set the actual time scale
		void Time::CoreSetTimeScale( const float& timeScale )
		{
			m_timeScale = timeScale;
			m_fixedDeltaTime = m_unscaledFixedDeltaTime * timeScale;
		}

		void Time::CoreSetFixedDeltaTime( const float& fixedDeltaTime )
		{
			m_unscaledFixedDeltaTime = std::chrono::duration<float>( fixedDeltaTime );
			m_fixedDeltaTime = m_unscaledFixedDeltaTime * m_timeScale;
		}
#pragma endregion
	}
}
