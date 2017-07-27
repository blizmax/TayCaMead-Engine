#pragma once

#include "DLLDefine.h"
#include "TCMMemory/TCMEventManager.hpp"

#define RATCHET_OBJ	false
#define MAX_RATCHET 1

#define LIGHT_COUNT 1

#define JAK_OBJ		false
#define TANK_OBJ	false
#define IRONMAN_FBX	false

#define CUBE_MESH   false
#define TUBE_MESH	false
#define CONE_MESH	false
#define SPHERE_MESH	true
#define CAPSULE_MESH	false //Bug dans tangeantes
#define TORUS_MESH	false //Bug dans tangeantes

#define TCMEDITOR

namespace TCM
{
	namespace Engine
	{
#ifdef TCMEDITOR
		class EditorLifecycle;
		typedef EditorLifecycle CustomLifecycle;
#else
		class Lifecycle;
		typedef Lifecycle CustomLifecycle;
#endif

		class TCMScene;
		class Project;

		class Core
		{
		public:
			Core();
			~Core();

			static TCMCORE_DLL void Initialize();
			static TCMCORE_DLL void ResizeRenderer( const int width, const int height );
			static TCMCORE_DLL void SetProject( Project* project = nullptr );
			static TCMCORE_DLL void Run();
			static TCMCORE_DLL void RunExampleScene();
			static TCMCORE_DLL void RunOnce();
			static TCMCORE_DLL void GenExampleScene();
			static TCMCORE_DLL void Shutdown();

			static TCMCORE_DLL void EditorChangeScene( TCMScene* scene );

			static TCMCORE_DLL void ChangeScene( TCMScene* scene );
			static TCMCORE_DLL void ChangeScene( const int index );

			static TCMCORE_DLL void* GetNativeWindow();
			static TCMCORE_DLL Event<>& GetUpdateCallback();
			static TCMCORE_DLL Event<>& GetRenderCallback();

			static TCMCORE_DLL Event<>& GetPlayEvent();
			static TCMCORE_DLL Event<>& GetPauseEvent();
			static TCMCORE_DLL Event<>& GetStopEvent();
			static TCMCORE_DLL Event<>& GetNextFrameEvent();

			static TCMCORE_DLL TCMScene* GetExampleScene();
			static TCMCORE_DLL void RefreshProjectSettings();

		private:
			static Core& GetInstance();

			void CoreInitialize();
			void CoreSetProject( Project* project );
			void CoreRun() const;
			void CoreRunOnce() const;
			void CoreGenExampleScene();
			void CoreShutdown();

			Event<>& CoreGetUpdateCallback() const;

			void CoreEditorChangeScene( TCMScene* scene );

			void CoreChangeScene( TCMScene* scene );
			void CoreChangeScene( const int index );

			void CoreRefreshProjectSettings() const;

			TCMScene* CoreGetExampleScene();

			TCMScene* m_currentScene;
			Project* m_currentProject;

			CustomLifecycle* lifecycle;

			Event<> m_OnPlay;
			Event<> m_OnPause;
			Event<> m_OnStop;
			Event<> m_OnNextFrame;

			void Play();
			void Pause();
			void Stop();
			void NextFrame();

			void Quit();

			bool m_bQuit = false;

			TCMScene* m_exampleScene = nullptr;
		};
	}
}
