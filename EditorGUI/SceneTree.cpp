#include "SceneTree.h"

#include "EditorDefines.h"
#include "EngineClasses/TCMScene.h"
#include "MainWindow.h"

namespace TCM
{
	namespace Editor
	{
		SceneTree::SceneTree()
		{
			MainWindow::GetPlayEvent().Push( &SceneTree::DisableUI, this );
			MainWindow::GetStopEvent().Push( &SceneTree::EnableUI, this );
			Engine::TCMSceneObject::GetOnRelationChange().Push( &SceneTree::RefreshUI, this );
		}

		SceneTree::~SceneTree()
		{
			MainWindow::GetPlayEvent().Pop( &SceneTree::DisableUI, this );
			MainWindow::GetStopEvent().Pop( &SceneTree::EnableUI, this );
			Engine::TCMSceneObject::GetOnRelationChange().Pop( &SceneTree::RefreshUI, this );
		}

		void SceneTree::Refresh( Engine::Project* project, Engine::TCMScene* scene )
		{
			SetScene( scene );
			SetSelectedSceneObject( nullptr );
		}

		std::vector<Engine::TCMSceneObject*> SceneTree::FindSceneObjects( const std::string& name ) const
		{
			return GetScene()->GetRootSceneObject()->GetChildren( name );
		}

		void SceneTree::SetSelectedSceneObject( Engine::TCMSceneObject* object )
		{
			MainWindow::SelectSceneObject( object );
		}
	}
}
