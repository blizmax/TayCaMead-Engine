#include "TCMRenderer/Gizmo/Gizmo.h"
#include "TCMRenderer/Gizmo/GizmoLine.h"
#include "TCMRenderer/Gizmo/GizmoPoint.h"
#include "TCMRenderer/Gizmo/GizmoMesh.h"
#include <memory>
#include <mutex>
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMRenderer/Rendering/RenderGraph.h"

namespace TCM
{
	namespace Debug
	{
		GizmoMesh::GizmoMesh( Engine::Graphics::AbstractMesh* mesh,
		                      const Engine::Math::Vec3& position,
		                      const Engine::Math::Vec3& rotation,
		                      const Engine::Math::Vec3& scale,
		                      Engine::Math::ColorRGBA color,
		                      bool isWired )
			: m_mesh( mesh )
			, m_color( color )
			, m_isWired( isWired )
		{
			m_matrix = Engine::Math::Mat4::CreateTransform( position,
			                                                rotation,
			                                                scale ).Transposed();
		}

		Engine::Graphics::AbstractMesh* GizmoMesh::GetMesh() const
		{
			return m_mesh;
		}

		Engine::Math::Mat4 GizmoMesh::GetMatrix() const
		{
			return m_matrix;
		}

		Engine::Math::ColorRGBA GizmoMesh::GetColor() const
		{
			return m_color;
		}

		bool GizmoMesh::GetIsWired() const
		{
			return m_isWired;
		}

		void Gizmo::DrawPoint( Engine::Math::Vec3 p )
		{
			GetInstance().CoreDrawPoint( p );
		}

		void Gizmo::DrawLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2 )
		{
			GetInstance().CoreDrawLine( p1, p2 );
		}

		void Gizmo::DrawMesh( Engine::Graphics::s_AbstractMesh* mesh, const Engine::Math::Vec3& position, const Engine::Math::Vec3& rotation, const Engine::Math::Vec3& scale )
		{
			GetInstance().CoreDrawMesh( mesh, position, rotation, scale );
		}

		void Gizmo::DrawWiredMesh( Engine::Graphics::s_AbstractMesh* mesh, const Engine::Math::Vec3& position, const Engine::Math::Vec3& rotation, const Engine::Math::Vec3& scale )
		{
			GetInstance().CoreDrawWiredMesh( mesh, position, rotation, scale );
		}

		void Gizmo::DrawGrid( const int beginWidth, const int endWidth, const int beginLength, const int endLength, const int padding )
		{
			GetInstance().CoreDrawGrid( beginWidth, endWidth, beginLength, endLength, padding );
		}

		void Gizmo::Color( Engine::Math::ColorRGBA color )
		{
			GetInstance().CoreColor( color );
		}

		Gizmo& Gizmo::GetInstance()
		{
			static std::unique_ptr<Gizmo> m_instance;
			static std::once_flag m_onceFlag;

			std::call_once( m_onceFlag,
			                []()
			                {
				                m_instance.reset( new Gizmo() );
			                }
			);
			return *( m_instance.get() );
		}

		void Gizmo::CoreDrawPoint( Engine::Math::Vec3 p ) const
		{
			Engine::Graphics::RenderGraph::RegisterGizmoPoint( GizmoPoint( p, m_currentColor ) );
		}

		void Gizmo::CoreDrawLine( Engine::Math::Vec3 p1, Engine::Math::Vec3 p2 ) const
		{
			Engine::Graphics::RenderGraph::RegisterGizmoLine( GizmoLine( p1, p2, m_currentColor ) );
		}

		void Gizmo::CoreDrawMesh( Engine::Graphics::s_AbstractMesh* mesh, const Engine::Math::Vec3& position, const Engine::Math::Vec3& rotation, const Engine::Math::Vec3& scale ) const
		{
			Engine::Graphics::RenderGraph::RegisterGizmoMesh( GizmoMesh( mesh,
			                                                             position, rotation,
			                                                             scale,
			                                                             m_currentColor,
			                                                             false ) );
		}

		void Gizmo::CoreDrawWiredMesh( Engine::Graphics::s_AbstractMesh* mesh, const Engine::Math::Vec3& position, const Engine::Math::Vec3& rotation, const Engine::Math::Vec3& scale ) const
		{
			Engine::Graphics::RenderGraph::RegisterGizmoMesh( GizmoMesh( mesh,
			                                                             position, rotation,
			                                                             scale,
			                                                             m_currentColor,
			                                                             true ) );
		}

		void Gizmo::CoreDrawGrid( const int beginWidth, const int endWidth, const int beginLength, const int endLength, const int padding ) const
		{
			for ( int i = beginWidth; i < endWidth; i += padding )
			{
				Engine::Math::Vec3 p1( static_cast<float>(i), 0.0f, static_cast<const float>(beginLength) );
				Engine::Math::Vec3 p2( static_cast<float>(i), 0.0f, static_cast<const float>(endLength) );

				Engine::Graphics::RenderGraph::RegisterGizmoLine( GizmoLine( p1, p2, m_currentColor ) );
			}

			for ( int i = beginLength; i < endLength; i += padding )
			{
				Engine::Math::Vec3 p1( static_cast<const float>(beginWidth), 0.0f, static_cast<float>(i) );
				Engine::Math::Vec3 p2( static_cast<const float>(endWidth), 0.0f, static_cast<float>(i) );

				Engine::Graphics::RenderGraph::RegisterGizmoLine( GizmoLine( p1, p2, m_currentColor ) );
			}
		}

		void Gizmo::CoreColor( Engine::Math::ColorRGBA color )
		{
			m_currentColor = color;
		}
	}
}
