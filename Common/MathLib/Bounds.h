#pragma once
#include <vector>
#include "MathLibDefine.h"
#include "MathLib/Vec3.h"

namespace TCM
{
	namespace Engine
	{
		namespace Math
		{
			class MATHLIB_DLL_EXPORT Bounds abstract
			{
			public:
				struct Box;
				struct Sphere;

				struct MATHLIB_DLL_EXPORT Sphere
				{
					/**
					 * \brief Center of the \c Sphere
					 */
					Vec3 position = { 0.0f, 0.0f, 0.0f };

					/**
					 * \brief Radius of the \c Sphere
					 */
					float radius = 0.0f;

					/**
					 * \brief Get the \c Box from this \c Sphere
					 * \return The \c Box bounds
					 */
					Box ToBox() const;

					/**
					 * \brief Gets if the \p point is inside the \c Sphere
					 * \param point _IN_ The point to be tested
					 * \return True if is inside, false otherwise
					 */
					bool IsInside( const Vec3& point ) const;

					/**
					 * \brief Gets if the \p sphere is colliding with the current \c Sphere
					 * \param sphere _IN_ The sphere to be tested
					 * \return True if is colliding, false otherwise
					 */
					bool IsColliding( const Sphere& sphere ) const;
				};

				struct MATHLIB_DLL_EXPORT Box
				{
					/**
					 * \brief Center of the \c Box
					 */
					Vec3 position = { 0.0f, 0.0f, 0.0f };

					/**
					 * \brief Width of the \c Box
					 */
					float width;

					/**
					 * \brief Height of the \c Box
					 */
					float height;

					/**
					 * \brief Length of the \c Box
					 */
					float length;

					/**
					 * \brief Get the \c Sphere from this \c Box
					 * \return The \c Sphere bounds
					 */
					Sphere ToSphere() const;

					/**
					 * \brief Gets if the \p point is inside the \c Box
					 * \param point _IN_ The point to be tested
					 * \return True if is inside, false otherwise
					 */
					bool IsInside( const Vec3& point ) const;

					/**
					 * \brief Create box from Vector3
					 * \param size _IN_ Vec3 with width, height, length of the box
					 * \return 
					 */
					static Box FromVec3( const Math::Vec3& size );
				};

				/**
				 * \brief Verify if a \c Sphere is in one frustum
				 * \param frustum _IN_ The frustum to be tested
				 * \param sphere _IN_ The \c Sphere to be tested
				 * \return True if is in frustum, false otherwise
				 */
				static bool IsInFrustum( const float frustum[24], const Sphere& sphere );

				/**
				 * \brief Verify if a \c Box is in one frustum
				 * \param frustum _IN_ The frustum to be tested
				 * \param box _IN_ The \c Box to be tested
				 * \return True if is in frustum, false otherwise
				 */
				static bool IsInFrustum( const float frustum[24], const Box& box );
			};
		}
	}
}
