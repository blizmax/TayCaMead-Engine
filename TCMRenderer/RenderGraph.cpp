#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMRenderer/Rendering/RenderableObject.h"
#include "TCMRenderer/Gizmo/GizmoPoint.h"
#include "TCMRenderer/Gizmo/GizmoLine.h"
#include "TCMRenderer/Camera/CameraData.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include <mutex>

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			RenderGraph::RenderGraph() : m_currentSkybox( nullptr )
			{
			}

			RenderGraph::~RenderGraph()
			{
			}

			RenderGraph& RenderGraph::GetInstance()
			{
				static std::unique_ptr<RenderGraph> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new RenderGraph() );
				                }
				);
				return *( m_instance.get() );
			}

			void RenderGraph::PreRenderScene( RenderableObject* scene )
			{
				GetInstance().CorePreRenderScene( scene );
			}

			void RenderGraph::RenderScene( Camera::CameraData* camera )
			{
				GetInstance().CoreRenderScene( camera );
			}

			void RenderGraph::PreRender()
			{
				GetInstance().CorePreRender();
			}

			void RenderGraph::PostRender()
			{
				GetInstance().CorePostRender();
			}

			void RenderGraph::RegisterRenderData( const RenderData* renderData )
			{
				GetInstance().CoreRegisterRenderData( renderData );
			}

			void RenderGraph::UnregisterRenderData( const RenderData* renderData )
			{
				GetInstance().CoreUnregisterRenderData( renderData );
			}

			void RenderGraph::RegisterSprite( const SpriteData* texture )
			{
				GetInstance().CoreRegisterSprite( texture );
			}

			void RenderGraph::UnregisterSprite( const SpriteData* texture )
			{
				GetInstance().CoreUnregisterSprite( texture );
			}

			void RenderGraph::RegisterUISprite( const AbstractTexture* texture, const Math::Mat4& worldMatrix )
			{
				GetInstance().CoreRegisterUISprite( texture, worldMatrix );
			}

			void RenderGraph::RegisterSkybox( AbstractSkybox* skyboxData )
			{
				GetInstance().CoreRegisterSkybox( skyboxData );
			}

			void RenderGraph::RegisterLight( LightData* data )
			{
				GetInstance().CoreRegisterLight( data );
			}

			void RenderGraph::UnregisterLight( LightData* data )
			{
				GetInstance().CoreUnregisterLight( data );
			}

			void RenderGraph::RegisterGizmoLine( const Debug::GizmoLine& line )
			{
				GetInstance().CoreRegisterGizmoLine( line );
			}

			void RenderGraph::RegisterGizmoPoint( const Debug::GizmoPoint& point )
			{
				GetInstance().CoreRegisterGizmoPoint( point );
			}

			void RenderGraph::RegisterGizmoMesh( const Debug::GizmoMesh& mesh )
			{
				GetInstance().CoreRegisterGizmoMesh( mesh );
			}

			void RenderGraph::PasteBufferOnScreen( FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size )
			{
				GetInstance().CorePasteBufferOnScreen( frameBuffer, position, size );
			}

			void RenderGraph::UnregisterAll()
			{
				GetInstance().CoreUnregisterAll();
			}

			const RenderData* RenderGraph::SphereRaycast( const Math::Vec3& origin, const Math::Vec3& direction )
			{
				return GetInstance().CoreSphereRaycast( origin, direction );
			}

			void RenderGraph::CorePreRenderScene( RenderableObject* scene ) const
			{
				scene->TCMRender();
				CorePreRender();
			}

			void RenderGraph::CoreRenderScene( Camera::CameraData* camera )
			{
				AbstractRenderer::DrawSkybox( camera, m_currentSkybox );

				m_visibleRenderData.clear();
				m_visibleLights.clear();

				size_t lightCount = m_lightList.size();
				size_t renderDataCount = m_renderDataList.size();

				for ( size_t i = 0; i < renderDataCount; ++i )
				{
					const RenderData* data = m_renderDataList[i];
					Math::Bounds::Sphere s = data->GetBoundingSphere();
					if ( camera->IsSphereSeen( s ) && data->m_enabled )
						m_visibleRenderData.push_back( data );
				}

				for ( size_t i = 0; i < lightCount; ++i )
				{
					LightData* light = m_lightList[i];
					if ( light->m_enabled == false )
						continue;
					if ( !light->IsDirectionnal() )
					{
						if ( !camera->IsSphereSeen( m_lightList[i]->GetBoundingSphere() ) )
							continue;
					}
					m_visibleLights.push_back( light );
				}

				AbstractRenderer::Render( camera, m_visibleRenderData, m_visibleLights );

				if ( camera->m_debugCamera )
				{
					AbstractRenderer::RenderGizmoLines( camera, m_gizmoLines, m_gizmoLinesCount );
					AbstractRenderer::RenderGizmoPoints( camera, m_gizmoPoints, m_gizmoPointsCount );
					for ( int i = 0; i < m_gizmoMeshes.size(); ++i )
						AbstractRenderer::RenderGizmoMesh( camera, m_gizmoMeshes[i] );
				}

				if ( camera->GetPostProcessEffects()->size() > 0 )
					AbstractRenderer::PostProcess( camera );
			}

			void RenderGraph::CorePreRender() const
			{
				AbstractRenderer::PreRender();
			}

			void RenderGraph::CorePostRender()
			{
				AbstractRenderer::PostRender();
				CoreUnregisterGizmos();
			}

			void RenderGraph::CoreRegisterRenderData( const RenderData* renderData )
			{
				m_renderDataList.insert
				(
					upper_bound( m_renderDataList.begin(), m_renderDataList.end(), renderData, AbstractRenderer::GetComparer() ),
					renderData
				);
			}

			void RenderGraph::CoreUnregisterRenderData( const RenderData* renderData )
			{
				for ( auto it = m_renderDataList.begin(); it != m_renderDataList.end(); ++it )
					if ( *it == renderData )
					{
						m_renderDataList.erase( it );
						break;
					}
			}

			void RenderGraph::CoreRegisterSprite( const SpriteData* texture )
			{
				m_spriteList.push_back( texture );
			}

			void RenderGraph::CoreUnregisterSprite( const SpriteData* texture )
			{
				for ( auto it = m_spriteList.begin(); it != m_spriteList.end(); ++it )
					if ( *it == texture )
					{
						m_spriteList.erase( it );
						break;
					}
			}

			void RenderGraph::CoreRegisterUISprite( const AbstractTexture* texture, const Math::Mat4& worldMatrix )
			{
				m_uiSpriteList.push_back( std::make_pair( texture, worldMatrix ) );
			}

			void RenderGraph::CoreRegisterSkybox( AbstractSkybox* skyboxData )
			{
				m_currentSkybox = skyboxData;
			}

			void RenderGraph::CoreRegisterLight( LightData* data )
			{
				m_lightList.push_back( data );
			}

			void RenderGraph::CoreUnregisterLight( LightData* data )
			{
				for ( auto it = m_lightList.begin(); it != m_lightList.end(); ++it )
					if ( *it == data )
					{
						m_lightList.erase( it );
						break;
					}
			}

			void RenderGraph::CoreRegisterGizmoLine( const Debug::GizmoLine& line )
			{
				CoreRegisterGizmoPoint( line.m_p1, m_gizmoLines );
				CoreRegisterGizmoPoint( line.m_p2, m_gizmoLines );
				++m_gizmoLinesCount;
			}

			void RenderGraph::CoreRegisterGizmoPoint( const Debug::GizmoPoint& point )
			{
				CoreRegisterGizmoPoint( point, m_gizmoPoints );
				++m_gizmoPointsCount;
			}

			void RenderGraph::CoreRegisterGizmoPoint( const Debug::GizmoPoint& point, std::vector<float>& list ) const
			{
				list.push_back( point.m_p.x );
				list.push_back( point.m_p.y );
				list.push_back( point.m_p.z );

				list.push_back( point.m_color.r );
				list.push_back( point.m_color.g );
				list.push_back( point.m_color.b );
				list.push_back( point.m_color.a );
			}

			void RenderGraph::CoreRegisterGizmoMesh( const Debug::GizmoMesh& mesh )
			{
				m_gizmoMeshes.push_back( mesh );
			}

			void RenderGraph::CoreUnregisterGizmos()
			{
				m_gizmoLines.clear();
				m_gizmoLinesCount = 0;
				m_gizmoPoints.clear();
				m_gizmoPointsCount = 0;
				m_gizmoMeshes.clear();
			}

			void RenderGraph::CorePasteBufferOnScreen( FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size ) const
			{
				AbstractRenderer::PasteBufferOnScreen( frameBuffer, position, size );
			}

			void RenderGraph::CoreUnregisterAll()
			{
				m_renderDataList.clear();
				m_lightList.clear();
			}

			const RenderData* RenderGraph::CoreSphereRaycast( const Math::Vec3& origin, const Math::Vec3& direction )
			{
				float minDistance = std::numeric_limits<float>::max();
				const RenderData* nearObject = nullptr;

				for ( const RenderData* data : m_renderDataList )
				{
					//TODO: Optimize maybe 
					//http://antongerdelan.net/opengl/raycasting.html

					Math::Bounds::Sphere sphere = data->GetBoundingSphere();
					Math::Vec3 toObject = sphere.position - origin;
					Math::Vec3 directionScaled = direction * toObject.Norm();

					float distance = directionScaled.DistanceSquaredTo( toObject );
					if ( distance < sphere.radius * sphere.radius )
						if ( distance < minDistance )
						{
							minDistance = distance;
							nearObject = data;
						}
				}
				return nearObject;
			}
		}
	}
}
