#include "TCMRenderer/Camera/CameraData.h"
#include "TCMRenderer/Camera/CameraManager.h"
#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "MathLib/MathUtils.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Camera
			{
				CameraData::CameraData( Math::Vec2 ratio, bool debugCamera ) : m_debugCamera( debugCamera )
				                                                             , m_ratio( ratio )
				                                                             , m_framebuffer( nullptr )
				                                                             , m_specificData( nullptr )
				{
					m_viewProjectionMatrix = new ViewProjectionMatrix;
					m_postProcess = new std::vector<PostProcess::PostProcess *>();
				}

				CameraData::~CameraData()
				{
					CameraManager::UnregisterCamera( this );
					if ( m_framebuffer )
						delete m_framebuffer;
					delete m_viewProjectionMatrix;
					delete m_postProcess;
					delete m_specificData;
				}

				void CameraData::Init()
				{
					m_framebuffer = CameraManager::CreateRendererFramebuffer( m_ratio );
					m_specificData = RendererFactory::CreateCameraSpecificData();
					CameraManager::RegisterCamera( this );
					UpdateCameraTransform();
					UpdateCameraParams();
				}

				void CameraData::Clear() const
				{
					m_framebuffer->ClearBuffer( m_color );
				}

				void CameraData::Render()
				{
					Clear();
					RenderGraph::RenderScene( this );
				}

				void CameraData::Resize()
				{
					int width, height;
					AbstractRenderer::GetScreenSize( width, height );

					width = static_cast<int>(round( width * m_ratio.x ));
					height = static_cast<int>(round( height * m_ratio.y ));

					m_framebuffer->Resize( width, height );
					UpdateCameraParams();
				}

				void CameraData::ForceRender()
				{
					if ( m_enabled )
						return;
					Render();
				}

				void CameraData::ForceBufferCreation()
				{
					if ( m_framebuffer )
						delete m_framebuffer;
					m_framebuffer = CameraManager::CreateRendererFramebuffer( m_ratio );
				}

				void CameraData::OnPositionChanged( const Math::Vec3& position )
				{
					m_worldPosition = position;
					UpdateCameraTransform();
				}

				void CameraData::OnRotationChanged( const Math::Vec3& rotation )
				{
					m_worldRotation = rotation;
					UpdateCameraTransform();
				}

				void CameraData::SetFov( const float& degree )
				{
					m_fov = degree;
					UpdateCameraParams();
				}

				void CameraData::SetNearDistance( const float& value )
				{
					m_nearDistance = value;
					UpdateCameraParams();
				}

				void CameraData::SetFarDistance( const float& value )
				{
					m_farDistance = value;
					UpdateCameraParams();
				}

				void CameraData::SetOrthoSize( const float& value )
				{
					m_orthoSize = value;
					UpdateCameraParams();
				}

				void CameraData::SetDepth( const int value )
				{
					m_depth = value;
					CameraManager::Sort();
				}

				void CameraData::SetOrthographic( const bool value )
				{
					m_isOrtho = value;
					UpdateCameraParams();
				}

				void CameraData::SetEnable( const bool value )
				{
					m_enabled = value;
				}

				void CameraData::SetRatio( const Math::Vec2& ratio )
				{
					m_ratio = ratio;
					UpdateCameraParams();
				}

				float CameraData::GetFovDegree() const
				{
					return m_fov;
				}

				float CameraData::GetFovRadian() const
				{
					return Math::Utils::ToRadian( m_fov );
				}

				float CameraData::GetNearDistance() const
				{
					return m_nearDistance;
				}

				float CameraData::GetFarDistance() const
				{
					return m_farDistance;
				}

				float CameraData::GetOrthoSize() const
				{
					return m_orthoSize;
				}

				int CameraData::GetDepth() const
				{
					return m_depth;
				}

				Math::Vec2 CameraData::GetRatio() const
				{
					return m_ratio;
				}

				Math::Vec3 CameraData::GetPosition() const
				{
					return m_worldPosition;
				}

				FrameBuffer* CameraData::GetFramebuffer() const
				{
					return m_framebuffer;
				}

				AbstractTexture* CameraData::GetTexture() const
				{
					if ( m_framebuffer )
						return m_framebuffer->GetTexture();
					return nullptr;
				}

				CameraSpecificData* CameraData::GetSpecificData() const
				{
					return m_specificData;
				}

				bool CameraData::IsOnScreen() const
				{
					return m_isOnScreen;
				}

				bool CameraData::IsFocused( const Math::Vec2& mousePos ) const
				{
					if ( !IsOnScreen() )
						return false;
					bool x = mousePos.x <= m_ratio.x + m_position.x && mousePos.x >= m_position.x;
					bool y = mousePos.y <= m_ratio.y + m_position.y && mousePos.y >= m_position.y;

					return x && y;
				}

				bool CameraData::IsOrhtographic() const
				{
					return m_isOrtho;
				}

				void CameraData::AddPostProcessEffect( const PostProcessEffect& effect ) const
				{
					if ( effect == PostProcessEffect::COUNT || effect == PostProcessEffect::NONE )
						return;
					m_postProcess->push_back( PostProcess::PostProcessManager::GetPostProcess( effect ) );
				}

				void CameraData::AddFirstPostProcessEffect( const PostProcessEffect& effect ) const
				{
					if ( effect == PostProcessEffect::COUNT || effect == PostProcessEffect::NONE )
						return;
					m_postProcess->insert( m_postProcess->begin(), PostProcess::PostProcessManager::GetPostProcess( effect ) );
				}

				void CameraData::RemovePostProcessEffect( const PostProcessEffect& effect ) const
				{
					auto iter = m_postProcess->begin();
					for ( ; iter != m_postProcess->end(); ++iter )
					{
						if ( ( *iter )->GetType() == effect )
							break;
					}

					if ( iter != m_postProcess->end() )
						m_postProcess->erase( iter );
				}

				void CameraData::RemoveFirstPostProcessEffect() const
				{
					if ( m_postProcess->size() > 0 )
						m_postProcess->erase( m_postProcess->begin() );
				}

				void CameraData::RemoveLastPostProcessEffect() const
				{
					m_postProcess->pop_back();
				}

				void CameraData::SetPostProcessEffects( const std::vector<PostProcessEffect>& effects ) const
				{
					m_postProcess->clear();
					for ( int i = 0; i < effects.size(); ++i )
					{
						m_postProcess->push_back( PostProcess::PostProcessManager::GetPostProcess( effects[i] ) );
					}
				}

				void CameraData::ClearPostProccesses() const
				{
					m_postProcess->clear();
				}

				const RenderData* CameraData::ViewportPointToRay( const Math::Vec2& position ) const
				{
					if ( position.x < 0 || position.x > 1 || position.y < 0 || position.y > 1 )
						return nullptr;
					Math::Vec3 deviceCoord = { position.x * 2.0f - 1.0f,
						position.y * 2.0f - 1.0f,
						1.0f };
					Math::Vec4 clipCoord = { deviceCoord.x * -1,
						deviceCoord.y,
						-1.0f,
						1.0f
					};
					Math::Vec4 ray_eye = m_viewProjectionMatrix->projectionMatrix.Inverted() * clipCoord;
					ray_eye.z = -1.0f;
					ray_eye.w = 0.0;

					Math::Vec3 ray_wor( m_viewProjectionMatrix->viewMatrix.Inverted() * ray_eye );
					ray_wor.Normalize();
					ray_wor.x *= -1;
					return RenderGraph::SphereRaycast( m_worldPosition, ray_wor.Normalized() );
				}

				std::vector<PostProcess::PostProcess *>* CameraData::GetPostProcessEffects() const
				{
					return m_postProcess;
				}

				void CameraData::UpdateCameraTransform()
				{
					m_viewProjectionMatrix->viewMatrix =
							( Math::Mat4::CreateTransposedTranslation( m_worldPosition ) * Math::Mat4::CreateRotation( m_worldRotation ) ).Inverted();
					UpdateMVP();
				}

				void CameraData::UpdateCameraParams()
				{
					if ( m_isOrtho )
						m_viewProjectionMatrix->projectionMatrix = RendererFactory::GenOrthoMatrix( m_ratio, m_nearDistance, m_farDistance ).Transposed();
					else
						m_viewProjectionMatrix->projectionMatrix = RendererFactory::GenProjMatrix( m_ratio, m_fov, m_nearDistance, m_farDistance ).Transposed();
					UpdateMVP();
				}

				void CameraData::UpdateMVP()
				{
					m_viewProjectionMatrix->mvpMatrix = m_viewProjectionMatrix->projectionMatrix
							* m_viewProjectionMatrix->viewMatrix;
					RendererFactory::UpdateCameraData( this );
					UpdateFrustum( m_viewProjectionMatrix->mvpMatrix );
				}

				void CameraData::UpdateFrustum( const Math::Mat4 clip )
				{
					Math::Vec3 vec;
					float norm;

					/* Extract the numbers for the RIGHT plane */
					vec.x = clip.GetRawValueAt( 3 ) - clip.GetRawValueAt( 0 );
					vec.y = clip.GetRawValueAt( 7 ) - clip.GetRawValueAt( 4 );
					vec.z = clip.GetRawValueAt( 11 ) - clip.GetRawValueAt( 8 );
					m_frustum[3] = clip.GetRawValueAt( 15 ) - clip.GetRawValueAt( 12 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[3] /= norm;

					m_frustum[0] = vec.x;
					m_frustum[1] = vec.y;
					m_frustum[2] = vec.z;

					/* Extract the numbers for the LEFT plane */
					vec.x = clip.GetRawValueAt( 3 ) + clip.GetRawValueAt( 0 );
					vec.y = clip.GetRawValueAt( 7 ) + clip.GetRawValueAt( 4 );
					vec.z = clip.GetRawValueAt( 11 ) + clip.GetRawValueAt( 8 );
					m_frustum[7] = clip.GetRawValueAt( 15 ) + clip.GetRawValueAt( 12 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[7] /= norm;

					m_frustum[4] = vec.x;
					m_frustum[5] = vec.y;
					m_frustum[6] = vec.z;

					/* Extract the BOTTOM plane */
					vec.x = clip.GetRawValueAt( 3 ) + clip.GetRawValueAt( 1 );
					vec.y = clip.GetRawValueAt( 7 ) + clip.GetRawValueAt( 5 );
					vec.z = clip.GetRawValueAt( 11 ) + clip.GetRawValueAt( 9 );
					m_frustum[11] = clip.GetRawValueAt( 15 ) + clip.GetRawValueAt( 13 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[11] /= norm;

					m_frustum[8] = vec.x;
					m_frustum[9] = vec.y;
					m_frustum[10] = vec.z;

					/* Extract the TOP plane */
					vec.x = clip.GetRawValueAt( 3 ) - clip.GetRawValueAt( 1 );
					vec.y = clip.GetRawValueAt( 7 ) - clip.GetRawValueAt( 5 );
					vec.z = clip.GetRawValueAt( 11 ) - clip.GetRawValueAt( 9 );
					m_frustum[15] = clip.GetRawValueAt( 15 ) - clip.GetRawValueAt( 13 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[15] /= norm;

					m_frustum[12] = vec.x;
					m_frustum[13] = vec.y;
					m_frustum[14] = vec.z;

					/* Extract the FAR plane */
					vec.x = clip.GetRawValueAt( 3 ) - clip.GetRawValueAt( 2 );
					vec.y = clip.GetRawValueAt( 7 ) - clip.GetRawValueAt( 6 );
					vec.z = clip.GetRawValueAt( 11 ) - clip.GetRawValueAt( 10 );
					m_frustum[19] = clip.GetRawValueAt( 15 ) - clip.GetRawValueAt( 14 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[19] /= norm;

					m_frustum[16] = vec.x;
					m_frustum[17] = vec.y;
					m_frustum[18] = vec.z;

					/* Extract the NEAR plane */
					vec.x = clip.GetRawValueAt( 3 ) + clip.GetRawValueAt( 2 );
					vec.y = clip.GetRawValueAt( 7 ) + clip.GetRawValueAt( 6 );
					vec.z = clip.GetRawValueAt( 11 ) + clip.GetRawValueAt( 10 );
					m_frustum[23] = clip.GetRawValueAt( 15 ) + clip.GetRawValueAt( 14 );

					norm = vec.Norm();
					/* Normalize the result */
					vec.Normalize();
					m_frustum[23] /= norm;

					m_frustum[20] = vec.x;
					m_frustum[21] = vec.y;
					m_frustum[22] = vec.z;
				}

				bool CameraData::IsSphereSeen( const Math::Bounds::Sphere& sphere ) const
				{
					return Math::Bounds::IsInFrustum( m_frustum, sphere );
				}
			}
		}
	}
}
