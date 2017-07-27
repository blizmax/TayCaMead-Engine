#include "TCMRenderer/PostProcess/PostProcess.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include <memory>
#include <mutex>
#include "TCMLogger/Logger.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

#if defined(TCM_OPENGL)
#include "TCMRenderer/PostProcess/OpenGL/PostProcessOpenGL.h"
typedef TCM::Engine::Graphics::PostProcess::OpenGL::PostProcessManagerGL TCMCorePostProcessManager;
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
			namespace PostProcess
			{
				void PostProcess::Init()
				{
					m_shader = RendererFactory::CreateShader( vertexShaderStr.c_str(), fragmentShaderStr.c_str() );
				}

				PostProcess::PostProcess(): m_type()
				{
					vertexShaderStr = PostProcessManager::GetDefaultVertexShader();
				}

				PostProcess::~PostProcess()
				{
					delete m_shader;
				}

				AbstractTexture* PostProcess::RenderEffect( AbstractTexture* texture, FrameBuffer* buffer )
				{
					return CoreRenderEffect( texture, buffer );
				}

				std::string PostProcess::GetName() const
				{
					return m_name;
				}

				PostProcessEffect PostProcess::GetType() const
				{
					return m_type;
				}

				AbstractTexture* PostProcess::CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer )
				{
					if ( m_shader == nullptr )
						Init();
					if ( m_shader == nullptr )
					{
						TCMFAILURE("Shader not init");
						return nullptr;
					}

					buffer->BindBuffer();
					buffer->ClearBuffer();

					AbstractRenderer::RenderScreen( texture, m_shader );

					return buffer->GetTexture();
				}

				BloomEffect::BloomEffect() : m_bloomShader( nullptr )
				                           , m_bloomBuffer( nullptr )
				                           , m_containerBuffer( nullptr )
				{
					m_name = "Bloom Effect";
					m_type = PostProcessEffect::BLOOM_EFFECT;
				}

				BloomEffect::~BloomEffect()
				{
					delete m_bloomBuffer;
					delete m_containerBuffer;
					delete m_bloomShader;
					for ( int i = 0; i < count; i++ )
					{
						delete m_containerBuffers[i];
					}
				}

				void BloomEffect::SetWeight( const float weight )
				{
					BloomEffect* bloom = dynamic_cast<BloomEffect *>(PostProcessManager::GetPostProcess( PostProcessEffect::BLOOM_EFFECT ));
					bloom->weight = weight;
				}

				void BloomEffect::SetPrecision( const int precision )
				{
					BloomEffect* bloom = dynamic_cast<BloomEffect *>(PostProcessManager::GetPostProcess( PostProcessEffect::BLOOM_EFFECT ));
					bloom->count = precision;
				}

				float BloomEffect::GetWeight()
				{
					BloomEffect* bloom = dynamic_cast<BloomEffect *>(PostProcessManager::GetPostProcess( PostProcessEffect::BLOOM_EFFECT ));
					return bloom->weight;
				}

				int BloomEffect::GetPrecision()
				{
					BloomEffect* bloom = dynamic_cast<BloomEffect *>(PostProcessManager::GetPostProcess( PostProcessEffect::BLOOM_EFFECT ));
					return bloom->count;
				}

				void BloomEffect::Init()
				{
					m_bloomBuffer = RendererFactory::CreateFramebuffer();

					m_containerBuffer = RendererFactory::CreateFramebuffer();

					int width, height;
					AbstractRenderer::GetScreenSize( width, height );

					m_bloomBuffer->GenTexture( TextureType::Color0, width, height );
					m_containerBuffer->GenTexture( TextureType::Color0, width, height );

					for ( int i = 0; i < count; ++i )
					{
						double power = pow( 2, i + 1 );

						FrameBuffer* framebuffer = RendererFactory::CreateFramebuffer();

						int tWidth = static_cast<int>(round( width / power ));
						int tHeight = static_cast<int>(round( height / power ));

						framebuffer->GenTexture( TextureType::Color0, tWidth, tHeight );

						m_containerBuffers.push_back( framebuffer );
					}

					m_shader = RendererFactory::CreateShader( vertexShaderStr.c_str(), fragmentShaderStr.c_str() );
					m_bloomShader = RendererFactory::CreateShader( verBloomShaderStr.c_str(), fragBloomShaderStr.c_str() );
				}

				void BloomEffect::Resize( const int width, const int height )
				{
					m_bloomBuffer->ResizeTexture( TextureType::Color0, width, height );
					m_containerBuffer->ResizeTexture( TextureType::Color0, width, height );

					for ( int i = 0; i < count; ++i )
					{
						double power = pow( 2, i + 1 );

						FrameBuffer* framebuffer = m_containerBuffers[i];

						int tWidth = static_cast<int>(round( width / power ));
						int tHeight = static_cast<int>(round( height / power ));

						framebuffer->ResizeTexture( TextureType::Color0, tWidth, tHeight );
					}
				}

				ToneMapping::ToneMapping() :
				                           exposure( 1 )
				{
					m_name = "Tone Mapping";
					m_type = PostProcessEffect::TONE_MAPPING;
				}

				void ToneMapping::SetExposure( const float exposure )
				{
					ToneMapping* bloom = dynamic_cast<ToneMapping *>(PostProcessManager::GetPostProcess( PostProcessEffect::TONE_MAPPING ));
					bloom->exposure = exposure;
				}

				float ToneMapping::GetExposure()
				{
					ToneMapping* bloom = dynamic_cast<ToneMapping *>(PostProcessManager::GetPostProcess( PostProcessEffect::TONE_MAPPING ));
					return bloom->exposure;
				}

				GrayScale::GrayScale()
				{
					m_name = "Gray Scale";
					m_type = PostProcessEffect::GRAYSCALE;
				}

				Inverted::Inverted()
				{
					m_name = "Inverted";
					m_type = PostProcessEffect::INVERTED;
				}

				KernelEffect::KernelEffect()
				{
					m_name = "Kernel Effect";
					m_type = PostProcessEffect::KERNEL_EFFECT;
				}

				Blur::Blur()
				{
					m_name = "Blur Effect";
					m_type = PostProcessEffect::BLUR;
				}

				GaussianBlur5::GaussianBlur5()
				{
					m_name = "Gaussian Blur Effect";
					m_type = PostProcessEffect::GAUSS_BLUR;
				}

				GaussianBlur5::~GaussianBlur5()
				{
					delete m_pingPongBuffers[0];
					delete m_pingPongBuffers[1];
				}

				void GaussianBlur5::SetPassCount( const int count )
				{
					GaussianBlur5* gaussian = dynamic_cast<GaussianBlur5 *>(PostProcessManager::GetPostProcess( PostProcessEffect::GAUSS_BLUR ));
					gaussian->nbrPass = count * 2;
				}

				int GaussianBlur5::GetPassCount()
				{
					GaussianBlur5* gaussian = dynamic_cast<GaussianBlur5 *>(PostProcessManager::GetPostProcess( PostProcessEffect::GAUSS_BLUR ));
					return gaussian->nbrPass / 2;
				}

				void GaussianBlur5::Init()
				{
					m_pingPongBuffers[0] = RendererFactory::CreateFramebuffer();

					m_pingPongBuffers[1] = RendererFactory::CreateFramebuffer();

					int width, height;
					AbstractRenderer::GetScreenSize( width, height );

					m_pingPongBuffers[0]->GenTexture( TextureType::Color0, width, height );
					m_pingPongBuffers[1]->GenTexture( TextureType::Color0, width, height );

					m_shader = RendererFactory::CreateShader( vertexShaderStr.c_str(), fragmentShaderStr.c_str() );
				}

				void GaussianBlur5::Resize( const int width, const int height )
				{
					m_pingPongBuffers[0]->ResizeTexture( TextureType::Color0, width, height );
					m_pingPongBuffers[1]->ResizeTexture( TextureType::Color0, width, height );
				}

				Edge::Edge()
				{
					m_name = "Edge Effect";
					m_type = PostProcessEffect::EDGE;
				}

				PostProcessManager::~PostProcessManager()
				{
					for ( int i = 0; i < m_postProcesses.size(); ++i )
						if ( m_postProcesses[i] != nullptr )
							delete m_postProcesses[i];
				}

				PostProcess* PostProcessManager::GetPostProcess( PostProcessEffect postProcess )
				{
					return GetInstance().CoreGetPostProcess( postProcess );
				}

				std::string PostProcessManager::GetDefaultVertexShader()
				{
					return GetInstance().CoreGetDefaultVertexShader();
				}

				void PostProcessManager::ResizePostProcesses( const int width, const int height )
				{
					GetInstance().CoreResizePostProcesses( width, height );
				}

				PostProcessManager& PostProcessManager::GetInstance()
				{
					static std::unique_ptr<PostProcessManager> m_instance;
					static std::once_flag m_onceFlag;

					std::call_once( m_onceFlag,
					                []()
					                {
						                m_instance.reset( new TCMCorePostProcessManager() );
					                }
					);
					return *( m_instance.get() );
				}

				PostProcessManager::PostProcessManager()
				{
					m_postProcesses.resize( INT(PostProcessEffect::COUNT) );
				}

				void PostProcessManager::CoreResizePostProcesses( const int width, const int height )
				{
					for ( int i = 0; i < INT(PostProcessEffect::COUNT); ++i )
					{
						if ( m_postProcesses[i] == nullptr )
							continue;
						m_postProcesses[i]->Resize( width, height );
					}
				}
			}
		}
	}
}
