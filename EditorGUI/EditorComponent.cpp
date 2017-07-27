#include "EditorComponent.h"
#include "MainWindow.h"

namespace TCM
{
	namespace Editor
	{
		EditorComponent::~EditorComponent()
		{
			MainWindow::Unregister( this );
		}

		void EditorComponent::Register()
		{
			MainWindow::Register( this );
		}

		Engine::TCMScene* EditorComponent::GetScene() const
		{
			return MainWindow::GetScene();
		}

		Engine::TCMSceneObject* EditorComponent::GetSceneObject() const
		{
			return MainWindow::GetSceneObject();
		}

		Engine::Project* EditorComponent::GetProject() const
		{
			return MainWindow::GetProject();
		}
	}
}
