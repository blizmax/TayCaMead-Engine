#pragma once

#include "EngineClassesDefines.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		/**
		* \brief Structure containing transformation data of an object
		*/
		typedef struct ENGINECLASSES_DLL_EXPORT s_TCMTransform
		{
			s_TCMTransform() = default;

			s_TCMTransform( const s_TCMTransform& other );

			s_TCMTransform& operator=( const s_TCMTransform& other );

			friend bool operator==( const s_TCMTransform& lhs, const s_TCMTransform& rhs )
			{
				return lhs.position == rhs.position
						&& lhs.eulerAngles == rhs.eulerAngles
						&& lhs.scale == rhs.scale;
			}

			friend bool operator!=( const s_TCMTransform& lhs, const s_TCMTransform& rhs )
			{
				return !( lhs == rhs );
			}

			/**
			* \brief Position of the object
			*/
			Math::Vec3 position = Math::Vec3( 0.0f, 0.0f, 0.0f );

			/**
			* \brief Euler angles of the object
			*/
			Math::Vec3 eulerAngles = Math::Vec3( 0.0f, 0.0f, 0.0f );

			/**
			* \brief Scale of the object
			*/
			Math::Vec3 scale = Math::Vec3( 1.0f, 1.0f, 1.0f );

			void Reset();
		} Transform;
	}
}
