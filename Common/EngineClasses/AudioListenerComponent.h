#pragma once

#include "MathLib/Vec3.h"
#include "TCMComponent.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief \n Component allowing sounds and musics to be audible in 3d space. \n
		*/
		class ENGINECLASSES_DLL_EXPORT AudioListenerComponent : public TCMComponent
		{
			TCM_CLASS(AudioListenerComponent, TCMComponent)

			AudioListenerComponent();

			~AudioListenerComponent() = default;

			/**
			* \brief \n Sets the position of the listener. \n
			* \param position _IN_ The new position of the listener.
			*/
			void SetPosition( const Math::Vec3& position ) const;
			/**
			* \brief \n Returns position the of the listener. \n
			* \return The position of the listener.
			*/
			Math::Vec3 GetPosition() const;

			/**
			* \brief \n Sets the direction of the listener. \n
			* \param direction _IN_ The new direction of the listener.
			*/
			void SetDirection( const Math::Vec3& direction ) const;
			/**
			* \brief \n Returns the direction of the listener.  \n
			* \return The direction of the listener.
			*/
			Math::Vec3 GetDirection() const;

			/**
			* \brief \n Sets the global volume of the listener. \n
			* \param volume _IN_ The new global volume of the listener.
			*/
			void SetGlobalVolume( const float& volume ) const;
			/**
			* \brief \n Returns the global volume of the listener.  \n
			* \return The global volume of the listener.
			*/
			float GetGlobalVolume() const;

			/**
			 * \brief \n Event raised when the position changes. \n
			 * \param position _IN_ The new position.
			 */
			void OnPositionChanged( const Math::Vec3& position ) override;
			/**
			* \brief \n Event raised when the rotation changes. \n
			* \param rotation _IN_ The new rotation.
			*/
			void OnRotationChanged( const Math::Vec3& rotation ) override;
		};
	}
}
