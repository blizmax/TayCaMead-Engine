#pragma once
#include "MathLib/Mat4.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct ViewProjectionMatrix
			{
				uint32_t size = sizeof(Math::Mat4) * 2;
				Math::Mat4 mvpMatrix;
				Math::Mat4 viewMatrix;
				Math::Mat4 projectionMatrix;

				void* GetPointer()
				{
					return &viewMatrix;
				}
			};
		}
	}
}
