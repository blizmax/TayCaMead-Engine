#pragma once
#include "TCMLightSource.h"

#define MIN_ATTENUATION 0.0001

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief \n Component for an attenuated light. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMAttenuatedLight abstract : public TCMLightSource
		{
			TCM_CLASS(TCMLightSource);

			TCM_SERIALIZE( m_data->m_constantAttenuation,
				m_data->m_linearAttenuation,
				m_data->m_quadraticAttenuation );

		public:
			TCMAttenuatedLight();

			void TCMPoolCreate() override;

			void ResetParams() override;

			void OnDrawGizmos() override;
			/**
			 * \brief \n Sets the constant attenuation of the light. \n
			 * \param value _IN_	 value of constant attenuation (0->1)
			 */
			void SetConstantAttenuation( float value ) const;
			/**
			 * \brief \n Sets the linear attenuation of the light. \n
			 * \param value _IN_	 value of linear attenuation (0->1)
			 */
			void SetLinearAttenuation( float value ) const;
			/**
			 * \brief \n Sets the quadratic attenuation of the light. \n
			 * \param value _IN_	 value of quadratic attenuation (0->1)
			 */
			void SetQuadraticAttenuation( float value ) const;

		private:
			Graphics::AbstractMesh* m_gizmoMesh;
		};
	}
}
