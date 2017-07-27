#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMRenderer/PostProcess/PostProcess.h"
#include "TCMLogger/Logger.h"

#include <memory>
#include <mutex>

#if defined(TCM_OPENGL)
#include "TCMRenderer/RHI/OpenGL/RendererOpenGL.h"
#include "TCMRenderer/RHI/OpenGL/RenderDataOpenGL.h"
typedef TCM::Engine::Graphics::OpenGL::RendererOpenGL TCMRenderer;
typedef TCM::Engine::Graphics::OpenGL::RenderDataComparerOpenGL CoreRenderDataComparer;
#elif defined(TCM_DIRECTX)
	#error "DirectX not implemented"
	typedef TCMRendererDirectX	TCMRenderer;
#else
	#error "No define for TCMRenderer"
#endif
namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
#pragma  region Constructor and co.
			AbstractRenderer& AbstractRenderer::GetInstance()
			{
				static std::unique_ptr<AbstractRenderer> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMRenderer() );
				                }
				);
				return *( m_instance.get() );
			}

			AbstractRenderer::~AbstractRenderer()
			{
				if ( m_pingPongBuffers[0] )
				{
					delete m_pingPongBuffers[0];
					m_pingPongBuffers[0] = nullptr;
				}

				if ( m_pingPongBuffers[1] )
				{
					delete m_pingPongBuffers[1];
					m_pingPongBuffers[1] = nullptr;
				}

				if ( m_skyboxShader )
				{
					delete m_skyboxShader;
					m_skyboxShader = nullptr;
				}
			}

#pragma endregion

#pragma region Static Functions

			void AbstractRenderer::Init( const int width, const int height )
			{
				try
				{
					GetInstance().CoreInitializeRenderer( width, height );
					GetInstance().CoreInitSkybox();
				}
				catch ( const std::exception& e )
				{
					TCMFATALERROR("Exception received: " + std::string(e.what()));
				}
			}

			void AbstractRenderer::Shutdown()
			{
				GetInstance().CoreShutdown();
			}

			void AbstractRenderer::Render( Camera::CameraData* camera, const std::vector<const RenderData*>& queueRenderData,
			                               const std::vector<LightData*>& influentLights )
			{
				GetInstance().CoreRender( camera, queueRenderData, influentLights );
			}

			void AbstractRenderer::RenderGizmoLines( Camera::CameraData* camera, const std::vector<float>& gizmoLines, const int gizmoLinesCount )
			{
				GetInstance().CoreRenderGizmoLines( camera, gizmoLines, gizmoLinesCount );
			}

			void AbstractRenderer::RenderGizmoPoints( Camera::CameraData* camera, const std::vector<float>& gizmoPoints, const int gizmoPointsCount )
			{
				GetInstance().CoreRenderGizmoPoints( camera, gizmoPoints, gizmoPointsCount );
			}

			void AbstractRenderer::RenderGizmoMesh( Camera::CameraData* camera, const Debug::GizmoMesh& gizmoMesh )
			{
				GetInstance().CoreRenderGizmoMesh( camera, gizmoMesh );
			}

			void AbstractRenderer::PreRender()
			{
				GetInstance().CorePreRender();
			}

			void AbstractRenderer::PostRender()
			{
				GetInstance().CorePostRender();
			}

			void AbstractRenderer::RenderScreen( AbstractTexture* tex, AbstractShader* shader )
			{
				GetInstance().CoreRenderScreen( tex, shader );
			}

			void AbstractRenderer::RenderSprites( Camera::CameraData* camera, std::vector<std::pair<const AbstractTexture*, Math::Mat4>> spriteList, bool isInCanvas )
			{
				GetInstance().CoreRenderSprites( camera, spriteList, isInCanvas );
			}

			// * Creates a RenderData of the type needed for the actual API
			RenderData* AbstractRenderer::CreateRenderData()
			{
				return new RenderData();
			}

			AbstractMesh* AbstractRenderer::CreateMesh(
				const uint32_t& vertexNum, const float* vertexBuff,
				const uint32_t& indicesNum, const uint16_t* indicesBuff )
			{
				return GetInstance().CoreCreateMesh( vertexNum, vertexBuff, indicesNum, indicesBuff );
			}

			AbstractTexture* AbstractRenderer::CreateAbstractTexture()
			{
				return GetInstance().CoreCreateAbstractTexture();
			}

			AbstractTexture* AbstractRenderer::CreateTexture( const int type, const int width, const int height, const uint8_t* texData )
			{
				return GetInstance().CoreCreateTexture( type, width, height, texData );
			}

			LightData* AbstractRenderer::CreateLight()
			{
				return GetInstance().CoreCreateLight();
			}

			AbstractMaterial* AbstractRenderer::CreateMaterial()
			{
				return GetInstance().CoreCreateMaterial();
			}

			FrameBuffer* AbstractRenderer::CreateFrameBuffer()
			{
				return GetInstance().CoreCreateFrameBuffer();
			}

			CameraSpecificData* AbstractRenderer::CreateCameraSpecificData()
			{
				return GetInstance().CoreCreateCameraSpecificData();
			}

			void AbstractRenderer::UpdateCameraData( Camera::CameraData* camera )
			{
				GetInstance().CoreUpdateCameraData( camera );
			}

			FrameBuffer* AbstractRenderer::CreateRendererFrameBuffer( const Math::Vec2& size, FrameBufferType type, const int samplesCount )
			{
				return GetInstance().CoreCreateRendererFrameBuffer( size, type, samplesCount );
			}

			// * Create shader from files
			AbstractShader* AbstractRenderer::CreateShader( const char* vertexShader, const char* fragmentShader, const char* geometryShader, const bool deferred )
			{
				return GetInstance().CoreCreateShader( vertexShader, fragmentShader, geometryShader, deferred );
			}

			AbstractSkybox* AbstractRenderer::CreateSkybox( std::string skyfaces[6] )
			{
				return GetInstance().CoreCreateSkybox( skyfaces );
			}

			void AbstractRenderer::SetShadowParams( const int width, const int height, const float& near_plane, const float& far_plane )
			{
				GetInstance().m_shadowMapSizeX = width;
				GetInstance().m_shadowMapSizeY = height;
				GetInstance().m_nearPlane = near_plane;
				GetInstance().m_farPlane = far_plane;
			}

			void AbstractRenderer::GetShadowParams( int& width, int& height, float& near_plane, float& far_plane )
			{
				width = GetInstance().m_shadowMapSizeX;
				height = GetInstance().m_shadowMapSizeY;
				near_plane = GetInstance().m_nearPlane;
				far_plane = GetInstance().m_farPlane;
			}

			void AbstractRenderer::DrawSkybox( Camera::CameraData* camera, const AbstractSkybox* skybox )
			{
				GetInstance().CoreDrawSkybox( camera, skybox );
			}

			void AbstractRenderer::GetScreenSize( int& width, int& height )
			{
				GetInstance().CoreGetScreenSize( width, height );
			}

			Math::Mat4 AbstractRenderer::GenProjMatrix( const Math::Vec2& ratio, const float fov, const float near, const float far )
			{
				return GetInstance().CoreGenProjMatrix( ratio, fov, near, far );
			}

			Math::Mat4 AbstractRenderer::GenOrthoMatrix( const Math::Vec2& ratio, const float near, const float far )
			{
				return GetInstance().CoreGenOrthoMatrix( ratio, near, far );
			}

			Math::Mat4 AbstractRenderer::GenOrthoMatrix( const Math::Vec2& ratio )
			{
				return GetInstance().CoreGenOrthoMatrix( ratio );
			}

			void AbstractRenderer::PasteBufferOnScreen( const FrameBuffer* frameBuffer, const Math::Vec2& position, const Math::Vec2& size )
			{
				GetInstance().CorePasteBufferOnScreen( frameBuffer, position, size );
			}

			void AbstractRenderer::PostProcess( Camera::CameraData* camera )
			{
				GetInstance().CorePostProcess( camera );
			}

			void AbstractRenderer::Clear()
			{
				GetInstance().CoreClear();
			}

			RenderDataComparer AbstractRenderer::GetComparer()
			{
				return GetInstance().CoreGetComparer();
			}

			void AbstractRenderer::ResizeRenderer( const int width, const int height )
			{
				GetInstance().m_width = width;
				GetInstance().m_height = height;
				GetInstance().CoreResizeRenderer( width, height );
			}

#pragma endregion

#pragma region Core Function

			void AbstractRenderer::GenericPostProcess( FrameBuffer* from, std::vector<PostProcess::PostProcess *>* postProcesses )
			{
				if ( from == nullptr )
				TCMFATALERROR("Framebuffer not init");
				if ( m_pingPongBuffers[0] == nullptr )
				TCMFATALERROR("m_pingPongBuffers[0] not init");
				if ( m_pingPongBuffers[1] == nullptr )
				TCMFATALERROR("m_pingPongBuffers[1] not init");

				bool even = true;

				AbstractTexture* tex = from->GetTexture();

				std::vector<PostProcess::PostProcess *> vecPostProcesses = *postProcesses;

				size_t size = vecPostProcesses.size();
				for ( int i = 0; i < size; i++ )
				{
					tex = vecPostProcesses[i]->RenderEffect( tex, m_pingPongBuffers[even] );
					even = !even;
				}

				if ( size > 0 )
				{
					//Works only if size > 0
					from->BindBuffer();
					from->ClearBuffer();
					CoreRenderScreen( tex );
				}
			}

			void AbstractRenderer::ResizeViewPort( const int width, const int height )
			{
				if ( m_viewportSizeX != width || m_viewportSizeY != height )
				{
					m_viewportSizeX = width;
					m_viewportSizeY = height;
					CoreResizeViewPort( width, height );
				}
			}

			void AbstractRenderer::CoreBindShader( s_AbstractShader* shader )
			{
				//if ( shader != m_lastShader )
				{
					shader->Bind();
					m_lastShader = shader;
				}
			}

			void AbstractRenderer::CoreDrawRenderData( Camera::CameraData* camera, const RenderData* renderData, AbstractShader* forcedShader )
			{
				CoreDrawRenderData( camera, renderData, nullptr, forcedShader );
			}

			void AbstractRenderer::CoreRenderSprites( Camera::CameraData* camera, std::vector<std::pair<const AbstractTexture*, Math::Mat4>> spriteList, bool isInCanvas )
			{
				for ( auto sprite : spriteList )
					CoreRenderSprite( camera, sprite, isInCanvas );
			}

			void AbstractRenderer::CoreGetScreenSize( int& width, int& height ) const
			{
				width = m_width;
				height = m_height;
			}

			Math::Mat4 AbstractRenderer::CoreGenProjMatrix( const Math::Vec2& ratio, const float fov, const float near, const float far ) const
			{
				return Math::Mat4::ProjMatrix( m_width * ratio.x,
				                               m_height * ratio.y,
				                               fov,
				                               near,
				                               far );
			}

			Math::Mat4 AbstractRenderer::CoreGenOrthoMatrix( const Math::Vec2& ratio, const float near, const float far ) const
			{
				return Math::Mat4::OrthoProj( 0, m_width * ratio.x,
				                              m_height * ratio.y, 0, near, far );
			}

			Math::Mat4 AbstractRenderer::CoreGenOrthoMatrix( const Math::Vec2& ratio ) const
			{
				return Math::Mat4::OrthoProj( int( m_width * ratio.x ),
				                              int( m_height * ratio.y ) );
			}

			RenderDataComparer AbstractRenderer::CoreGetComparer() const
			{
				return static_cast<RenderDataComparer>(CoreRenderDataComparer());
			}
#pragma endregion
		}
	}
}
