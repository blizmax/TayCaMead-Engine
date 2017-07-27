#pragma once
#include <vector>
#include "TCMMemory/TCMEventManager.hpp"

#define PROJECT_EXTENSION "tcmproj"
#define SCENE_EXTENSION "tcmscene"
#define SCENE_OBJ_EXTENSION "tcmscobj"

#define TCM_SCOBJ_MIME_DATA "application/tcmscobj"

namespace TCM
{
	namespace Engine
	{
		class TCMScene;
		class TCMSceneObject;
		class Project;
	}

	namespace Editor
	{
		enum class IconType : int
		{
			ADD = 0,
			BUILD,
			CANCEL,
			COPY,
			PASTE,
			CUT,
			OPEN,
			PAUSE,
			PLAY,
			NEXT,
			UNDO,
			REDO,
			SAVE,
			SETTINGS,
			REMOVE,
			COUNT
		};

		class EditorComponent;

		class MainWindow abstract
		{
		public:

			MainWindow();
			virtual ~MainWindow();

			static std::string GetDocPath();

			static void NewScene();
			static void NewProject();

			static void LoadScene( const std::string& filename );
			static void LoadProject( const std::string& fillename );

			static bool SaveScene();
			static bool SaveProject();
			static void SaveScene( const std::string& filename );
			static void SaveProject( const std::string& filename );

			static std::string GetProjectFilepath();
			static std::string GetSceneFilepath();

			static Engine::TCMScene* GetScene();
			static Engine::TCMSceneObject* GetSceneObject();
			static Engine::Project* GetProject();

			static void Register( TCM::Editor::EditorComponent* component );
			static void Unregister( TCM::Editor::EditorComponent* component );
			static void SelectSceneObject( Engine::TCMSceneObject* object );

			static Engine::Event<>& GetUpdateCallback();

			static Engine::Event<>& GetPauseEvent();
			static Engine::Event<>& GetPlayEvent();
			static Engine::Event<>& GetNextFrameEvent();
			static Engine::Event<>& GetStopEvent();

			static std::string GetIconPath( IconType icon );

		protected:

			static MainWindow* instance;

			void CoreNewScene();
			void CoreNewProject();

			bool CoreLoadScene( const std::string& filename );
			bool CoreLoadProject( const std::string& fillename );

			bool CoreSaveScene();
			bool CoreSaveProject();
			bool CoreSaveScene( const std::string& filename );
			bool CoreSaveProject( const std::string& fillename );

			Engine::TCMScene* CoreGetScene() const;
			Engine::TCMSceneObject* CoreGetSceneObject() const;
			Engine::Project* CoreGetProject() const;

			void CoreRegister( TCM::Editor::EditorComponent* component );
			void CoreUnregister( TCM::Editor::EditorComponent* component );
			void CoreSelectSceneObject( Engine::TCMSceneObject* object );
			void RefreshComponents();

			void OnPlayEvent();
			void OnStopEvent();

			virtual void CoreOnPlayEvent() = 0;
			virtual void OnPauseEvent() = 0;
			virtual void CoreOnStopEvent() = 0;
			virtual void OnNextFrameEvent() = 0;

			void OnCommandChangeCallback();

			virtual void SetEnableUndo( const bool value )
			{
			};

			virtual void SetEnableRedo( const bool value )
			{
			};

			void OnDrawedEvent();

			virtual void CoreOnDrawedEvent()
			{
			};

			void RefreshCore();

			Engine::TCMScene* m_scene = nullptr;
			Engine::TCMScene* m_savedScene = nullptr;

			Engine::TCMSceneObject* m_sceneObject = nullptr;
			Engine::Project* m_project = nullptr;

			std::string sceneFilePath;
			std::string projectFilePath;
			std::vector<EditorComponent *> components;

			bool m_canRunCore = true;

			std::string m_color = "white";
		};
	}
}
