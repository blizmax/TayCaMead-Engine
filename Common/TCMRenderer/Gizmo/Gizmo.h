#pragma once

#include "TCMRenderer/RendererDefines.h"
#include "MathLib/Vec3.h"
#include "MathLib/ColorRGBA.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_AbstractMesh;
		}
	}

	namespace Debug
	{
		class Gizmo
		{
		public:
			virtual ~Gizmo() = default;

			static TCMRENDERER_DLL void DrawPoint( Engine::Math::Vec3 p );
			static TCMRENDERER_DLL void DrawLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2 );
			static TCMRENDERER_DLL void DrawMesh( Engine::Graphics::s_AbstractMesh* mesh,
			                                      const Engine::Math::Vec3& position = { 0.0f, 0.0f, 0.0f },
			                                      const Engine::Math::Vec3& rotation = { 0.0f, 0.0f, 0.0f },
			                                      const Engine::Math::Vec3& scale = { 1.0f, 1.0f, 1.0f } );
			static TCMRENDERER_DLL void DrawWiredMesh( Engine::Graphics::s_AbstractMesh* mesh,
			                                           const Engine::Math::Vec3& position = { 0.0f, 0.0f, 0.0f },
			                                           const Engine::Math::Vec3& rotation = { 0.0f, 0.0f, 0.0f },
			                                           const Engine::Math::Vec3& scale = { 1.0f, 1.0f, 1.0f } );

			static TCMRENDERER_DLL void DrawGrid( const int beginWidth, const int endWidth, const int beginLength, const int endLength, const int padding );
			static TCMRENDERER_DLL void Color( Engine::Math::ColorRGBA color );

		private:
			/**
			* \brief Get the unique instance of the Gizmo
			* \return A reference to the Gizmo
			*/
			static Gizmo& GetInstance();

		protected:
			Gizmo() = default;
			void CoreDrawPoint( Engine::Math::Vec3 p ) const;
			void CoreDrawLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2 ) const;
			void CoreDrawMesh( Engine::Graphics::s_AbstractMesh* mesh,
			                   const Engine::Math::Vec3& position = { 0.0f, 0.0f, 0.0f },
			                   const Engine::Math::Vec3& rotation = { 0.0f, 0.0f, 0.0f },
			                   const Engine::Math::Vec3& scale = { 1.0f, 1.0f, 1.0f } ) const;
			void CoreDrawWiredMesh( Engine::Graphics::s_AbstractMesh* mesh,
			                        const Engine::Math::Vec3& position = { 0.0f, 0.0f, 0.0f },
			                        const Engine::Math::Vec3& rotation = { 0.0f, 0.0f, 0.0f },
			                        const Engine::Math::Vec3& scale = { 1.0f, 1.0f, 1.0f } ) const;

			void CoreDrawGrid( const int beginWidth, const int endWidth, const int beginLength, const int endLength, const int padding ) const;
			void CoreColor( Engine::Math::ColorRGBA color );

			Engine::Math::ColorRGBA m_currentColor;
		};
	}
}
