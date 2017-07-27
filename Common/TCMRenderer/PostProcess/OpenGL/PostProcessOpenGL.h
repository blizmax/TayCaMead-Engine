#pragma once
#include "TCMRenderer/PostProcess/PostProcess.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace PostProcess
			{
				namespace OpenGL
				{
					struct BloomEffectGL : BloomEffect
					{
						BloomEffectGL();
						virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override;

					private:
						const int weightLocation = 14;
					};

					struct ToneMappingGL : ToneMapping
					{
						ToneMappingGL();
						virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override;

					private:
						const int exposureLocation = 14;
					};

					struct GrayScaleGL : GrayScale
					{
						GrayScaleGL();
					};

					struct InvertedGL : Inverted
					{
						InvertedGL();
					};

					struct KernelEffectGL : KernelEffect
					{
						KernelEffectGL();
					};

					struct BlurGL : Blur
					{
						BlurGL();
					};

					struct GaussianBlur5GL : GaussianBlur5
					{
						GaussianBlur5GL();
						virtual AbstractTexture* CoreRenderEffect( AbstractTexture* texture, FrameBuffer* buffer ) override;
					};

					struct EdgeGL : Edge
					{
						EdgeGL();
					};

					class PostProcessManagerGL : public PostProcessManager
					{
					public:
						PostProcessManagerGL() = default;
						virtual PostProcess* CoreGetPostProcess( PostProcessEffect postProcess ) override;
						virtual std::string CoreGetDefaultVertexShader() override;
					};
				}
			}
		}
	}
}
