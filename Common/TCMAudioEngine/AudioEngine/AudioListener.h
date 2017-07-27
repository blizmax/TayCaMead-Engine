#pragma once
#include "TCMAudioEngine/AudioEngineDefine.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Audio
		{
			/**
			 * \brief \n Class allowing sounds and musics to be audible in 3d space. \n
			 */
			class AudioListener
			{
			public:
				AudioListener() = default;
				virtual ~AudioListener() = default;

				/**
				 * \brief \n Sets the position of the listener. \n
				 * \param position _IN_ The new position of the listener.
				 */
				static TCMAUDIOENGINE_DLL void SetPosition( const Math::Vec3& position ) { GetInstance().CoreSetPosition( position ); }

				/**
				 * \brief \n Returns position the of the listener. \n
				 * \return The position of the listener.
				 */
				static TCMAUDIOENGINE_DLL Math::Vec3 GetPosition() { return GetInstance().CoreGetPosition(); }

				/**
				 * \brief \n Sets the direction of the listener. \n
				 * \param direction _IN_ The new direction of the listener.
				 */
				static TCMAUDIOENGINE_DLL void SetDirection( const Math::Vec3& direction ) { GetInstance().CoreSetDirection( direction ); }

				/**
				 * \brief \n Returns the direction of the listener.  \n
				 * \return The direction of the listener.
				 */
				static TCMAUDIOENGINE_DLL Math::Vec3 GetDirection() { return GetInstance().CoreGetDirection(); }

				/**
				 * \brief \n Sets the global volume of the listener. \n
				 * \param volume _IN_ The new global volume of the listener.
				 */
				static TCMAUDIOENGINE_DLL void SetGlobalVolume( const float& volume ) { GetInstance().CoreSetGlobalVolume( volume ); }

				/**
				 * \brief \n Returns the global volume of the listener.  \n
				 * \return The global volume of the listener.
				 */
				static TCMAUDIOENGINE_DLL float GetGlobalVolume() { return GetInstance().CoreGetGlobalVolume(); };

			protected:
				virtual void CoreSetPosition( const Math::Vec3& position ) = 0;
				virtual Math::Vec3 CoreGetPosition() = 0;

				virtual void CoreSetDirection( const Math::Vec3& position ) = 0;
				virtual Math::Vec3 CoreGetDirection() = 0;

				virtual void CoreSetGlobalVolume( const float& volume ) = 0;
				virtual float CoreGetGlobalVolume() = 0;

			private:
				static AudioListener& GetInstance();
			};
		}
	}
}
