#pragma once
#include "TCMComponent.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_AbstractTexture;
			class SpriteData;
		}

		class ENGINECLASSES_DLL_EXPORT TCMComponentDrawable2D :
				public TCMComponent
		{
			TCM_CLASS( TCMComponentDrawable2D, TCMComponent )

		public:

			TCMComponentDrawable2D();
			~TCMComponentDrawable2D();
			/**
			* \brief Pseudo-destructor used when released by pool
			*/
			virtual void TCMPoolCreate() override;

			/**
			* \brief Pseudo-destructor used when released by pool
			*/
			virtual void TCMPoolRelease() override;

			/**
			* \brief Construct the TCMComponentSprite using a name
			* \param filepath _IN_ Filepath of the TCM::Engine::Graphics::AbstractTexture
			*/
			void Construct( const std::string& filepath );

			/**
			* \brief Construct the TCMComponentSprite using a name
			* \param sprite _IN_ TCM::Engine::Graphics::AbstractTexture to draw
			*/
			void Construct( Graphics::s_AbstractTexture* sprite );

			virtual void OnWorldMatrixChanged( const Math::Mat4& mat ) override;

		protected:

			void CalculModelMat();

		private:

			Graphics::SpriteData* m_data;
		};
	}
}
