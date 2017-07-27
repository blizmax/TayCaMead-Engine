#pragma once

#include "TCMTime/TimeDefines.h"

#include <chrono>

typedef std::chrono::high_resolution_clock TCMCLOCK_T;

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Class used for time managing
		 */
		class Time
		{
		public:
			~Time();

#pragma region Static public methods

			/**
			* \brief Update the timer for this new frame
			*/
			static TCMTIME_DLL void Reset();

			/**
			 * \brief Update the timer for this new frame
			 */
			static TCMTIME_DLL void UpdateFrame();

			/**
			 * \brief Get the deltaTime since the last frame
			 * \return DeltaTime since last frame in seconds
			 */
			static TCMTIME_DLL float GetDeltaTime();

			/**
			* \brief Get the fixed deltaTime
			* \return Fixed deltaTime in seconds
			*/
			static TCMTIME_DLL float GetFixedDeltaTime();

			/**
			 * \brief Get the unscaled deltaTime since the last frame
			 * \return Unscaled deltaTime since last frame in seconds
			 */
			static TCMTIME_DLL float GetUnscaledDeltaTime();

			/**
			* \brief Get the unscaled fixed deltaTime
			* \return Unscaled fixed deltaTime in seconds
			*/
			static TCMTIME_DLL float GetUnscaledFixedDeltaTime();

			/**
			 * \brief Get the realtime since startup
			 * \return Realtime since startup in seconds
			 */
			static TCMTIME_DLL float GetRealtimeSinceStartup();

			/**
			 * \brief Get the current time scale
			 * \return Current timeScale
			 */
			static TCMTIME_DLL float GetTimeScale();

			/**
			 * \brief Set the current time scale
			 * \param timeScale _IN_ New Time Scale to be set
			 */
			static TCMTIME_DLL void SetTimeScale( const float& timeScale );

			/**
			* \brief Set the fixed deltaTime
			* \param fixedDeltaTime _IN_ New fixed deltaTime to be set
			*/
			static TCMTIME_DLL void SetFixedDeltaTime( const float& fixedDeltaTime );
#pragma endregion

		private:
			/**
			 * \brief Get the unique instance of the Time
			 * \return A reference to the Time
			 */
			static Time& GetInstance();
			Time();

#pragma	region	Implemented methods

			void CoreReset();

			/**
			 * \brief Update the timer for this new frame
			 */
			void CoreUpdateFrame();

			/**
			* \brief Get the deltaTime since the last frame
			* \return DeltaTime since last frame in seconds
			*/
			float CoreGetDeltaTime() const;

			/**
			* \brief Get the fixed deltaTime
			* \return Fixed deltaTime in seconds
			*/
			float CoreGetFixedDeltaTime() const;

			/**
			 * \brief Get the unscaled deltaTime since the last frame
			 * \return Unscaled deltaTime since last frame in seconds
			 */
			float CoreGetUnscaledDeltaTime() const;

			/**
			* \brief Get the unscaled fixed deltaTime
			* \return Unscaled fixed deltaTime in seconds
			*/
			float CoreGetUnscaledFixedDeltaTime() const;

			/**
			 * \brief Get the realtime since startup
			 * \return Realtime since startup in seconds
			 */
			float CoreGetRealtimeSinceStartup() const;

			/**
			 * \brief Get the current time scale
			 * \return Current timeScale
			 */
			float CoreGetTimeScale() const;

			/**
			 * \brief Set the current time scale
			 * \param timeScale _IN_ New Time Scale to be set
			 */
			void CoreSetTimeScale( const float& timeScale );

			/**
			* \brief Set the fixed deltaTime
			* \param fixedDeltaTime _IN_ New fixed deltaTime to be set
			*/
			void CoreSetFixedDeltaTime( const float& fixedDeltaTime );
#pragma endregion

#pragma region	Members
			/**
			 * \brief Time point set when starting Time
			 * \note Is const, will not change when running
			 */
			TCMCLOCK_T::time_point m_startupTime;

			/**
			 * \brief Time point of the last frame
			 */
			TCMCLOCK_T::time_point m_lastTimePoint;

			/**
			 * \brief Delta Time of the last frame
			 */
			std::chrono::duration<float> m_deltaTime;

			/**
			* \brief Delta Time of the last frame
			*/
			std::chrono::duration<float> m_fixedDeltaTime;

			/**
			 * \brief Unscaled Delta Time of the last frame
			 */
			std::chrono::duration<float> m_unscaledDeltaTime;

			/**
			* \brief Unscaled Delta Time of the last frame
			*/
			std::chrono::duration<float> m_unscaledFixedDeltaTime;

			/**
			 * \brief Current Time Scale
			 */
			float m_timeScale;
#pragma endregion
		};
	}
}
