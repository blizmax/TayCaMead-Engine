#pragma once
#include "MathLib/ColorRGBA.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Debug
	{
		struct GizmoMesh
		{
			GizmoMesh( Engine::Graphics::AbstractMesh* mesh,
			           const Engine::Math::Vec3& position = { 0.0f, 0.0f, 0.0f },
			           const Engine::Math::Vec3& rotation = { 0.0f, 0.0f, 0.0f },
			           const Engine::Math::Vec3& scale = { 1.0f, 1.0f, 1.0f },
			           Engine::Math::ColorRGBA color = { 1.0f, 1.0f, 1.0f },
			           bool isWired = true );

			Engine::Graphics::AbstractMesh* GetMesh() const;
			Engine::Math::Mat4 GetMatrix() const;
			Engine::Math::ColorRGBA GetColor() const;
			bool GetIsWired() const;
		protected:
			Engine::Graphics::AbstractMesh* m_mesh;
			Engine::Math::Mat4 m_matrix;
			Engine::Math::ColorRGBA m_color;
			bool m_isWired;
		};
	}
}
