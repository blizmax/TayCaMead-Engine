#pragma once
#include  "TCMRenderer/RHI/Generic/RenderData.h"

#define INT(a) static_cast<int>(a)

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace PostProcess
			{
				struct PostProcess abstract
				{
					PostProcess();
					virtual ~PostProcess();

					AbstractTexture* RenderEffect( AbstractTexture* texture, FrameBuffer* buffer );

					virtual void Resize( const int width, const int height )
					{
					};

					std::string GetName() const;
					PostProcessEffect GetType() const;

				protected:
					virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer );
					virtual void Init();
					AbstractShader* m_shader = nullptr;

					std::string m_name;
					std::string vertexShaderStr;
					std::string fragmentShaderStr;

					PostProcessEffect m_type;
				};

				struct BloomEffect : PostProcess
				{
					BloomEffect();
					~BloomEffect();
					static void SetWeight( const float weight );
					static void SetPrecision( const int precision );
					static float GetWeight();
					static int GetPrecision();
					virtual void Resize( const int width, const int height ) override;

				protected:
					virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override = 0;
					virtual void Init() override;

					AbstractShader* m_bloomShader;;
					FrameBuffer* m_bloomBuffer;
					FrameBuffer* m_containerBuffer;
					std::vector<FrameBuffer*> m_containerBuffers;

					std::string verBloomShaderStr;
					std::string fragBloomShaderStr;

					float weight = 0.7f;
					int count = 4;
				};

				struct ToneMapping : PostProcess
				{
					ToneMapping();

					static void SetExposure( const float exposure );
					static float GetExposure();

					virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override = 0;

					float exposure;
				};

				struct GrayScale : PostProcess
				{
					GrayScale();
				};

				struct Inverted : PostProcess
				{
					Inverted();
				};

				struct KernelEffect : PostProcess
				{
					KernelEffect();
				};

				struct Blur : PostProcess
				{
					Blur();
				};

				struct GaussianBlur5 : PostProcess
				{
					GaussianBlur5();
					~GaussianBlur5();

					static void SetPassCount( const int count );
					static int GetPassCount();

					virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override = 0;
					virtual void Init() override;
					virtual void Resize( const int width, const int height ) override;

					FrameBuffer* m_pingPongBuffers[2];

					int nbrPass = 6;
				};

				struct Edge : PostProcess
				{
					Edge();
				};

				class PostProcessManager
				{
				public:
					virtual ~PostProcessManager();
					static PostProcess* GetPostProcess( PostProcessEffect postProcess );
					static std::string GetDefaultVertexShader();
					static void ResizePostProcesses( const int width, const int height );

				private:
					static PostProcessManager& GetInstance();

				protected:
					PostProcessManager();
					virtual PostProcess* CoreGetPostProcess( PostProcessEffect postProcess ) = 0;
					virtual std::string CoreGetDefaultVertexShader() = 0;
					void CoreResizePostProcesses( const int width, const int height );
					std::vector<PostProcess *> m_postProcesses;
				};
			}
		}
	}
}
