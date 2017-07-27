#pragma once
#include "EditorComponent.h"
#include <string>

namespace TCM
{
	namespace Editor
	{
		class SkyboxEditor abstract
		{
		public:
			SkyboxEditor();
			virtual ~SkyboxEditor() = default;
			void SetSkyface( int index, const char* filename );
			void Apply();

			void ShowSkyfaces();
			virtual void ShowSkyface( int index ) = 0;

			std::string skyfaces[6];
			Engine::TCMScene* m_scene;
		};
	}
}
