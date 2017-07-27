#pragma once
#include "TCMComponent.h"
#include "MathLib/ColorRGB.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief \n Abstract class for every light source component. \n
		 */
		class ENGINECLASSES_DLL_EXPORT TCMLightSource abstract : public TCMComponent
		{
			TCM_CLASS(TCMComponent);

			TCM_SERIALIZE(m_data->m_ambientColor,
				m_data->m_diffuseColor,
				m_data->m_specularColor);

			TCMLightSource();
			virtual ~TCMLightSource();

			void TCMPoolCreate() override;
			void TCMPoolRelease() override;

			void TCMEditorUpdate( const float deltaTime ) override;

			/**
			 * \brief \n Resets the parameters of the light source. \n
			 */
			virtual void ResetParams();

			/**
			 * \brief \n Gets the direction of the light. \n
			 * \return The direction of the light.
			 */
			virtual Math::Vec4 GetDirection() const = 0;

			/**
			 * \brief \n Test if light source is directionnal. \n
			 * \return \p true if the value is directionnal, \p false if not.
			 */
			bool IsDirectionnal() const;

			/**
			 * \brief \n Gets the Ambient Color of the light source. \n
			 * \return The ambient color.
			 */
			Math::ColorRGB GetAmbientColor() const;
			/**
			* \brief \n Gets the Diffuse Color of the light source. \n
			* \return The diffuse color.
			*/
			Math::ColorRGB GetDiffuseColor() const;
			/**
			* \brief \n Gets the Specular Color of the light source. \n
			* \return The specular color.
			*/
			Math::ColorRGB GetSpecularColor() const;

			/**
			* \brief \n Sets the Ambient Color of the light source. \n
			* \param color _IN_	The new ambient color.
			*/
			void SetAmbientColor( Math::ColorRGB color ) const;

			/**
			* \brief \n Sets the Diffuse Color of the light source. \n
			* \param color _IN_	The new diffuse color.
			*/
			void SetDiffuseColor( Math::ColorRGB color ) const;

			/**
			* \brief \n Sets the Specular Color of the light source. \n
			* \param color _IN_	The new ambient color.
			*/
			void SetSpecularColor( Math::ColorRGB color ) const;

			/**
			 * \brief \n Gets the Constant Attenuation of the light source. \n
			 * \return The constant attenuation of the light source.
			 */
			float GetConstantAttenuation() const;
			/**
			* \brief \n Gets the Linear Attenuation of the light source. \n
			* \return The linear attenuation of the light source.
			*/
			float GetLinearAttenuation() const;
			/**
			* \brief \n Gets the Quadratic Attenuation of the light source. \n
			* \return The quadratic attenuation of the light source.
			*/
			float GetQuadraticAttenuation() const;

			/**
			 * \brief \n Gets the CutOff of the light source. \n
			 * \return The cutoff of the light source.
			 */
			float GetCutOff() const;
			/**
			 * \brief \n Gets the outer CutOff of the light source. \n
			 * \return The outer cutoff of the light source.
			 */
			float GetOuterCutOff() const;

			/**
			 * \brief \n Gets all light data. \n
			 * \return All light data.
			 */
			Graphics::s_LightData* GetLightData() const;

			/**
			* \brief Called when position is changed
			* \param position : new position of the scene object
			*/
			void OnPositionChanged( const Math::Vec3& position ) override;

			/**
			* \brief Called when rotation is changed
			* \param rotation : new rotation of the scene object
			*/
			void OnRotationChanged( const Math::Vec3& rotation ) override;

		protected:
			/**
			 * \brief Update light data
			 */
			void UpdateData() const;

			void OnEnable() override;
			void OnDisable() override;

			void OnWorldMatrixChanged( const Math::Mat4& mat ) override;

			Math::Mat4 m_lightProjection;
			Graphics::s_LightData* m_data;

		private:
			Graphics::s_LightData* m_lightCached;
		};
	}
}
