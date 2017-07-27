#include "MainWindow.h"
#include "EditorComponent.h"
#include "EditorDefines.h"
#include "EngineClasses/TCMScene.h"
#include "EngineClasses/Project.h"
#include "TCMCore/Core.h"
#include "EditorCommandManager.h"

#include "TCMSaveManager/SaveData.h"
#include "TCMSaveManager/BinarySerialization.hpp"
#include "ObjectInsertion.h"
#include "TCMTime/Time.h"

namespace TCM
{
	namespace Editor
	{
		MainWindow* MainWindow::instance = nullptr;

		MainWindow::MainWindow()
		{
			TCM::Engine::Core::Initialize();
			//TCM::Engine::Core::SetExampleScene();
			MainWindow::instance = this;

			EditorCommandManager::GetOnAddCommandEvent().Push( &MainWindow::OnCommandChangeCallback, this );
			EditorCommandManager::GetOnUndoRedoEvent().Push( &MainWindow::OnCommandChangeCallback, this );

			GetPauseEvent().Push( &MainWindow::OnPauseEvent, this );
			GetPlayEvent().Push( &MainWindow::OnPlayEvent, this );
			GetStopEvent().Push( &MainWindow::OnStopEvent, this );
			GetNextFrameEvent().Push( &MainWindow::OnNextFrameEvent, this );

			TCM::Engine::Core::GetRenderCallback().Push( &MainWindow::OnDrawedEvent, this );

			CoreNewProject();
		}

		MainWindow::~MainWindow()
		{
			TCM::Engine::Core::Shutdown();
			if ( m_savedScene && m_scene )
			{
				m_scene->TCMDestroy();
				delete m_scene;
				m_scene = nullptr;
			}
		}

		std::string MainWindow::GetDocPath()
		{
			std::wstring text( SOLUTION_DIR );
			std::string result( text.begin(), text.end() );
			result += "Doxygen\\html\\index.html";

			return result;
		}

		void MainWindow::NewScene()
		{
			instance->CoreNewScene();
		}

		void MainWindow::NewProject()
		{
			instance->CoreNewProject();
		}

		void MainWindow::LoadScene( const std::string& filename )
		{
			instance->CoreLoadScene( filename );
		}

		void MainWindow::LoadProject( const std::string& filename )
		{
			instance->CoreLoadProject( filename );
		}

		bool MainWindow::SaveScene()
		{
			return instance->CoreSaveScene();
		}

		bool MainWindow::SaveProject()
		{
			return instance->CoreSaveProject();
		}

		void MainWindow::SaveScene( const std::string& filename )
		{
			instance->CoreSaveScene( filename );
		}

		void MainWindow::SaveProject( const std::string& filename )
		{
			instance->CoreSaveProject( filename );
		}

		std::string MainWindow::GetProjectFilepath()
		{
			return instance->projectFilePath;
		}

		std::string MainWindow::GetSceneFilepath()
		{
			return instance->sceneFilePath;
		}

		Engine::TCMScene* MainWindow::GetScene()
		{
			return instance->CoreGetScene();
		}

		Engine::TCMSceneObject* MainWindow::GetSceneObject()
		{
			return instance->CoreGetSceneObject();
		}

		Engine::Project* MainWindow::GetProject()
		{
			return instance->CoreGetProject();
		}

		void MainWindow::Register( EditorComponent* component )
		{
			instance->CoreRegister( component );
		}

		void MainWindow::Unregister( EditorComponent* component )
		{
			instance->CoreUnregister( component );
		}

		void MainWindow::SelectSceneObject( Engine::TCMSceneObject* object )
		{
			instance->CoreSelectSceneObject( object );
		}

		Engine::Event<>& MainWindow::GetUpdateCallback()
		{
			return TCM::Engine::Core::GetUpdateCallback();
		}

		Engine::Event<>& MainWindow::GetPauseEvent()
		{
			return TCM::Engine::Core::GetPauseEvent();
		}

		Engine::Event<>& MainWindow::GetPlayEvent()
		{
			return TCM::Engine::Core::GetPlayEvent();
		}

		Engine::Event<>& MainWindow::GetNextFrameEvent()
		{
			return TCM::Engine::Core::GetNextFrameEvent();
		}

		Engine::Event<>& MainWindow::GetStopEvent()
		{
			return TCM::Engine::Core::GetStopEvent();
		}

		std::string MainWindow::GetIconPath( IconType icon )
		{
			std::string color = instance->m_color;
			switch ( icon )
			{
				case IconType::ADD: return ":/" + color + "/add";
				case IconType::BUILD: return ":/" + color + "/build";
				case IconType::CANCEL: return ":/" + color + "/cancel";
				case IconType::COPY: return ":/" + color + "/copy";
				case IconType::PASTE: return ":/" + color + "/paste";
				case IconType::CUT: return ":/" + color + "/cut";
				case IconType::OPEN: return ":/" + color + "/open";
				case IconType::PAUSE: return ":/" + color + "/pause";
				case IconType::PLAY: return ":/" + color + "/play";
				case IconType::NEXT: return ":/" + color + "/next";
				case IconType::UNDO: return ":/" + color + "/undo";
				case IconType::REDO: return ":/" + color + "/redo";
				case IconType::SAVE: return ":/" + color + "/save";
				case IconType::SETTINGS: return ":/" + color + "/settings";
				case IconType::REMOVE: return ":/" + color + "/delete";
				case IconType::COUNT: return "";
				default: return "";
			}
		}

		void MainWindow::CoreNewScene()
		{
			sceneFilePath = "";
			m_scene = new Engine::TCMScene();

			ObjectInsertion insert( m_scene->GetRootSceneObject() );

			insert.CreateDirectionnalLight();
			insert.CreateCamera();

			std::string skyfaces[6];

			skyfaces[0] = "../Common/TCMRenderer/skybox/right.jpg";
			skyfaces[1] = "../Common/TCMRenderer/skybox/left.jpg";
			skyfaces[2] = "../Common/TCMRenderer/skybox/top.jpg";
			skyfaces[3] = "../Common/TCMRenderer/skybox/bottom.jpg";
			skyfaces[4] = "../Common/TCMRenderer/skybox/back.jpg";
			skyfaces[5] = "../Common/TCMRenderer/skybox/front.jpg";

			m_scene->SetSkyfaces( skyfaces );

			//m_scene = Engine::Core::GetExampleScene();
			Engine::Core::ChangeScene( m_scene );
			RefreshComponents();
		}

		void MainWindow::CoreNewProject()
		{
			projectFilePath = "";
			m_project = new Engine::Project();
			Engine::Core::SetProject( m_project );
			CoreNewScene();
		}

		bool MainWindow::CoreLoadScene( const std::string& filename )
		{
			Data::SceneData* sceneData = BinarySerialization::Reader::LoadSceneData( filename );
			if ( sceneData == nullptr )
			{
				TCMFAILURE( "MainWindow::CoreLoadScene() - couldn't retrieve SceneData" );
				return false;
			}

			Engine::TCMScene* newScene = sceneData->Extract();
			delete sceneData;
			if ( newScene == nullptr )
			{
				TCMFAILURE( "MainWindow::CoreLoadScene() - couldn't retrieve newScene" );
				return false;
			}

			m_scene = newScene;

			Engine::Core::ChangeScene( m_scene );
			sceneFilePath.assign( filename );
			RefreshComponents();
			return true;
		}

		bool MainWindow::CoreLoadProject( const std::string& filename )
		{
			Data::ProjectData* projectData = BinarySerialization::Reader::LoadProjectData( filename );
			if ( projectData == nullptr )
			{
				TCMFAILURE( "MainWindow::CoreLoadScene() - couldn't retrieve ProjectData" );
				return false;
			}

			Engine::Project* newProject = projectData->Extract();
			delete projectData;
			if ( newProject == nullptr )
			{
				TCMFAILURE( "MainWindow::CoreLoadScene() - couldn't retrieve newProject" );
				return false;
			}
			m_project = newProject;

			Engine::Core::SetProject( m_project );
			projectFilePath = filename;

			CoreLoadScene( m_project->GetScenePaths()->at( 0 ) );
			RefreshComponents();

			return true;
		}

		bool MainWindow::CoreSaveScene()
		{
			Data::SceneData* sceneData = new Data::SceneData( m_scene );
			bool result = BinarySerialization::Writer::SaveSceneData( sceneData );
			delete sceneData;
			return result;
		}

		bool MainWindow::CoreSaveProject()
		{
			Data::ProjectData* projectData = new Data::ProjectData( m_project );
			bool result = BinarySerialization::Writer::SaveProjectData( projectData );
			delete projectData;
			return result;
		}

		bool MainWindow::CoreSaveScene( const std::string& filename )
		{
			sceneFilePath = filename;

			Data::SceneData* sceneData = new Data::SceneData( m_scene );
			sceneData->SetFilename( filename );
			bool result = BinarySerialization::Writer::SaveSceneData( sceneData );
			delete sceneData;

			if ( result )
				m_project->AddScenePath( filename );
			return result;
		}

		bool MainWindow::CoreSaveProject( const std::string& filename )
		{
			projectFilePath = filename;

			Data::ProjectData* projectData = new Data::ProjectData( m_project );
			projectData->SetFilename( filename );
			bool result = BinarySerialization::Writer::SaveProjectData( projectData );
			delete projectData;
			return result;
		}

		Engine::TCMScene* MainWindow::CoreGetScene() const
		{
			return m_scene;
		}

		Engine::TCMSceneObject* MainWindow::CoreGetSceneObject() const
		{
			return m_sceneObject;
		}

		Engine::Project* MainWindow::CoreGetProject() const
		{
			return m_project;
		}

		void MainWindow::CoreRegister( EditorComponent* component )
		{
			components.push_back( component );
			component->Refresh( m_project, m_scene );
		}

		void MainWindow::CoreUnregister( EditorComponent* component )
		{
			for ( auto iter = components.begin(); iter != components.end(); ++iter )
				if ( *iter == component )
				{
					components.erase( iter );
					break;
				}
		}

		void MainWindow::CoreSelectSceneObject( Engine::TCMSceneObject* object )
		{
			m_sceneObject = object;

			for ( EditorComponent* component : components )
			{
				component->SetSceneObject( object );
			}
		}

		void MainWindow::RefreshComponents()
		{
			Engine::TCMScene* scene = m_savedScene ? m_savedScene : m_scene;
			TCM::Engine::Core::ChangeScene( scene );
			EditorCommandManager::StaticRefresh( m_project, scene );
			for ( EditorComponent* component : components )
			{
				component->Refresh( m_project, scene );
			}
		}

		void MainWindow::OnPlayEvent()
		{
			// Was not on pause, first time running
			if ( !m_savedScene )
			{
				Editor::Data::SceneData sceneData = Editor::Data::SceneData( m_scene );
				m_savedScene = sceneData.Extract();

				Engine::Time::Reset();
				Engine::Core::RefreshProjectSettings();

				Engine::Core::EditorChangeScene( m_savedScene );

				RefreshComponents();
			}

			CoreOnPlayEvent();
		}

		void MainWindow::OnStopEvent()
		{
			if ( m_savedScene )
			{
				m_savedScene = nullptr;
			}

			if ( m_scene )
			{
				Engine::Core::ChangeScene( m_scene );
				RefreshComponents();
			}

			CoreOnStopEvent();
		}

		void MainWindow::OnDrawedEvent()
		{
			m_canRunCore = true;
			CoreOnDrawedEvent();
		}

		void MainWindow::RefreshCore()
		{
			if ( m_canRunCore )
			{
				m_canRunCore = false;
				TCM::Engine::Core::RunOnce();
			}
		}

		void MainWindow::OnCommandChangeCallback()
		{
			bool redoEnabled = EditorCommandManager::CanRedo();
			bool undoEnabled = EditorCommandManager::CanUndo();

			SetEnableUndo( undoEnabled );
			SetEnableRedo( redoEnabled );
		}
	}
}
