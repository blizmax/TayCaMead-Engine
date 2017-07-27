#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMRenderer/Rendering/RenderGraph.h"
#include "TCMRenderer/Camera/CameraManager.h"
#include "MathLib/MathUtils.h"
#include "TCMLogger/Logger.h"
#include <mutex>

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Camera
			{
				CameraManager& CameraManager::GetInstance()
				{
					static std::unique_ptr<CameraManager> m_instance;
					static std::once_flag m_onceFlag;

					std::call_once( m_onceFlag,
					                []()
					                {
						                m_instance.reset( new CameraManager() );
					                }
					);
					return *( m_instance.get() );
				}

				void CameraManager::Render( RenderableObject* scene )
				{
					GetInstance().CoreRender( scene );
				}

				CameraData* CameraManager::GetMainCamera()
				{
					return GetInstance().CoreGetMainCamera();
				}

				int CameraManager::GetMainCameraIndex()
				{
					return GetInstance().CoreGetMainCameraIndex();
				}

				CameraData* CameraManager::GetCamera( const int index )
				{
					return GetInstance().CoreGetCamera( index );
				}

				int CameraManager::GetIndex( CameraData* camera )
				{
					return GetInstance().CoreGetIndex( camera );
				}

				int CameraManager::GetIndex( CameraData const* camera )
				{
					return GetInstance().CoreGetIndex( camera );
				}

				void CameraManager::UnregisterCamera( const int index )
				{
					GetInstance().CoreRemoveCamera( index );
				}

				void CameraManager::UnregisterCamera( CameraData* camera )
				{
					GetInstance().CoreRemoveCamera( camera );
				}

				void CameraManager::UnregisterAllCameras()
				{
					GetInstance().CoreUnregisterAllCameras();
				}

				size_t CameraManager::CamerasCount()
				{
					return GetInstance().CoreCamerasCount();
				}

				void CameraManager::RegisterCamera( CameraData* camera )
				{
					GetInstance().CoreRegisterCamera( camera );
				}

				void CameraManager::SetCameraCountOnScreen( const int count )
				{
					GetInstance().CoreSetCameraCountOnScreen( count );
				}

				size_t CameraManager::GetCameraCountOnScreen()
				{
					return GetInstance().CoreGetCameraCountOnScreen();
				}

				void CameraManager::SwitchCameras()
				{
					GetInstance().CoreSwitchCameras();
				}

				void CameraManager::Sort()
				{
					GetInstance().CoreSort();
				}

				void CameraManager::ResizeViewport( const int width, const int height )
				{
					GetInstance().CoreResizeViewport( width, height );
				}

				void CameraManager::SetMSAASettings( bool enabled, int count )
				{
					GetInstance().CoreSetMSAASettings( enabled, count );
				}

				bool CameraManager::IsMSAAEnabled()
				{
					return GetInstance().m_MSAAEnabled;
				}

				int CameraManager::GetMSAASamplesCount()
				{
					return GetInstance().m_MSAASamples;
				}

				FrameBuffer* CameraManager::CreateRendererFramebuffer( const Math::Vec2& size )
				{
					if ( GetInstance().m_MSAAEnabled )
						return RendererFactory::CreateRendererFramebuffer( size, FrameBufferType::MSAA, GetInstance().m_MSAASamples );
					else
						return RendererFactory::CreateRendererFramebuffer( size, FrameBufferType::NONE );
				}

				void CameraManager::CoreRender( RenderableObject* scene )
				{
					RenderGraph::PreRenderScene( scene );

					size_t size = CoreCamerasCount();

					int mainCam = CoreGetMainCameraIndex();
					int index;
					int i, j = 0;

					size_t renderedCamCount = CoreGetCameraCountOnScreen();

					for ( i = 0; i < size; ++i )
					{
						index = ( mainCam + i ) % size;
						CameraData* camera = m_cameras[index];
						if ( camera->m_enabled )
						{
							camera->Render();
							if ( j < renderedCamCount )
							{
								camera->m_isOnScreen = true;
								RenderGraph::PasteBufferOnScreen( camera->GetFramebuffer(), camera->m_position, camera->GetRatio() );
								++j;
								continue;
							}
						}
						camera->m_isOnScreen = false;
					}

					if ( j < renderedCamCount )
					TCMWARNING( "Not enough (enabled) camera" );
				}

				CameraData* CameraManager::CoreGetMainCamera()
				{
					return m_cameras[m_currentCamera];
				}

				int CameraManager::CoreGetMainCameraIndex() const
				{
					return m_currentCamera;
				}

				CameraData* CameraManager::CoreGetCamera( const int index )
				{
					if ( index < 0 || index >= m_cameras.size() )
						return nullptr;
					return m_cameras[index];
				}

				int CameraManager::CoreGetIndex( CameraData const* camera )
				{
					for ( int i = 0; i < m_cameras.size(); ++i )
						if ( camera == m_cameras[i] )
							return i;
					return -1;
				}

				int CameraManager::CoreGetIndex( CameraData* camera )
				{
					for ( int i = 0; i < m_cameras.size(); ++i )
						if ( camera == m_cameras[i] )
							return i;
					return -1;
				}

				void CameraManager::CoreRemoveCamera( const int index )
				{
					if ( index < 0 || index >= m_cameras.size() )
						return;
					m_cameras.erase( m_cameras.begin() + index );
				}

				void CameraManager::CoreRemoveCamera( CameraData* camera )
				{
					if ( m_cameras.size() == 0 )
						return;
					auto it = m_cameras.begin();
					for ( ; it != m_cameras.end(); ++it )
						if ( *it == camera )
						{
							m_cameras.erase( it );
							return;
						}
				}

				void CameraManager::CoreUnregisterAllCameras()
				{
					m_cameras.clear();
				}

				size_t CameraManager::CoreCamerasCount() const
				{
					return m_cameras.size();
				}

				void CameraManager::CoreRegisterCamera( CameraData* camera )
				{
					m_cameras.push_back( camera );
					CoreSort();
				}

				void CameraManager::CoreSetCameraCountOnScreen( const int count )
				{
					m_countCamerasOnScreen = count > 1 ? count : 1;
				}

				size_t CameraManager::CoreGetCameraCountOnScreen() const
				{
					return m_countCamerasOnScreen < m_cameras.size() ? m_countCamerasOnScreen : m_cameras.size();;
				}

				void CameraManager::CoreSwitchCameras()
				{
					if ( m_cameras.size() == 0 )
						return;
					m_currentCamera = ( m_currentCamera % m_cameras.size() );
				}

				void CameraManager::CoreSort()
				{
					sort( m_cameras.begin(), m_cameras.end(), comp_CameraData() );
				}

				void CameraManager::CoreSetMSAASettings( bool enabled, int count )
				{
					bool modified = false;
					if ( m_MSAAEnabled != enabled )
					{
						m_MSAAEnabled = enabled;
						modified = true;
					}
					if ( m_MSAASamples != count )
					{
						m_MSAASamples = count;
						modified = true;
					}

					if ( modified )
					{
						for ( CameraData* data : m_cameras )
							data->ForceBufferCreation();
					}
				}

				void CameraManager::CoreResizeViewport( const int width, const int height )
				{
					size_t count = CoreCamerasCount();
					for ( int i = 0; i < count; ++i )
					{
						m_cameras[i]->Resize();
					}
				}
			}
		}
	}
}
