#include "SkyboxEditor.h"
#include "MainWindow.h"
#include "EngineClasses/TCMScene.h"

namespace TCM
{
	namespace Editor
	{
		SkyboxEditor::SkyboxEditor()
			: m_scene( MainWindow::GetScene() )
		{
			const std::vector<std::string>* basicStrings = m_scene->GetSkyfaces();
			for ( int i = 0; i < 6; ++i )
				skyfaces[i] = basicStrings->at( i );
		}

		void SkyboxEditor::SetSkyface( int index, const char* filename )
		{
			skyfaces[index] = filename;
		}

		void SkyboxEditor::Apply()
		{
			m_scene->SetSkyfaces( skyfaces );
		}

		void SkyboxEditor::ShowSkyfaces()
		{
			for ( int i = 0; i < 6; ++i )
				ShowSkyface( i );
		}
	}
}
