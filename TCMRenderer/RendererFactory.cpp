#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			RenderData* RendererFactory::CreateRenderData()
			{
				return AbstractRenderer::CreateRenderData();
			}

			AbstractMesh* RendererFactory::CreateMesh(
				const uint32_t& vertexNum, const float* vertexBuff,
				const uint32_t& indicesNum, const uint16_t* indicesBuff )
			{
				return AbstractRenderer::CreateMesh( vertexNum, vertexBuff, indicesNum, indicesBuff );
			}

			AbstractMesh* RendererFactory::CreateMesh( std::vector<Graphics::VertexData*>& vecFragmentVertex, std::vector<Graphics::Fragment*>& vecFragment )
			{
				uint16_t vertexNum = static_cast<uint16_t>(vecFragmentVertex.size());
				uint32_t vertexSize = vertexNum * 11; // X, Y, Z, Nx, Ny, Nz, U, V -> 8 values
				float* vertexBuff = new float[vertexSize];
				for ( size_t i = 0; i < vertexNum; i++ )
					memcpy( &( vertexBuff[i * 11] ), vecFragmentVertex.at( i ), sizeof( float) * 11 );

				uint16_t indicesNum = static_cast<uint16_t>(vecFragment.size());
				uint32_t indicesSize = indicesNum * 3; // v1, v2, v3 -> 3 values
				uint16_t* indicesBuff = new uint16_t[indicesSize];
				for ( size_t i = 0; i < indicesNum; i++ )
					memcpy( &( indicesBuff[i * 3] ), vecFragment.at( i ), sizeof( uint16_t) * 3 );

				AbstractMesh* abstractMesh = AbstractRenderer::CreateMesh( vertexSize, vertexBuff, indicesSize, indicesBuff );

				for ( int i = 0; i < vecFragmentVertex.size(); ++i )
					delete vecFragmentVertex[i];
				vecFragmentVertex.clear();
				for ( int i = 0; i < vecFragment.size(); ++i )
					delete vecFragment[i];
				vecFragment.clear();

				delete[] vertexBuff;
				delete[] indicesBuff;

				return abstractMesh;
			}

			AbstractTexture* RendererFactory::CreateAbstractTexture()
			{
				return AbstractRenderer::CreateAbstractTexture();
			}

			AbstractTexture* RendererFactory::CreateTexture( const int type, const int width, const int height, const uint8_t* texData )
			{
				return AbstractRenderer::CreateTexture( type, width, height, texData );
			}

			LightData* RendererFactory::CreateLight()
			{
				return AbstractRenderer::CreateLight();
			}

			void RendererFactory::SetShadowParams( const int width, const int height, const float& near_plane, const float& far_plane )
			{
				AbstractRenderer::SetShadowParams( width, height, near_plane, far_plane );
			}

			void RendererFactory::GetShadowParams( int& width, int& height, float& near_plane, float& far_plane )
			{
				AbstractRenderer::GetShadowParams( width, height, near_plane, far_plane );
			}

			AbstractMaterial* RendererFactory::CreateMaterial()
			{
				return AbstractRenderer::CreateMaterial();
			}

			AbstractShader* RendererFactory::CreateShader( const char* vertexShader, const char* fragmentShader, const char* geometryShader, const bool deferred )
			{
				return AbstractRenderer::CreateShader( vertexShader, fragmentShader, geometryShader, deferred );
			}

			AbstractSkybox* RendererFactory::CreateSkybox( std::string skyfaces[6] )
			{
				bool isValid = true;
				for ( int i = 0; i < 6; ++i )
				{
					if ( skyfaces[i] == "" )
					{
						isValid = false;
						break;
					}
				}
				if ( isValid )
					return AbstractRenderer::CreateSkybox( skyfaces );
				return nullptr;
			}

			FrameBuffer* RendererFactory::CreateFramebuffer()
			{
				return AbstractRenderer::CreateFrameBuffer();
			}

			CameraSpecificData* RendererFactory::CreateCameraSpecificData()
			{
				return AbstractRenderer::CreateCameraSpecificData();
			}

			void RendererFactory::UpdateCameraData( Camera::CameraData* camera )
			{
				AbstractRenderer::UpdateCameraData( camera );
			}

			FrameBuffer* RendererFactory::CreateRendererFramebuffer( const Math::Vec2& size, FrameBufferType type, const int samplesCount )
			{
				return AbstractRenderer::CreateRendererFrameBuffer( size, type, samplesCount );
			}

			Math::Mat4 RendererFactory::GenProjMatrix( const Math::Vec2& ratio, float fov, float near, float far )
			{
				return AbstractRenderer::GenProjMatrix( ratio, fov, near, far );
			}

			Math::Mat4 RendererFactory::GenOrthoMatrix( const Math::Vec2& ratio )
			{
				return AbstractRenderer::GenOrthoMatrix( ratio );
			}

			Math::Mat4 RendererFactory::GenOrthoMatrix( const Math::Vec2& ratio, float near, float far )
			{
				return AbstractRenderer::GenOrthoMatrix( ratio, near, far );
			}

			void RendererFactory::Init( const int width, const int height )
			{
				AbstractRenderer::Init( width, height );
			}

			void RendererFactory::Resize( const int width, const int height )
			{
				Camera::CameraManager::ResizeViewport( width, height );
				AbstractRenderer::ResizeRenderer( width, height );
				PostProcess::PostProcessManager::ResizePostProcesses( width, height );
			}
		}
	}
}
